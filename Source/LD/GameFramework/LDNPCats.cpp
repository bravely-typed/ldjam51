#include "LDNPCats.h"

#include "LDGameMode.h"
#include "PaperFlipbookComponent.h"

ALDNPCats::ALDNPCats()
{
	PrimaryActorTick.bCanEverTick = true;

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Root Flipbook Comp"));
	Flipbook->SetHiddenInGame(false, true);
	SetRootComponent(Flipbook);
}

void ALDNPCats::SetCurrentService(UGameplayEvent* ServiceRequest)
{
	CurrentServiceRequest = ServiceRequest;
	LastServiceRequest = nullptr;
	CurrentServiceRequest->OnCompleted.AddDynamic(this, &ALDNPCats::OnServiceRequestCompleted);
}

bool ALDNPCats::CanRequestService() const
{
	return !IsWaitingForService();
}

bool ALDNPCats::IsWaitingForService() const
{
	return IsValid(CurrentServiceRequest);
}

void ALDNPCats::CheckOut_Implementation()
{
	// API stub
}

void ALDNPCats::FollowSpline_Implementation(bool bForward, USplineComponent* Spline)
{
	bMoving = false;
}

void ALDNPCats::MoveToLocation(FVector Location, float Duration)
{
	MoveStartLocation = GetActorLocation();
	MoveEndLocation = Location;
	MoveDuration = Duration;
	ElapsedMoveTime = 0.f;
	bMoving = true;
}

void ALDNPCats::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetAuthGameMode<ALDGameMode>()->OnIntervalStarted.AddDynamic(this, &ALDNPCats::UpdateSatisfaction);
}

void ALDNPCats::OnServiceRequestCompleted(UGameplayEvent* ServiceRequest)
{
	ServiceRequest->OnCompleted.RemoveAll(this);
	CurrentServiceRequest = nullptr;
	LastServiceRequest = nullptr;
}

void ALDNPCats::UpdateSatisfaction()
{
	if (LastServiceRequest == CurrentServiceRequest && IsValid(CurrentServiceRequest))
	{
		if (Satisfaction == 0)
		{
			return;
		}
		
		Satisfaction--;
		OnSatisfactionUpdated.Broadcast(this, Satisfaction);
	}
	LastServiceRequest = CurrentServiceRequest;
}

void ALDNPCats::UpdateAnimation(ECatAnimationState DesiredState)
{
	//pass in desired animation state
	CurrentAnimationState = DesiredState;
	//compare with current state
	//if same, do nothing
	//if different, change current to desired
}

void ALDNPCats::CheckAnimationState()
{

}

void ALDNPCats::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoving)
	{
		ElapsedMoveTime += DeltaTime;
		const float Alpha = ElapsedMoveTime / MoveDuration;
		FVector Location;
		if (Alpha >= 1.f)
		{
			bMoving = false;
			Location = MoveEndLocation;
		}
		else
		{
			Location = MoveStartLocation + (MoveEndLocation - MoveStartLocation) * Alpha;
		}
		SetActorLocation(Location);
	}
}