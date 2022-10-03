// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Systems/Objectives/GameplayEvent.h"
#include "LDNPCats.generated.h"

class ALDNPCats;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSatisfactionUpdated, ALDNPCats*, Guest, int32, Satisfaction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishedFollowingSpline, ALDNPCats*, Guest);

UENUM(BlueprintType)
enum class ECatAnimationState : uint8
{
	Idle,
	Moving,
	Sleeping,
	Request,
};

UCLASS()
class LD_API ALDNPCats : public AActor
{
	GENERATED_BODY()
	
public:
	ALDNPCats();

	void SetCurrentService(UGameplayEvent* ServiceRequest);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UGameplayEvent* GetCurrentServiceRequest() const { return CurrentServiceRequest; }
	bool CanRequestService() const;
	bool IsWaitingForService() const;

	UFUNCTION(BlueprintNativeEvent)
	void FollowSpline(bool bForward, class USplineComponent* Spline);
	UFUNCTION(BlueprintNativeEvent)
	void CheckOut();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetSatisfaction() const { return Satisfaction; }
	void MoveToLocation(FVector Location, float Duration);
	
	UPROPERTY(BlueprintAssignable)
	FOnSatisfactionUpdated OnSatisfactionUpdated;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnFinishedFollowingSpline OnFinishedFollowingSpline;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnServiceRequestCompleted(UGameplayEvent* ServiceRequest);
	UFUNCTION()
	void UpdateSatisfaction();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECatAnimationState CurrentAnimationState = ECatAnimationState::Idle;
	void CheckAnimationState();
	void UpdateAnimation(ECatAnimationState);
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperFlipbookComponent* Flipbook = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Satisfaction = 3;
	bool bServiceRequestChanged = false;
	UPROPERTY()
	UGameplayEvent* CurrentServiceRequest = nullptr;
	UPROPERTY()
	UGameplayEvent* LastServiceRequest = nullptr;
	UPROPERTY()
	USplineComponent* SplineComponent = nullptr;

	bool bMoving = false;
	float ElapsedMoveTime = 0.f;
	float MoveDuration = 0.f;
	FVector MoveStartLocation;
	FVector MoveEndLocation;
};
