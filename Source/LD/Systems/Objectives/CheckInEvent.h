#pragma once

#include "GameplayEvent.h"
#include "SubclassOf.h"
#include "GameFramework/LDNPCats.h"

#include "CheckInEvent.generated.h"

UCLASS()
class UCheckInEvent : public UGameplayEvent
{
	GENERATED_BODY()
	
public:
	virtual void Start_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ALDNPCats* GetGuest() const { return Guest; }
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALDNPCats> GuestClass;
	UPROPERTY()
	ALDNPCats* Guest = nullptr;
};
