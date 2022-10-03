#include "ProgressionSubsystem.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/LDGameMode.h"
#include "LDLog.h"

void UProgressionSubsystem::ExecuteProgressionSequence(float DeltaSeconds)
{
	if (!bProgressionSequenceLaunched)
	{
		// Not an error, we're just not executing right now
		// Very verbose
		//LOG_LD(Log, TEXT("Progression not currently happening"));
		return;
	}

	// Very verbose
	//LOG_LD(Log, TEXT("Executing progression sequence"));

	const FVector CheckInLocation = CheckInDesk->GetActorLocation();
	const FVector PlayerLocation = Player->GetActorLocation();

	const auto DeltaX = PlayerLocation.X - CheckInLocation.X;
	if (DeltaX < -20.0f)
	{
		Player->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), 1.0f);
		return;
	}

	if (DeltaX > 20.0f)
	{
		Player->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), -1.0f);
		return;
	}

	FinishProgressionSequence();
}

void UProgressionSubsystem::LaunchProgressionSequence()
{
	if (bProgressionSequenceLaunched)
	{
		LOG_LD(Warning, TEXT("Already launched progression, not launching them it again"));
		return;
	}

	StartProgressionSequence();
}

void UProgressionSubsystem::StartProgressionSequence()
{
	check(!bProgressionSequenceLaunched);
	check(Player == nullptr);

	bProgressionSequenceLaunched = true;

	LOG_LD(Log, TEXT("Started progression"));
	OnProgressionSequenceStarted.Broadcast();

	if (bFirstTime)
	{
		// Don't take over when the game boots - just let the player run around
		FinishProgressionSequence();
		bFirstTime = false;
		return;
	}

	const auto* World = GetWorld();
	check(World != nullptr);

	auto *PlayerActor = UGameplayStatics::GetActorOfClass(World, ALDCharacter::StaticClass());
	check(PlayerActor != nullptr);

	Player = dynamic_cast<ALDCharacter*>(PlayerActor);
	check(Player != nullptr);

	auto* CheckInDeskActor = UGameplayStatics::GetActorOfClass(World, ACheckInActor::StaticClass());
	check(CheckInDeskActor != nullptr);

	CheckInDesk = dynamic_cast<ACheckInActor*>(CheckInDeskActor);
	check(CheckInDesk != nullptr);

	auto* PlayerController = dynamic_cast<APlayerController*>(Player->Controller);
	check(PlayerController != nullptr);

	Player->DisableInput(PlayerController);
}

void UProgressionSubsystem::FinishProgressionSequence()
{
	if (bFirstTime)
	{
		LOG_LD(Log, TEXT("Finished progression"));
		bProgressionSequenceLaunched = false;
		OnProgressionSequenceFinished.Broadcast();
		return;
	}

	check(bProgressionSequenceLaunched);
	check(Player != nullptr);

	auto* PlayerController = dynamic_cast<APlayerController*>(Player->Controller);
	check(PlayerController != nullptr);

	Player->EnableInput(PlayerController);

	bProgressionSequenceLaunched = false;
	Player = nullptr;

	LOG_LD(Log, TEXT("Finished progression"));
	OnProgressionSequenceFinished.Broadcast();
}