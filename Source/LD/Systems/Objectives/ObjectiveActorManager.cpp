#include "ObjectiveActorManager.h"

#include "Engine/World.h"
#include "GameFramework/LDGameMode.h"

void AObjectiveActorManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetAuthGameMode<ALDGameMode>()->ObjectiveActorManager = this;
}
