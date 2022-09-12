// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleActor.h"

#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATurtleActor::ATurtleActor()
{

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<USkeletalMesh> BaseMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinder<USoundCue> FinishSoundCue;
		ConstructorHelpers::FObjectFinder<USoundBase> WalkSoundBase;
		ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleDestroyEffect;
		ConstructorHelpers::FObjectFinder<UAnimSequence> MuffinIdleAnimation;
		FConstructorStatics()
			: BaseMesh(TEXT("/Game/Puzzle/Muffin/SK_Muffin.SK_Muffin"))
			, BaseMaterial(TEXT("/Game/Puzzle/Muffin/Materials/M_Muffin.M_Muffin"))
			, FinishSoundCue(TEXT("/Game/Puzzle/Audio/Special_Powerup_02_wav_Cue.Special_Powerup_02_wav_Cue"))
			, WalkSoundBase(TEXT("/Game/Puzzle/Audio/mixkit-footsteps-on-hay-loop-541.mixkit-footsteps-on-hay-loop-541"))
			, ParticleDestroyEffect(TEXT("/Game/Puzzle/Effects/P_Status_Stun.P_Status_Stun"))
			, MuffinIdleAnimation(TEXT("AnimSequence'/Game/Puzzle/Muffin/SK_Muffin_Walk.SK_Muffin_Walk'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Create Static Mesh
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetSkeletalMesh(ConstructorStatics.BaseMesh.Get());
	Mesh->SetRelativeScale3D(FVector(1.f,1.f,1.f));
	Mesh->SetRelativeLocation(FVector(0.f,0.f,0.f));
	Mesh->SetRelativeRotation(FRotator(0.f,-90.f,0.f));
	Mesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	Mesh->SetupAttachment(RootComponent);
	//add animation
	UMaffinAnimationSequence = ConstructorStatics.MuffinIdleAnimation.Object;
	Mesh->SetAnimation(UMaffinAnimationSequence);
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	
	
	
	// Sound effect setup
	FinishAudioCue = ConstructorStatics.FinishSoundCue.Object;
	FinishAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FinishAudioComp"));
	FinishAudioComponent->bAutoActivate = false; // don't play the sound immediately.
	FinishAudioComponent->SetupAttachment(RootComponent);
	FinishAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	WalkAudioBase = ConstructorStatics.WalkSoundBase.Object;
	WalkAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WalkAudioComp"));
	WalkAudioComponent->bAutoActivate = false; // don't play the sound immediately.
	WalkAudioComponent->SetupAttachment(RootComponent);
	WalkAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	WalkAudioComponent->SetVolumeMultiplier(0.2f);
	// Particles
	UPartDestroy = ConstructorStatics.ParticleDestroyEffect.Object;
	UPartDestroySystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DestroyParticalComp"));
	UPartDestroySystem->bAutoActivate = false; // don't play the effect immediately.
	UPartDestroySystem->SetupAttachment(RootComponent);
	UPartDestroySystem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	
	TmpSpeed = Speed;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurtleActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ATurtleActor::TimerElapsed, 1, false);

	
	Mesh->PlayAnimation(UMaffinAnimationSequence,true);
	
}

void ATurtleActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set sound  effect
	if (FinishAudioCue->IsValidLowLevelFast())
	{
		FinishAudioComponent->SetSound(FinishAudioCue);
	}

	if (WalkAudioBase->IsValidLowLevelFast())
	{
		WalkAudioComponent->SetSound(WalkAudioBase);
	}

	// Set particles effect
	if (UPartDestroy->IsValidLowLevelFast())
	{
		UPartDestroySystem->SetTemplate(UPartDestroy);
	}
}

// Setter for target pointer
void ATurtleActor::SetTarget(AActor* tg)
{
	if(tg != nullptr)
	{
		Target = tg;
	}
}

void ATurtleActor::FindTarget()
{
	if (Target != nullptr)
	{
		StartLocation = GetActorLocation();

		Direction = Target->GetActorLocation() - StartLocation;

		TotalDistance = Direction.Size();
		
		// Get Normal Direction
		Direction = Direction.GetSafeNormal();
		CurrentDistance = 0.0f;
	}
}

void ATurtleActor::SetId(int32 id)
{
	ActorId = id;
}


// Called every frame
void ATurtleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Target != nullptr)
	{

		 if (CurrentDistance < TotalDistance)
		{
			FVector Location = GetActorLocation();

			Location += Direction * Speed * DeltaTime;

			SetActorLocation(Location);
		 	
			CurrentDistance = (Location - StartLocation).Size();
		 	SetActorRelativeRotation(FRotator((Location - StartLocation).Rotation().Pitch,(Location - StartLocation).Rotation().Yaw,0));

		 	// Play walk sound
		 	WalkAudioComponent->SetActive(true);
		 	
		}
		else
		{
			// Disable walk sound and enable finish sound
			WalkAudioComponent->SetActive(false);
			FinishAudioComponent->SetVolumeMultiplier(2);
			FinishAudioComponent->SetActive(true);
			
			// Enable destroy effect
			UPartDestroySystem->SetActive(true);
			
			// Delay to play sound effect or particle effect
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ATurtleActor::KillActor, 1.2f, false);
			
		}

	}
}

void ATurtleActor::TimerElapsed()
{
	bTurn = !bTurn;
	
	if(ActorId%2==0)
	{
		Speed ? Speed = 0 : Speed = TmpSpeed;
	}
	else if(ActorId%3 == 0)
	{
			Speed = -Speed;
		
	}
	FTimerHandle UnusedHandle;
	if(!bTurn)
	{
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ATurtleActor::TimerElapsed, 1.f, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ATurtleActor::TimerElapsed, 0.5f, false);	
	}


}

void ATurtleActor::KillActor()
{
	FinishAudioComponent->SetActive(false);
	UPartDestroySystem->SetActive(false);
	
	Destroy();
}
