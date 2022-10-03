#include "GameplayEvent.h"

#include "GameFramework/LDGameMode.h"

void UGameplayEvent::Start_Implementation()
{
	OnStarted.Broadcast(this);
}

void UGameplayEvent::Complete_Implementation()
{
	OnCompleted.Broadcast(this);
}

ALDGameMode* UGameplayEvent::GetGameMode() const
{
	return Cast<ALDGameMode>(GetOuter());
}
