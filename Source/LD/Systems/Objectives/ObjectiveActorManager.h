#pragma once
#include "CheckInActor.h"
#include "GameFramework/Actor.h"

#include "ObjectiveActorManager.generated.h"

class ALDRooms;
class AObjectiveActor;
/*
 * This is bad, but it's quicker to just make a bespoke class to stitch scene references.
 */
UCLASS(Blueprintable, BlueprintType)
class AObjectiveActorManager : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TArray<ALDRooms*> Rooms;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	ACheckInActor* CheckIn = nullptr;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	AObjectiveActor* RoomService = nullptr;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	AActor* SpawnPoint = nullptr;
};
