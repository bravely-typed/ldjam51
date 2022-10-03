#pragma once

#include <CoreMinimal.h>
#include <GameInstanceSubsystem.h>

#include "GameFramework/LDCharacter.h"
#include "Systems/Objectives/CheckInActor.h"

#include "ProgressionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLDProgressionEvent);

UCLASS(Blueprintable)
class UProgressionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// This is basically a Tick(), but since subsystems don't have Tick(), we call this from the GameMode
	void ExecuteProgressionSequence(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void LaunchProgressionSequence();

	UPROPERTY(BlueprintAssignable)
	FLDProgressionEvent OnProgressionSequenceStarted;
	UPROPERTY(BlueprintAssignable)
	FLDProgressionEvent OnProgressionSequenceFinished;

private:
	void StartProgressionSequence();
	void FinishProgressionSequence();

	ALDCharacter* Player = nullptr;
	ACheckInActor* CheckInDesk = nullptr;

	bool bProgressionSequenceLaunched = false;
	bool bFirstTime = true;
};