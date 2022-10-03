#pragma once

#include "GameplayEvent.h"

#include "RoomEvent.generated.h"

class ALDRooms;
UCLASS()
class URoomEvent : public UGameplayEvent
{
	GENERATED_BODY()
	
public:
	void SetRoom(ALDRooms* NewRoom);

	virtual void Complete_Implementation() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	ALDRooms* Room = nullptr;
};
