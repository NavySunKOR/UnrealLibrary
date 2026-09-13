// Fill out your copyright notice in the Description page of Project Settings.


#include "ListOptimizedScrollBox.h"

#include "ListOptimizedScrollBoxEntry.h"
#include "SInheritedListOptimizedScrollBox.h"
#include "Blueprint/UserWidget.h"

UListOptimizedScrollBox::UListOptimizedScrollBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EntryWidgetPool(*this)
{
	SetVisibilityInternal(ESlateVisibility::Visible);
	SetClipping(EWidgetClipping::ClipToBounds);
}

TSharedRef<SWidget> UListOptimizedScrollBox::RebuildWidget()
{
	SAssignNew(MyListScrollBox, SInheritedListOptimizedScrollBox)
		.Orientation(Orientation)
		.EntryExtent(EntryExtent)
		.OverscanCount(OverscanCount)
		.ItemCount(Items.Num())
		.OnVisibleRangeChanged(
			SInheritedListOptimizedScrollBox::FOnVisibleRangeChanged::CreateUObject(
				this, &UListOptimizedScrollBox::RebuildVisibleEntries));

	return MyListScrollBox.ToSharedRef();
}

void UListOptimizedScrollBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyListScrollBox.IsValid())
	{
		MyListScrollBox->SetOrientation(Orientation);
		MyListScrollBox->SetEntryExtent(EntryExtent);
		MyListScrollBox->SetOverscanCount(OverscanCount);
		MyListScrollBox->SetItemCount(Items.Num());
		MyListScrollBox->RequestRefresh();
	}
}

void UListOptimizedScrollBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	EntryWidgetPool.ReleaseAllSlateResources();
	MyListScrollBox.Reset();
}

void UListOptimizedScrollBox::AddItem(UObject* InItem)
{
	if (!IsValid(InItem) || Items.Contains(InItem))
	{
		return;
	}

	Items.Add(InItem);
	RequestRefresh();
}

void UListOptimizedScrollBox::AddItemAt(UObject* InItem, int32 InIndex)
{
	if (!IsValid(InItem) || Items.Contains(InItem))
	{
		return;
	}

	Items.Insert(InItem, FMath::Clamp(InIndex, 0, Items.Num()));
	RequestRefresh();
}

bool UListOptimizedScrollBox::RemoveItem(UObject* InItem)
{
	const int32 ItemIndex = Items.IndexOfByKey(InItem);
	return RemoveItemAt(ItemIndex);
}

bool UListOptimizedScrollBox::RemoveItemAt(int32 InIndex)
{
	if (!Items.IsValidIndex(InIndex))
	{
		return false;
	}

	Items.RemoveAt(InIndex);
	RequestRefresh();
	return true;
}

void UListOptimizedScrollBox::ClearItems()
{
	if (Items.IsEmpty())
	{
		return;
	}

	Items.Reset();
	RequestRefresh();
}

UObject* UListOptimizedScrollBox::GetItemAt(int32 InIndex) const
{
	return Items.IsValidIndex(InIndex) ? Items[InIndex].Get() : nullptr;
}

void UListOptimizedScrollBox::RequestRefresh()
{
	if (MyListScrollBox.IsValid())
	{
		MyListScrollBox->SetItemCount(Items.Num());
		MyListScrollBox->RequestRefresh();
	}
}

void UListOptimizedScrollBox::SetOrientation(EOrientation InOrientation)
{
	Orientation = InOrientation;

	if (MyListScrollBox.IsValid())
	{
		MyListScrollBox->SetOrientation(InOrientation);
	}
}

void UListOptimizedScrollBox::SetItemClass(TSubclassOf<UUserWidget> InItemClass)
{
	if (ItemClass == InItemClass)
	{
		return;
	}

	ItemClass = InItemClass;
	RequestRefresh();
}

void UListOptimizedScrollBox::SetEntryExtent(float InEntryExtent)
{
	EntryExtent = FMath::Max(1.0f, InEntryExtent);

	if (MyListScrollBox.IsValid())
	{
		MyListScrollBox->SetEntryExtent(EntryExtent);
	}
}

void UListOptimizedScrollBox::SetOverscanCount(int32 InOverscanCount)
{
	OverscanCount = FMath::Max(0, InOverscanCount);

	if (MyListScrollBox.IsValid())
	{
		MyListScrollBox->SetOverscanCount(OverscanCount);
	}
}

void UListOptimizedScrollBox::RebuildVisibleEntries(
	int32 FirstItemIndex,
	int32 VisibleItemCount)
{
	if (!MyListScrollBox.IsValid())
	{
		return;
	}

	MyListScrollBox->ClearEntries();
	ReleaseActiveEntries();

	const int32 LastItemIndexExclusive = FMath::Min(
		FirstItemIndex + VisibleItemCount, Items.Num());
	const int32 EntryCount = FMath::Max(0, LastItemIndexExclusive - FirstItemIndex);

	if (!ItemClass
		|| !ItemClass->ImplementsInterface(UListOptimizedScrollBoxEntry::StaticClass())
		|| EntryCount == 0)
	{
		const TArray<TSharedRef<SWidget>> EmptyEntries;
		MyListScrollBox->SetVisibleEntries(FirstItemIndex, EmptyEntries);
		return;
	}

	for (int32 EntryIndex = 0; EntryIndex < EntryCount; ++EntryIndex)
	{
		EntryWidgetPool.GetOrCreateInstance(ItemClass);
	}

	EntryWidgetPool.RebuildWidgets();

	TArray<TSharedRef<SWidget>> SlateEntries;
	SlateEntries.Reserve(EntryCount);

	const TArray<UUserWidget*>& ActiveEntries = EntryWidgetPool.GetActiveWidgets();
	for (int32 EntryIndex = 0; EntryIndex < EntryCount; ++EntryIndex)
	{
		UUserWidget* EntryWidget = ActiveEntries[EntryIndex];
		const int32 ItemIndex = FirstItemIndex + EntryIndex;
		if (!EntryWidget || !Items.IsValidIndex(ItemIndex))
		{
			continue;
		}

		IListOptimizedScrollBoxEntry::Execute_OnListItemObjectSet(EntryWidget, Items[ItemIndex]);

		if (const TSharedPtr<SWidget> CachedWidget = EntryWidget->GetCachedWidget())
		{
			SlateEntries.Add(CachedWidget.ToSharedRef());
		}
	}

	MyListScrollBox->SetVisibleEntries(FirstItemIndex, SlateEntries);
}

void UListOptimizedScrollBox::ReleaseActiveEntries()
{
	for (UUserWidget* EntryWidget : EntryWidgetPool.GetActiveWidgets())
	{
		if (EntryWidget)
		{
			IListOptimizedScrollBoxEntry::Execute_OnEntryReleased(EntryWidget);
		}
	}

	EntryWidgetPool.ReleaseAll();
}
