#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListOptimizedScrollBoxTestWidget.generated.h"

class UListOptimizedScrollBox;

/** Native host widget that exercises the list without requiring a test Blueprint hierarchy. */
UCLASS()
class LISTONLYSCROLLBOX_API UListOptimizedScrollBoxTestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box|Test")
	void PopulateTestItems(int32 ItemCount = 20);

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box|Test")
	void AddTestItem();

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box|Test")
	bool RemoveLastTestItem();

	UFUNCTION(BlueprintCallable, Category = "List Optimized Scroll Box|Test")
	void ClearTestItems();

	UFUNCTION(BlueprintPure, Category = "List Optimized Scroll Box|Test")
	UListOptimizedScrollBox* GetTestList() const { return ListWidget; }

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UListOptimizedScrollBox> ListWidget;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> TestItems;
};
