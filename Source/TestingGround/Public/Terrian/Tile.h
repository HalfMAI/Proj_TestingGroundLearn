// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


class UBoxComponent;


#define ECC_GameTraceChannel_SpawnProps ECollisionChannel::ECC_GameTraceChannel2


UCLASS()
class TESTINGGROUND_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SpawnProps(TArray< TSubclassOf<AActor> > SpawnActors, int MinSpawn, int MaxSpawn, float ScaleMin=1.f, float ScaleMax=1.f);


	UFUNCTION(BlueprintCallable, Category = "Tile")
	bool CastSphere(FVector Location, float Radius);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	UBoxComponent* SpawnBox;


	void PlaceActor(TSubclassOf<AActor> &ActorClass, FBox &SpawnBod, FVector2DHalf ScaleRange, int RetryTimes = 10);
};
