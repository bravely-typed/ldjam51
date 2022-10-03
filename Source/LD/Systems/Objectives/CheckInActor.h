#pragma once

#include "CheckInEvent.h"
#include "ObjectiveActor.h"

#include "CheckInActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckInQueueUpdate);

UCLASS()
class ACheckInActor : public AObjectiveActor
{
	GENERATED_BODY()
	
public:
	virtual void HandleAction_Implementation(FName ActionName) override;
	
	void CheckIn();
	void AddToCheckInQueue(UCheckInEvent* CheckInEvent);
	void RemoveFromCheckInQueue(UCheckInEvent* CheckInEvent);

	UFUNCTION(BlueprintNativeEvent)
	void OnIsInteractable();
	UFUNCTION(BlueprintNativeEvent)
	void OnNotInteractable();

	UPROPERTY(BlueprintAssignable)
	FOnCheckInQueueUpdate OnQueueUpdate;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetQueueCount() const { return CheckInQueue.Num(); }
	
protected:
	void MoveGuestToQueueLocation(int32 Index, float Duration);
	UFUNCTION()
	void OnSatisfactionChanged(ALDNPCats* Guest, int32 Satisfaction);

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> QueuePositions;
	UPROPERTY(EditAnywhere)
	float AddToQueueMoveDuration = 2.f;
	UPROPERTY(EditAnywhere)
	float AdvanceInQueueMoveDuration = 2.f;
	UPROPERTY(EditAnywhere)
	FName CheckInActionName;
	UPROPERTY()
	TArray<UCheckInEvent*> CheckInQueue;
	
};
