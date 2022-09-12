// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "TurtleActor.generated.h"

UCLASS()
class TURTLE_API ATurtleActor : public AActor
{
	GENERATED_BODY()

protected:

	// Root component to hold the rest of elements together
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MoveActor")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MoveActor")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* FinishAudioCue;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* FinishAudioComponent;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* WalkAudioBase;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* WalkAudioComponent;
	
	UPROPERTY(EditAnywhere,Category = "Particle")
	UParticleSystem* UPartDestroy;
	
	UPROPERTY(EditAnywhere,Category = "Particle")
	UParticleSystemComponent* UPartDestroySystem;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence* UMaffinAnimationSequence;
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MoveActor")
	class AActor* Target;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MoveActor")
	float Speed = 45.f;
	float TmpSpeed = 0;
public:	
	// Sets default values for this actor's properties
	ATurtleActor();

	UFUNCTION()
	void SetTarget(AActor* tg);

	// Compute start distance to target 
	UFUNCTION()
	void FindTarget();

	//SetActorId
	UFUNCTION()
	void SetId(int32 id);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TimerElapsed();
	void KillActor();

protected:

	FVector Direction;
	float TotalDistance;
	float CurrentDistance;
	FVector StartLocation;

	int32 ActorId;
	bool bTurn = 0;

};
