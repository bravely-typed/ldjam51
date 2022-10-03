#pragma once

#include "Object.h"

#include "GameplayEvent.generated.h"

class UGameplayEvent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameplayEventHook, UGameplayEvent*, GameplayEvent);

UCLASS(Blueprintable, BlueprintType)
class UGameplayEvent : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void Start();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Complete();

	UPROPERTY(BlueprintAssignable)
	FGameplayEventHook OnStarted;
	UPROPERTY(BlueprintAssignable)
	FGameplayEventHook OnCompleted;
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class ALDGameMode* GetGameMode() const;
};
