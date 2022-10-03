#pragma once

#include <GameFramework/Actor.h>
#include <PaperSpriteComponent.h>

#include "ItemActor.generated.h"

class UInventoryComponent;

UCLASS(Blueprintable, BlueprintType)
class AItemActor : public AActor
{
	GENERATED_BODY()
	
public:
	AItemActor();

	void Pickup(UInventoryComponent* NewInventory);
	void Drop();

	UFUNCTION(BlueprintCallable)
	bool IsPickedUp() const;
	FORCEINLINE UInventoryComponent* GetInventory() const { return Inventory; }
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnPickup();
	UFUNCTION(BlueprintNativeEvent)
	void OnDrop();
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* PaperSprite = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* Inventory = nullptr;
};
