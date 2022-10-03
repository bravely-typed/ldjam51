// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "LDCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;

UCLASS()
class ALDCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	ALDCharacter();
	
	FORCEINLINE UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UInventoryComponent* GetInventory() const { return InventoryComponent; }
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	virtual void Tick(float DeltaSeconds) override;

	void MoveRight(float Value);
	
	void UpdateCharacter();
	void UpdateAnimation();

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* RunningAnimation = nullptr;
	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* IdleAnimation = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* SideViewCameraComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UInventoryComponent* InventoryComponent = nullptr;
};
