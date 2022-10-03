#include "InventoryComponent.h"

void UInventoryComponent::Pickup(AItemActor* Item)
{
	if (!Item->IsPickedUp())
	{
		Items.Add(Item);
		Item->Pickup(this);
	}
}

void UInventoryComponent::Drop(AItemActor* Item)
{
	if (Items.Remove(Item) > 0)
	{
		Item->Drop();
	}
}

void UInventoryComponent::DropSlot(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		AItemActor* ItemActor = Items[Index];
		Items.RemoveAt(Index);
		ItemActor->Drop();
	}
}
