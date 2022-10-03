#pragma once

#include "ItemActor.h"

#include <Components/ActorComponent.h>

#include "InventoryComponent.generated.h"

UCLASS()
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	void Pickup(AItemActor* Item);
	void Drop(AItemActor* Item);
	void DropSlot(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TArray<AItemActor*> GetItemsCopy() const { return Items; }
	FORCEINLINE const TArray<AItemActor*>& GetItems() const { return Items; }
	
protected:
	UPROPERTY()
	TArray<AItemActor*> Items;
};
