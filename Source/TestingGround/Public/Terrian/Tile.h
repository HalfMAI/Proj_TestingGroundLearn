// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


class UBoxComponent;
class UActorPool;


#define ECC_GameTraceChannel_SpawnProps ECollisionChannel::ECC_GameTraceChannel2


USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

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
	void SpawnAIPawns(TArray< TSubclassOf<APawn> > SpawnPawns, int MinSpawn, int MaxSpawn);

	UFUNCTION(BlueprintCallable, Category = "Tile")
	bool CastSphere(FVector Location, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetupPool(UActorPool* InPool);

private:
	UActorPool * ActorPool;
	AActor * TitleNavMeshBoundsVolume;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	UBoxComponent* SpawnBox;
	
private:
	FBox _GetSpawnBox();

	template<typename T>
	using SpawnFuncType = AActor* (ATile::*) (T, FSpawnPosition);

	AActor* _SpawnProps(TSubclassOf<AActor> SpawnPropClass, FSpawnPosition SpawnPosition);
	AActor* _SpawnAI(TSubclassOf<APawn> SpawnAIPawnClass, FSpawnPosition SpawnPosition);

	void PositionNavMeshBoundsVolume();

	template<class T>
	void PlaceActor(TArray<T> SpawnActors, SpawnFuncType<T> SpawnFunc, int MinSpawn, int MaxSpawn, FVector2DHalf ScaleRange, int RetryTimes);
	TArray<FSpawnPosition> GetSpawnPoints(int SpawnNum, int RetryTimes, FBox & SpawnBox, FVector2DHalf &ScaleRange, float Radius);
};
