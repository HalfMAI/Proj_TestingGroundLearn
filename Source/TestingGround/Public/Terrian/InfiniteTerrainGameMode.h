// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AInfiniteTerrainGameMode : public ATestingGroundGameMode
{
	GENERATED_BODY()


	UFUNCTION(BlueprintCallable, Category="MyGameMode")
	void PopulateBoundsVolumPool();


	void AddToPool(class ANavMeshBoundsVolume *VolumToAdd);
	
};
