#include "RoomEvent.h"

#include "GameFramework/LDGameMode.h"
#include "Systems/LDRooms.h"

void URoomEvent::SetRoom(ALDRooms* NewRoom)
{
	Room = NewRoom;
	Room->GetGuest()->SetCurrentService(this);
}

void URoomEvent::Complete_Implementation()
{
	// We're only doing one room event and then checking them out
	ALDNPCats* Guest = Room->GetGuest();
	// Guest->CheckOut();
	GetGameMode()->AddPoints(5 * (Guest->GetSatisfaction() + 1));
	Room->RemoveGuest();
	Guest->Destroy();
	
	Super::Complete_Implementation();
}
