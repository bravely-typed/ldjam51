#include "CheckInActor.h"

#include "Components/SplineComponent.h"
#include "Engine/World.h"
#include "GameFramework/LDGameMode.h"
#include "Systems/LDRooms.h"

void ACheckInActor::HandleAction_Implementation(FName ActionName)
{
	Super::HandleAction_Implementation(ActionName);

	if (ActionName == CheckInActionName
		&& CheckInQueue.Num() > 0)
	{
		CheckIn();
	}
}

void ACheckInActor::CheckIn()
{
	UCheckInEvent* CheckInEvent = CheckInQueue[0];
	CheckInQueue.RemoveAt(0);
	OnQueueUpdate.Broadcast();
	for (const auto& Room : GetWorld()->GetAuthGameMode<ALDGameMode>()->ObjectiveActorManager->Rooms)
	{
		if (Room->IsVacant())
		{
			ALDNPCats* Guest = CheckInEvent->GetGuest(); 
			Room->SetGuest(Guest);
			Guest->OnSatisfactionUpdated.RemoveAll(this);
			bool bNeedsTeleport = true;
			if (IsValid(Room->GetSplineActor()))
			{
				USplineComponent* Spline = Room->GetSplineActor()->FindComponentByClass<USplineComponent>();
				if (IsValid(Spline))
				{
					Guest->FollowSpline(true, Spline);
					bNeedsTeleport = false;
				}
			}

			// Failsafe in case a spline wasn't hooked up properly
			if (bNeedsTeleport)
			{
				Guest->SetActorLocation(Room->GetActorLocation());
			}
			
			CheckInEvent->Complete();
			
			if (CheckInQueue.Num() < 1)
			{
				OnNotInteractable();
			}
			else
			{
				for (int32 CheckInIndex = 0; CheckInIndex < CheckInQueue.Num(); ++CheckInIndex)
				{
					MoveGuestToQueueLocation(CheckInIndex, AdvanceInQueueMoveDuration);
				}
			}
			return;
		}
	}

	checkNoEntry();
}

void ACheckInActor::AddToCheckInQueue(UCheckInEvent* CheckInEvent)
{
	if (CheckInQueue.Num() < 1)
	{
		OnIsInteractable();
	}

	CheckInQueue.Add(CheckInEvent);

	CheckInEvent->GetGuest()->OnSatisfactionUpdated.AddDynamic(this, &ACheckInActor::OnSatisfactionChanged);
	MoveGuestToQueueLocation(CheckInQueue.Num() - 1, AddToQueueMoveDuration);
}

void ACheckInActor::RemoveFromCheckInQueue(UCheckInEvent* CheckInEvent)
{
	CheckInQueue.Remove(CheckInEvent);
	if (CheckInQueue.Num() < 1)
	{
		OnNotInteractable();
	}

	CheckInEvent->GetGuest()->OnSatisfactionUpdated.RemoveAll(this);
	// TODO: Move cat off screen
	CheckInEvent->GetGuest()->Destroy();
}

void ACheckInActor::MoveGuestToQueueLocation(int32 Index, float Duration)
{
	if (!CheckInQueue.IsValidIndex(Index))
	{
		return;
	}

	FVector Location;
	if (QueuePositions.IsValidIndex(Index))
	{
		Location = QueuePositions[Index]->GetActorLocation();
	}
	else
	{
		Location = GetActorLocation();
	}
	CheckInQueue[Index]->GetGuest()->MoveToLocation(Location, Duration);
}

void ACheckInActor::OnSatisfactionChanged(ALDNPCats* Guest, int32 Satisfaction)
{
	if (Satisfaction == 0)
	{
		for (const auto& CheckInEvent : CheckInQueue)
		{
			if (CheckInEvent->GetGuest() == Guest)
			{
				RemoveFromCheckInQueue(CheckInEvent);
				break;
			}
		}
	}
}

void ACheckInActor::OnNotInteractable_Implementation()
{
}

void ACheckInActor::OnIsInteractable_Implementation()
{
}
