// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassHISMC.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=("CustomClass"), meta=(BlueprintSpawnableComponent))
class TESTINGGROUND_API UGrassHISMC : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()

	
public:
	// Sets default values for this actor's properties
	UGrassHISMC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "SpawnSetting")
	int SpawnCount;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnSetting")
	FBox SpawningExtents;
	
private:
	void SpawnGrass();
};
