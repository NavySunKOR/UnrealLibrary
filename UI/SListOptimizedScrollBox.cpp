#include "SListOptimizedScrollBox.h"

#include "Widgets/Layout/SBox.h"

void SListOptimizedScrollBox::Construct(const FArguments& InArgs)
{
	Orientation = InArgs._Orientation;
	EntryExtent = FMath::Max(1.0f, InArgs._EntryExtent);
	OverscanCount = FMath::Max(0, InArgs._OverscanCount);
	ItemCount = FMath::Max(0, InArgs._ItemCount);
	OnVisibleRangeChanged = InArgs._OnVisibleRangeChanged;

	ChildSlot
	[
		SAssignNew(ScrollBox, SScrollBox)
		.Orientation(Orientation)
	];
}

void SListOptimizedScrollBox::SetOrientation(EOrientation InOrientation)
{
	if (Orientation == InOrientation)
	{
		return;
	}

	Orientation = InOrientation;
	if (ScrollBox.IsValid())
	{
		ScrollBox->SetOrientation(InOrientation);
	}

	RequestRefresh();
}

void SListOptimizedScrollBox::SetEntryExtent(float InEntryExtent)
{
	const float ClampedExtent = FMath::Max(1.0f, InEntryExtent);
	if (FMath::IsNearlyEqual(EntryExtent, ClampedExtent))
	{
		return;
	}

	EntryExtent = ClampedExtent;
	RequestRefresh();
}

void SListOptimizedScrollBox::SetOverscanCount(int32 InOverscanCount)
{
	const int32 ClampedOverscanCount = FMath::Max(0, InOverscanCount);
	if (OverscanCount == ClampedOverscanCount)
	{
		return;
	}

	OverscanCount = ClampedOverscanCount;
	RequestRefresh();
}

void SListOptimizedScrollBox::SetItemCount(int32 InItemCount)
{
	const int32 ClampedItemCount = FMath::Max(0, InItemCount);
	if (ItemCount == ClampedItemCount)
	{
		return;
	}

	ItemCount = ClampedItemCount;
	RequestRefresh();
}

void SListOptimizedScrollBox::SetVisibleEntries(
	int32 FirstItemIndex,
	const TArray<TSharedRef<SWidget>>& InEntries)
{
	ClearEntries();

	if (!ScrollBox.IsValid())
	{
		return;
	}

	const int32 ClampedFirstItemIndex = FMath::Clamp(FirstItemIndex, 0, ItemCount);
	AddExtentSlot(ClampedFirstItemIndex * EntryExtent);

	for (const TSharedRef<SWidget>& Entry : InEntries)
	{
		AddEntrySlot(Entry);
	}

	const int32 RemainingItemCount = FMath::Max(
		0, ItemCount - ClampedFirstItemIndex - InEntries.Num());
	AddExtentSlot(RemainingItemCount * EntryExtent);

	Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
}

void SListOptimizedScrollBox::ClearEntries()
{
	if (ScrollBox.IsValid())
	{
		ScrollBox->ClearChildren();
	}
}

void SListOptimizedScrollBox::RequestRefresh()
{
	bRefreshRequested = true;
	Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
}

void SListOptimizedScrollBox::Tick(
	const FGeometry& AllottedGeometry,
	const double InCurrentTime,
	const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (!ScrollBox.IsValid())
	{
		return;
	}

	const FVector2f LocalSize = AllottedGeometry.GetLocalSize();
	const float ViewportExtent = Orientation == Orient_Vertical ? LocalSize.Y : LocalSize.X;
	if (ViewportExtent <= 0.0f)
	{
		return;
	}

	const float ScrollOffset = FMath::Max(0.0f, ScrollBox->GetScrollOffset());
	const int32 FirstVisibleItemIndex = FMath::FloorToInt(ScrollOffset / EntryExtent);
	const int32 LastVisibleItemIndex = FMath::CeilToInt(
		(ScrollOffset + ViewportExtent) / EntryExtent);

	const int32 FirstItemIndex = FMath::Clamp(
		FirstVisibleItemIndex - OverscanCount, 0, ItemCount);
	const int32 LastItemIndexExclusive = FMath::Clamp(
		LastVisibleItemIndex + OverscanCount, FirstItemIndex, ItemCount);
	const int32 VisibleItemCount = LastItemIndexExclusive - FirstItemIndex;

	if (!bRefreshRequested
		&& LastFirstItemIndex == FirstItemIndex
		&& LastVisibleItemCount == VisibleItemCount)
	{
		return;
	}

	bRefreshRequested = false;
	LastFirstItemIndex = FirstItemIndex;
	LastVisibleItemCount = VisibleItemCount;
	OnVisibleRangeChanged.ExecuteIfBound(FirstItemIndex, VisibleItemCount);
}

void SListOptimizedScrollBox::AddExtentSlot(float Extent)
{
	if (!ScrollBox.IsValid())
	{
		return;
	}

	if (Orientation == Orient_Vertical)
	{
		ScrollBox->AddSlot()
		[
			SNew(SBox)
			.HeightOverride(Extent)
		];
	}
	else
	{
		ScrollBox->AddSlot()
		[
			SNew(SBox)
			.WidthOverride(Extent)
		];
	}
}

void SListOptimizedScrollBox::AddEntrySlot(const TSharedRef<SWidget>& Entry)
{
	if (!ScrollBox.IsValid())
	{
		return;
	}

	if (Orientation == Orient_Vertical)
	{
		ScrollBox->AddSlot()
		[
			SNew(SBox)
			.HeightOverride(EntryExtent)
			[
				Entry
			]
		];
	}
	else
	{
		ScrollBox->AddSlot()
		[
			SNew(SBox)
			.WidthOverride(EntryExtent)
			[
				Entry
			]
		];
	}
}
