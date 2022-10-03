#include "CheckInEvent.h"

#include "Engine/World.h"
#include "GameFramework/LDGameMode.h"

void UCheckInEvent::Start_Implementation()
{
	Super::Start_Implementation();

	const ALDGameMode* GameMode = GetGameMode();

	const FVector SpawnPoint = GameMode->ObjectiveActorManager->SpawnPoint->GetActorLocation();
	const FRotator Rotation = FRotator::ZeroRotator;
	Guest = GameMode->GetWorld()->SpawnActor<ALDNPCats>(GuestClass, SpawnPoint, Rotation);
	Guest->SetCurrentService(this);
	GameMode->ObjectiveActorManager->CheckIn->AddToCheckInQueue(this);
}
