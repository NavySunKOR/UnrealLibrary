#include "ListOptimizedScrollBoxTestEntry.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

TSharedRef<SWidget> UListOptimizedScrollBoxTestEntry::RebuildWidget()
{
	return SNew(SBorder)
		.Padding(FMargin(12.0f, 8.0f))
		[
			SAssignNew(LabelText, STextBlock)
			.Text(DisplayText)
		];
}

void UListOptimizedScrollBoxTestEntry::OnListItemObjectSet_Implementation(UObject* ListItemObject)
{
	DisplayText = IsValid(ListItemObject)
		? FText::FromString(ListItemObject->GetName())
		: FText::FromString(TEXT("Invalid Item"));

	UpdateLabel();
}

void UListOptimizedScrollBoxTestEntry::OnEntryReleased_Implementation()
{
	DisplayText = FText::GetEmpty();
	UpdateLabel();
}

void UListOptimizedScrollBoxTestEntry::UpdateLabel()
{
	if (LabelText.IsValid())
	{
		LabelText->SetText(DisplayText);
	}
}
