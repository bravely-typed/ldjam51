#pragma once

#include "CoreMinimal.h"
#include "GameFramework/LDNPCats.h"
#include "Objectives/ObjectiveActor.h"

#include "LDRooms.generated.h"

UENUM(BlueprintType)
enum class ERoomNeed : uint8
{
	None,
	Cleaning,
	RoomService,
	BugExtermination,
};

UCLASS()
class LD_API ALDRooms : public AObjectiveActor
{
	GENERATED_BODY()
	
public:
	bool SetGuest(ALDNPCats* NewGuest);
	bool RemoveGuest();

	FORCEINLINE ALDNPCats* GetGuest() const { return Guest; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsVacant() const { return !IsValid(Guest); }
	FORCEINLINE AActor* GetSplineActor() const { return Spline; }
	
protected:
	UPROPERTY(BlueprintReadOnly)
	ALDNPCats* Guest = nullptr;
	UPROPERTY(BlueprintReadWrite)
	ERoomNeed RoomNeed = ERoomNeed::None;
	UPROPERTY(EditInstanceOnly)
	AActor* Spline = nullptr;
};
