// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"


class UActorPool;


/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AInfiniteTerrainGameMode : public ATestingGroundGameMode
{
	GENERATED_BODY()
		
public:

	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category="Bounds Pool")
	void PopulateBoundsVolumPool();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pool")
	UActorPool* NavMeshBoundsVolumePool;


private:
	void AddToPool(class ANavMeshBoundsVolume *VolumToAdd);
	
};
