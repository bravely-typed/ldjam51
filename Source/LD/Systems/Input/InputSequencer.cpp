#include "InputSequencer.h"

#include "GameFramework/InputSettings.h"

void UInputSequencer::Initialize()
{
	for (const auto& Entry : Sequence)
	{
		for (int32 Count = 0; Count < Entry.InputCount; ++Count)
		{
			FlattenedSequence.Add(Entry.ActionName);
		}
	}
}

void UInputSequencer::AddInput(FName ActionName)
{
	SequenceIndex++;
	if (FlattenedSequence.IsValidIndex(SequenceIndex))
	{
		if (ActionName != FlattenedSequence[SequenceIndex])
		{
			if (bFailOnIncorrectInput)
			{
				if (bResetOnFailure)
				{
					ResetInput();
				}
				else
				{
					SequenceIndex = FlattenedSequence.Num();
				}
				
				OnSequenceFail.Broadcast();
			}
		}
		else if (SequenceIndex == FlattenedSequence.Num() - 1)
		{
			OnSequenceSuccess.Broadcast();
		}
	}
}

void UInputSequencer::ResetInput()
{
	SequenceIndex = -1;
}

#if WITH_EDITOR
EDataValidationResult UInputSequencer::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = UObject::IsDataValid(ValidationErrors);

	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	const auto& ActionMappings = InputSettings->GetActionMappings();
	for (const auto& Entry : Sequence)
	{
		const FName& ActionName = Entry.ActionName; 
		const bool bMissingAction = !ActionMappings.ContainsByPredicate(
			[ActionName](const FInputActionKeyMapping& ActionKeyMapping)
			{
				return ActionName == ActionKeyMapping.ActionName;
			}
		);
		if (bMissingAction)
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Missing action %s"), *ActionName.ToString())));
		}
	}
	
	return Result;
}
#endif