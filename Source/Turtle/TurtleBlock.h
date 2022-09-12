// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "TurtleBlock.generated.h"

class ATurtleActor;
class ATargetActor;
/** A block that can be clicked */
UCLASS(minimalapi)
class ATurtleBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	/** Text component for the score */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* NumText;

	/** StaticMesh component for spawning */
	UPROPERTY(Category = "Basket", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BasketMesh;

	UPROPERTY(EditAnywhere,Category = "Basket")
	FVector BasketRelativeLocation = FVector(350,50,50);
	
	UPROPERTY(EditAnywhere,Category = "Basket")
	FRotator BasketRelativeRotator;

	UPROPERTY(EditAnywhere,Category = "Target")
	FVector TargetRelativeLocation = FVector(500,-50,0);
	
	UPROPERTY(EditAnywhere,Category = "Target")
	FRotator TargetRelativeRotator;

	UPROPERTY(EditAnywhere,Category = "Color")
	float TimerDelay = 30.f;


	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* ClickAudioCue;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* ClickAudioComponent;

	
public:	
	/** Target actor*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Target")
	class ATargetActor* Target;

	UPROPERTY(EditAnywhere, Category = "Type")
	int32 BasketNum = 0;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	// End AActor interface

	
public:
	ATurtleBlock();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;

	/** Grid that owns us */
	UPROPERTY()
	class ATurtleBlockGrid* OwningGrid;

	/** Actor to spawn */
	UPROPERTY(EditDefaultsOnly, Category= "Spawning")
	TSubclassOf<ATurtleActor> ActorTurtleToSpawn;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();

	void Highlight(bool bOn);

	void TimerElapsed();

	UFUNCTION()
	void SetNumText();
	

protected:
	UFUNCTION()
	void SpawnObject(FVector location, FRotator rotation);

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};



