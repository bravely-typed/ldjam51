// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LDGameMode.h"

#include "LDCharacter.h"
#include "LDLog.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Systems/LDRooms.h"
#include "Systems/Progression/ProgressionSubsystem.h"

ALDGameMode::ALDGameMode()
{
	DefaultPawnClass = ALDCharacter::StaticClass();
	InstantTimerDelegate.BindUObject(this, &ALDGameMode::ExecuteInstantTimer);
	PrimaryActorTick.bCanEverTick = true;
}

void ALDGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto* Game = GetGameInstance();
	check(Game != nullptr);

	auto* Progression = Game->GetSubsystem<UProgressionSubsystem>();
	check(Progression != nullptr);

	Progression->ExecuteProgressionSequence(DeltaSeconds);
}

void ALDGameMode::LaunchGame()
{
	if (bGameLaunched)
	{
		LOG_LD(Warning, TEXT("Game already launched, not launching it again"));
		return;
	}

	StartGame();
}

void ALDGameMode::LaunchRound()
{
	if (!bGameLaunched)
	{
		LOG_LD(Warning, TEXT("Game must be launched before a round can be launched"));
		return;
	}

	if (bRoundLaunched)
	{
		LOG_LD(Warning, TEXT("Round already launched, not launching it again"));
		return;
	}

	LOG_LD(Log, TEXT("RoundsStarted: %d, RoundsFinished: %d, RoundsPerGame: %d"), RoundsStarted, RoundsFinished, RoundsPerGame);

	if (RoundsStarted == RoundsFinished && RoundsFinished == RoundsPerGame)
	{
		LOG_LD(Warning, TEXT("All rounds finished, not launching a new one"));
		return;
	}

	StartRound();
}

void ALDGameMode::GenerateEvent()
{
	bool bRoomServiceAlreadyRequested = false;
	int32 VacantRooms = 0;
	TArray<int32> AvailableRooms;
	for (int32 RoomIndex = 0; RoomIndex < ObjectiveActorManager->Rooms.Num(); ++RoomIndex)
	{
		const auto& Room = ObjectiveActorManager->Rooms[RoomIndex];
		if (Room->IsVacant())
		{
			VacantRooms++;
		}
		else if (Room->GetGuest()->CanRequestService())
		{
			AvailableRooms.Add(RoomIndex);
		}
		else
		{
			if (Room->GetGuest()->GetCurrentServiceRequest()->IsA(RoomServiceEventClass))
			{
				bRoomServiceAlreadyRequested = true;
			}
		}
	}

	TArray<TSubclassOf<UGameplayEvent>> EventClasses;
	if (VacantRooms > ObjectiveActorManager->CheckIn->GetQueueCount())
	{
		EventClasses.Add(CheckInEventClass);
	}

	if (!bRoomServiceAlreadyRequested)
	{
		// EventClasses.Add(RoomServiceEventClass);
	}

	if (AvailableRooms.Num() > 0)
	{
		EventClasses.Add(CleanRoomEventClass);
		// EventClasses.Add(SquashBugsEventClass);
	}
	
	if (EventClasses.Num() < 1)
	{
		LOG_LD(Error, TEXT("Needs at minimum one gameplay event available at all times."));
		return;
	}
	
	const int32 EventIndex = FMath::RandRange(0, EventClasses.Num() - 1);
	const TSubclassOf<UGameplayEvent> EventClass = EventClasses[EventIndex];
	UGameplayEvent* Event = NewObject<UGameplayEvent>(this, EventClass);
	URoomEvent* RoomEvent = Cast<URoomEvent>(Event);
	if (IsValid(RoomEvent))
	{
		const int32 RoomIndex = FMath::RandRange(0, AvailableRooms.Num() - 1);
		RoomEvent->SetRoom(ObjectiveActorManager->Rooms[AvailableRooms[RoomIndex]]);
	}
	Event->OnCompleted.AddDynamic(this, &ALDGameMode::RemoveEvent);
	GameplayEvents.Add(Event);
	LOG_LD(Log, TEXT("Created event %s"), *Event->GetClass()->GetName());
	Event->Start();
}

void ALDGameMode::RemoveEvent(UGameplayEvent* Event)
{
	Event->OnCompleted.RemoveAll(this);
	GameplayEvents.Remove(Event);
}

void ALDGameMode::AddPoints(int32 AdditionalPoints)
{
	Points += AdditionalPoints;
	OnPointsUpdated.Broadcast(Points);
}

void ALDGameMode::StartGame()
{
	RoundsStarted = 0;
	RoundsFinished = 0;
	bGameLaunched = true;

	LOG_LD(Log, TEXT("Game started!"));
	OnGameStarted.Broadcast();
}

void ALDGameMode::FinishGame()
{
	LOG_LD(Log, TEXT("RoundsStarted: %d, RoundsFinished: %d, RoundsPerGame: %d"), RoundsStarted, RoundsFinished, RoundsPerGame);

	check(RoundsStarted == RoundsFinished);
	check(RoundsFinished == RoundsPerGame);

	bGameLaunched = false;

	LOG_LD(Log, TEXT("Game finished!"));
	OnGameFinished.Broadcast();
}

void ALDGameMode::StartRound()
{
	IntervalsStarted = 0;
	IntervalsFinished = 0;
	bRoundLaunched = true;

	LOG_LD(Log, TEXT("Round %d started!"), RoundsStarted + 1);
	++RoundsStarted;
	OnRoundStarted.Broadcast();

	StartInterval();
}

void ALDGameMode::FinishRound()
{
	check(IntervalsStarted == IntervalsFinished);
	check(IntervalsFinished == IntervalsPerRound);

	bRoundLaunched = false;

	LOG_LD(Log, TEXT("Round %d finished!"), RoundsFinished + 1);
	++RoundsFinished;
	OnRoundFinished.Broadcast();

	if (RoundsFinished == RoundsPerGame)
	{
		FinishGame();
		return;
	}
}

void ALDGameMode::StartInterval()
{
	InstantsStarted = 0;
	InstantsFinished = 0;

	LOG_LD(Log, TEXT("Interval %d started!"), IntervalsFinished + 1);
	++IntervalsStarted;
	OnIntervalStarted.Broadcast();

	StartInstant();
}

void ALDGameMode::FinishInterval()
{
	check(InstantsStarted == InstantsFinished);
	check(InstantsFinished == InstantsPerInterval);

	LOG_LD(Log, TEXT("Interval %d finished!"), IntervalsFinished + 1);
	++IntervalsFinished;
	OnIntervalFinished.Broadcast();
	
	GenerateEvent();

	if (IntervalsFinished == IntervalsPerRound)
	{
		FinishRound();
		return;
	}

	StartInterval();
}

void ALDGameMode::StartInstant()
{
	// Very verbose
	//LOG_LD(Log, TEXT("Instant %d started!"), InstantsFinished + 1);
	++InstantsStarted;
	OnInstantStarted.Broadcast();

	LaunchInstantTimer();
}

void ALDGameMode::FinishInstant()
{
	// Very verbose
	//LOG_LD(Log, TEXT("Instant %d finished!"), InstantsFinished + 1);
	++InstantsFinished;
	OnInstantFinished.Broadcast();

	if (InstantsFinished == InstantsPerInterval)
	{
		FinishInterval();
		return;
	}

	StartInstant();
}

void ALDGameMode::LaunchInstantTimer()
{
	// We tick one time per second, because some things need sub-interval information
	static constexpr float OneSecond = 1.0f;
	static constexpr bool bLoopTimer = false;

	const auto* World = GetWorld();
	check(World != nullptr);

	auto& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(InstantTimerHandle, InstantTimerDelegate, OneSecond, bLoopTimer);
}

void ALDGameMode::ExecuteInstantTimer()
{
	// We start the first instant in LaunchInstantTimer, so this function ends the instant at the
	// beginning and starts the new instant at the end
	FinishInstant();
}