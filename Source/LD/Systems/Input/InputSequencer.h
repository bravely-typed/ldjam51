#pragma once

#include "InputSequencer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputSequencerEvent);

USTRUCT(BlueprintType)
struct FInputSequencerEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName ActionName;
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int32 InputCount = 1;
};

UCLASS(Blueprintable, BlueprintType)
class UInputSequencer : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void AddInput(FName ActionName);
	UFUNCTION(BlueprintCallable)
	void ResetInput();

	const TArray<FInputSequencerEntry>& GetSequence() const { return Sequence; }
	
	UPROPERTY(BlueprintAssignable)
	FInputSequencerEvent OnSequenceSuccess;
	UPROPERTY(BlueprintAssignable)
	FInputSequencerEvent OnSequenceFail;
	
protected:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

	// Will only trigger OnSequenceFail if this is enabled
	UPROPERTY(EditDefaultsOnly)
	bool bFailOnIncorrectInput = true;
	UPROPERTY(EditDefaultsOnly)
	bool bResetOnFailure = true;
	UPROPERTY(EditDefaultsOnly)
	TArray<FInputSequencerEntry> Sequence;
	
	TArray<FName> FlattenedSequence;
	int32 SequenceIndex = -1;
};
