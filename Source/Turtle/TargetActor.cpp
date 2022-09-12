// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor.h"

// Sets default values
ATargetActor::ATargetActor()
{

	
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/Flag/Mesh/Flag.Flag"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/Flag/Mesh/MFlag.MFlag"))
			
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh0"));
	Mesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	Mesh->SetRelativeScale3D(FVector(6.f,6.f,6.f));
	Mesh->SetRelativeLocation(FVector(0.f,0.f,0.f));
	Mesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	Mesh->SetupAttachment(DummyRoot);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATargetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

