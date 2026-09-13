#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"

class SListOptimizedScrollBox : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_TwoParams(FOnVisibleRangeChanged, int32, int32);

	SLATE_BEGIN_ARGS(SListOptimizedScrollBox)
		: _Orientation(Orient_Vertical)
		, _EntryExtent(40.0f)
		, _OverscanCount(1)
		, _ItemCount(0)
	{
	}
		SLATE_ARGUMENT(EOrientation, Orientation)
		SLATE_ARGUMENT(float, EntryExtent)
		SLATE_ARGUMENT(int32, OverscanCount)
		SLATE_ARGUMENT(int32, ItemCount)
		SLATE_EVENT(FOnVisibleRangeChanged, OnVisibleRangeChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetOrientation(EOrientation InOrientation);
	void SetEntryExtent(float InEntryExtent);
	void SetOverscanCount(int32 InOverscanCount);
	void SetItemCount(int32 InItemCount);
	void SetVisibleEntries(int32 FirstItemIndex, const TArray<TSharedRef<SWidget>>& InEntries);
	void ClearEntries();
	void RequestRefresh();

	virtual void Tick(
		const FGeometry& AllottedGeometry,
		const double InCurrentTime,
		const float InDeltaTime) override;

private:
	void AddExtentSlot(float Extent);
	void AddEntrySlot(const TSharedRef<SWidget>& Entry);

	TSharedPtr<SScrollBox> ScrollBox;
	EOrientation Orientation = Orient_Vertical;
	float EntryExtent = 40.0f;
	int32 OverscanCount = 1;
	int32 ItemCount = 0;
	int32 LastFirstItemIndex = INDEX_NONE;
	int32 LastVisibleItemCount = INDEX_NONE;
	bool bRefreshRequested = true;
	FOnVisibleRangeChanged OnVisibleRangeChanged;
};
