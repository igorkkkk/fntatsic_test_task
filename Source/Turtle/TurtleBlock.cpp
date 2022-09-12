// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurtleBlock.h"

#include <string>

#include "TargetActor.h"
#include "TurtleBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "TurtleActor.h"
#include "Components/AudioComponent.h"
#include "Math/UnrealMathUtility.h"




ATurtleBlock::ATurtleBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BasketMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BasketMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinder<USoundCue> ClickSoundCue;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/Button/Mesh/Play.Play"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BasketMesh(TEXT("/Game/Puzzle/Meshes/Basket/Mesh/Basket.Basket"))
			, BasketMaterial(TEXT("/Game/Puzzle/Meshes/Basket/Mesh/MBasket.MBasket"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("MaterialInstanceConstant'/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial'"))
			, ClickSoundCue(TEXT("/Game/Puzzle/Audio/Click_11_wav_Cue.Click_11_wav_Cue"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ATurtleBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ATurtleBlock::OnFingerPressedBlock);

	// Create Basket
	BasketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh1"));
	BasketMesh->SetStaticMesh(ConstructorStatics.BasketMesh.Get());
	BasketMesh->SetRelativeScale3D(FVector(1.0f,1.0f,1.0f));
	BasketMesh->SetRelativeLocation(BasketRelativeLocation+FVector(FMath::RandRange(-200,200),FMath::RandRange(-200,200),0));
	BasketMesh->SetRelativeRotation(BasketRelativeRotator);
	BasketMesh->SetMaterial(0, ConstructorStatics.BasketMaterial.Get());
	BasketMesh->SetupAttachment(DummyRoot);

	// Create text component
	NumText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NumText0"));
	NumText->SetRelativeLocation(FVector(0.f,0.f,150.f)); 
	NumText->SetRelativeRotation(FRotator(90.f,180.f,0.f));
	NumText->SetTextRenderColor(FColor::Purple);
	NumText->SetXScale(5.f);
	NumText->SetYScale(5.f);
	NumText->SetupAttachment(DummyRoot);

	//sound effect setup
	ClickAudioCue = ConstructorStatics.ClickSoundCue.Object;
	ClickAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ClickAudioComp"));
	ClickAudioComponent->bAutoActivate = false; // don't play the sound immediately.
	ClickAudioComponent->SetupAttachment(RootComponent);
	ClickAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	
	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	

}


void ATurtleBlock::BeginPlay()
{
	Super::BeginPlay();
	// Spawn a target
	
	ATargetActor* NewTarget = GetWorld()->SpawnActor<ATargetActor>(GetActorLocation()+FVector(FMath::RandRange(-300,300),FMath::RandRange(-300,300),0.f), TargetRelativeRotator);

	//Tell the block about its owner
	if (NewTarget != nullptr)
	{
		Target = NewTarget;
		Target->SetActorScale3D(FVector(20.f,20.f,20.f));
	}

	
}

void ATurtleBlock::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//set sound  effect
	if (ClickAudioCue->IsValidLowLevelFast())
	{
		ClickAudioComponent->SetSound(ClickAudioCue);
	}
}

void ATurtleBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void ATurtleBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void ATurtleBlock::HandleClicked()
{
	//play sound when click
	ClickAudioComponent->Play();
	
	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, OrangeMaterial);
		SpawnObject(BasketMesh->GetComponentLocation(),BasketRelativeRotator);
		
	}
	else
	{
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ATurtleBlock::TimerElapsed, TimerDelay, false);
		bIsActive = false;

		// Change material
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void ATurtleBlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void ATurtleBlock::SpawnObject(FVector location, FRotator rotation)
{
	FActorSpawnParameters SpawnParams;
	ATurtleActor* SpawnActorRef = GetWorld()->SpawnActor<ATurtleActor>(location,rotation/*,SpawnParams*/);
	
	if(Target != nullptr)
	{
		SpawnActorRef->SetTarget(Target);
		SpawnActorRef->FindTarget();
		SpawnActorRef->SetId(BasketNum);
	}
}

void ATurtleBlock::TimerElapsed(){}

void ATurtleBlock::SetNumText()
{
	NumText->SetText(FString::FromInt(BasketNum));
}
