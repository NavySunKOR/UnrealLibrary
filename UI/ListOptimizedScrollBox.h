// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidgetPool.h"
#include "Components/Widget.h"
#include "ListOptimizedScrollBox.generated.h"

class SInheritedListOptimizedScrollBox;

UCLASS(meta = (DisplayName = "List Optimized Scroll Box"))
class LISTONLYSCROLLBOX_API UListOptimizedScrollBox : public UWidget
{
	GENERATED_BODY()

public:
	UListOptimizedScrollBox(const FObjectInitializer& ObjectInitializer);

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	void AddItem(UObject* InItem);

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	void AddItemAt(UObject* InItem, int32 InIndex);

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	bool RemoveItem(UObject* InItem);

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	bool RemoveItemAt(int32 InIndex);

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	void ClearItems();

	const TArray<TObjectPtr<UObject>>& GetAllItems() const { return Items; }

	UFUNCTION(BlueprintPure, Category = "List Optimized Scroll Box")
	UObject* GetItemAt(int32 InIndex) const;

	UFUNCTION(BlueprintPure, Category = "List Optimized Scroll Box")
	int32 GetNumItems() const { return Items.Num(); }

	UFUNCTION(BlueprintPure, Category = "List Optimized Scroll Box")
	int32 GetNumDisplayedEntries() const { return EntryWidgetPool.GetActiveWidgets().Num(); }

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	void RequestRefresh();

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	void SetOrientation(EOrientation InOrientation);

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	void SetItemClass(TSubclassOf<UUserWidget> InItemClass);

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	void SetEntryExtent(float InEntryExtent);

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box")
	void SetOverscanCount(int32 InOverscanCount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "List Entries",
		meta = (DesignerRebuild, MustImplement = "/Script/ListOnlyScrollBox.ListOptimizedScrollBoxEntry"))
	TSubclassOf<UUserWidget> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scrolling")
	TEnumAsByte<EOrientation> Orientation = Orient_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "List Entries", meta = (ClampMin = "1.0"))
	float EntryExtent = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "List Entries", meta = (ClampMin = "0"))
	int32 OverscanCount = 1;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Items;

private:
	void RebuildVisibleEntries(int32 FirstItemIndex, int32 VisibleItemCount);
	void ReleaseActiveEntries();

	UPROPERTY(Transient)
	FUserWidgetPool EntryWidgetPool;

	TSharedPtr<SInheritedListOptimizedScrollBox> MyListScrollBox;
};
