// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "GameFramework/GameModeBase.h"

#include "Systems/Objectives/CheckInEvent.h"
#include "Systems/Objectives/ObjectiveActor.h"
#include "Systems/Objectives/ObjectiveActorManager.h"
#include "Systems/Objectives/RoomEvent.h"

#include "LDGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLDGameModeEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPointsUpdated, int32, Points);

UCLASS(Blueprintable, MinimalAPI)
class ALDGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALDGameMode();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void LaunchGame();
	UFUNCTION(BlueprintCallable)
	void LaunchRound();
	
	UPROPERTY(BlueprintAssignable)
	FLDGameModeEvent OnGameStarted;
	UPROPERTY(BlueprintAssignable)
	FLDGameModeEvent OnGameFinished;
	UPROPERTY(BlueprintAssignable)
	FLDGameModeEvent OnRoundStarted;
	UPROPERTY(BlueprintAssignable)
	FLDGameModeEvent OnRoundFinished;
	UPROPERTY(BlueprintAssignable)
	FLDGameModeEvent OnIntervalStarted;
	UPROPERTY(BlueprintAssignable)
	FLDGameModeEvent OnIntervalFinished;
	UPROPERTY(BlueprintAssignable)
	FLDGameModeEvent OnInstantStarted;
	UPROPERTY(BlueprintAssignable)
	FLDGameModeEvent OnInstantFinished;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int32 RoundsPerGame = 2;
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int32 IntervalsPerRound = 12;
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int32 InstantsPerInterval = 10;

// Objectives
	void GenerateEvent();
	UFUNCTION()
	void RemoveEvent(UGameplayEvent* Event);
	// TODO: Replace with proper location
	FVector GetCheckInSpawnPoint() const { return ObjectiveActorManager->CheckIn->GetActorLocation(); }

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCheckInEvent> CheckInEventClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URoomEvent> RoomServiceEventClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URoomEvent> SquashBugEventClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URoomEvent> CleanRoomEventClass;
	UPROPERTY(BlueprintReadOnly)
	int32 Points = 0;
	UPROPERTY(BlueprintReadOnly)
	AObjectiveActorManager* ObjectiveActorManager = nullptr;
	UPROPERTY()
	TSet<UGameplayEvent*> GameplayEvents;

	void AddPoints(int32 AdditionalPoints);

	UPROPERTY(BlueprintAssignable)
	FOnPointsUpdated OnPointsUpdated;
	
private:
	void StartGame();
	void FinishGame();
	void StartRound();
	void FinishRound();
	void StartInterval();
	void FinishInterval();
	void StartInstant();
	void FinishInstant();

	void LaunchInstantTimer();
	void ExecuteInstantTimer();
	
	FTimerHandle InstantTimerHandle;
	FTimerDelegate InstantTimerDelegate;

	int32 RoundsStarted = 0;
	int32 RoundsFinished = 0;
	int32 IntervalsStarted = 0;
	int32 IntervalsFinished = 0;
	int32 InstantsStarted = 0;
	int32 InstantsFinished = 0;

	bool bGameLaunched = false;
	bool bRoundLaunched = false;
};
