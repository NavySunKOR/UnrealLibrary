#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ListOptimizedScrollBoxEntry.generated.h"

UINTERFACE(BlueprintType)
class LISTONLYSCROLLBOX_API UListOptimizedScrollBoxEntry : public UInterface
{
	GENERATED_BODY()
};

class LISTONLYSCROLLBOX_API IListOptimizedScrollBoxEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "List Optimized Scroll Box")
	void OnListItemObjectSet(UObject* ListItemObject);

	UFUNCTION(BlueprintNativeEvent, Category = "List Optimized Scroll Box")
	void OnEntryReleased();
};
