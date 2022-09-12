// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurtleBlockGrid.h"
#include "TurtleBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"



ATurtleBlockGrid::ATurtleBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;



	// Set defaults
	Size = 3;
	BlockSpacing = 300.f;
}


void ATurtleBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size;

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		ATurtleBlock* NewBlock = GetWorld()->SpawnActor<ATurtleBlock>(BlockLocation, FRotator(0,0,0));

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
			NewBlock->BasketNum = BlockIndex+1;
			NewBlock->SetNumText();
		}
	}
}


