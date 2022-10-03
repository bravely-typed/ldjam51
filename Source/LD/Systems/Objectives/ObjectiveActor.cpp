#include "ObjectiveActor.h"

#include "Components/BoxComponent.h"

AObjectiveActor::AObjectiveActor()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	Bounds = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	Bounds->SetupAttachment(GetRootComponent());
}

void AObjectiveActor::HandleAction_Implementation(FName ActionName)
{
	// API stub
}
