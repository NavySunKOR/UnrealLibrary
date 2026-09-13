#include "ListOptimizedScrollBoxTestWidget.h"

#include "ListOptimizedScrollBox.h"
#include "ListOptimizedScrollBoxTestEntry.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SNullWidget.h"

void UListOptimizedScrollBoxTestWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ListWidget)
	{
		ListWidget->SetItemClass(UListOptimizedScrollBoxTestEntry::StaticClass());
	}
	PopulateTestItems();
}

void UListOptimizedScrollBoxTestWidget::PopulateTestItems(int32 ItemCount)
{
	ClearTestItems();

	const int32 SafeItemCount = FMath::Max(0, ItemCount);
	TestItems.Reserve(SafeItemCount);

	for (int32 Index = 0; Index < SafeItemCount; ++Index)
	{
		AddTestItem();
	}
}

void UListOptimizedScrollBoxTestWidget::AddTestItem()
{
	if (!ListWidget)
	{
		return;
	}

	const FName ItemName = MakeUniqueObjectName(
		this, UObject::StaticClass(), TEXT("ListTestItem"));
	UObject* NewItem = NewObject<UObject>(this, ItemName);

	TestItems.Add(NewItem);
	ListWidget->AddItem(NewItem);
}

bool UListOptimizedScrollBoxTestWidget::RemoveLastTestItem()
{
	if (!ListWidget || TestItems.IsEmpty())
	{
		return false;
	}

	UObject* ItemToRemove = TestItems.Last();
	TestItems.Pop();
	return ListWidget->RemoveItem(ItemToRemove);
}

void UListOptimizedScrollBoxTestWidget::ClearTestItems()
{
	if (ListWidget)
	{
		ListWidget->ClearItems();
	}

	TestItems.Reset();
}
