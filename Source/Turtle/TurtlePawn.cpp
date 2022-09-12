// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurtlePawn.h"
#include "TurtleBlock.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ATurtlePawn::ATurtlePawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ATurtlePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UCameraComponent* OurCamera = PC->GetViewTarget()->FindComponentByClass<UCameraComponent>())
			{
				FVector Start = OurCamera->GetComponentLocation();
				FVector End = Start + (OurCamera->GetComponentRotation().Vector() * 8000.0f);
				TraceForBlock(Start, End, true);
			}
		}
		else
		{
			FVector Start, Dir, End;
			PC->DeprojectMousePositionToWorld(Start, Dir);
			End = Start + (Dir * 8000.0f);
			TraceForBlock(Start, End, false);
		}
	}
}

void ATurtlePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", EInputEvent::IE_Pressed, this, &ATurtlePawn::OnResetVR);
	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &ATurtlePawn::TriggerClick);
	PlayerInputComponent->BindAction("Quit", IE_Pressed, this, &ATurtlePawn::ExitButtonClick);
}

void ATurtlePawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void ATurtlePawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATurtlePawn::TriggerClick()
{
	if (CurrentBlockFocus)
	{
		CurrentBlockFocus->HandleClicked();
	}
}

void ATurtlePawn::ExitButtonClick()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void ATurtlePawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		ATurtleBlock* HitBlock = Cast<ATurtleBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}
}
