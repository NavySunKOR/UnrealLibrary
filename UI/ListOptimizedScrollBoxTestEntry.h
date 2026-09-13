#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListOptimizedScrollBoxEntry.h"
#include "ListOptimizedScrollBoxTestEntry.generated.h"

class STextBlock;

/** Minimal native entry used to verify list item creation, binding, and release. */
UCLASS()
class LISTONLYSCROLLBOX_API UListOptimizedScrollBoxTestEntry
	: public UUserWidget
	, public IListOptimizedScrollBoxEntry
{
	GENERATED_BODY()

public:
	virtual void OnListItemObjectSet_Implementation(UObject* ListItemObject) override;
	virtual void OnEntryReleased_Implementation() override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	void UpdateLabel();

	FText DisplayText = FText::GetEmpty();
	TSharedPtr<STextBlock> LabelText;
};
