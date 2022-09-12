// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurtlePawn.generated.h"

UCLASS(config=Game)
class ATurtlePawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:
	void OnResetVR();
	void TriggerClick();
	void ExitButtonClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ATurtleBlock* CurrentBlockFocus;
};
