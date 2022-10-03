#include "ItemActor.h"

#include "InventoryComponent.h"

AItemActor::AItemActor()
{
	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComponent"));
	SetRootComponent(PaperSprite);
}

// Caller must make sure this is valid
void AItemActor::Pickup(UInventoryComponent* NewInventory)
{
	Inventory = NewInventory;
	OnPickup();
}

void AItemActor::Drop()
{
	if (!IsPickedUp())
	{
		return;
	}

	OnDrop();
	Inventory = nullptr;
}

bool AItemActor::IsPickedUp() const
{
	return IsValid(Inventory);
}

void AItemActor::OnPickup_Implementation()
{
	// API stub
}

void AItemActor::OnDrop_Implementation()
{
	// API stub
}