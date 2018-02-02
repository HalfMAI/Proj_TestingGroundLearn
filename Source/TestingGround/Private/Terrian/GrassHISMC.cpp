// Fill out your copyright notice in the Description page of Project Settings.

#include "GrassHISMC.h"


UGrassHISMC::UGrassHISMC()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGrassHISMC::BeginPlay()
{
	Super::BeginPlay();

	this->SpawnGrass();
}

void UGrassHISMC::SpawnGrass()
{
	for (int i = 0; i < this->SpawnCount; i++)
	{
		FVector tmpLocation = FMath::RandPointInBox(this->SpawningExtents);
		this->AddInstance(FTransform(tmpLocation));
	}
}

void UGrassHISMC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}