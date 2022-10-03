#pragma once

#include "GameFramework/Actor.h"

#include "ObjectiveActor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:
	AObjectiveActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleAction(FName ActionName);
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBoxComponent* Bounds = nullptr;
};
