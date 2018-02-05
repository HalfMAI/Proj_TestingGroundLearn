// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"

#include "EngineUtils.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"

void AInfiniteTerrainGameMode::PopulateBoundsVolumPool()
{
	auto tmpActorIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (tmpActorIterator)
	{
		this->AddToPool(*tmpActorIterator);
		++tmpActorIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume *VolumToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *VolumToAdd->GetName());
}
