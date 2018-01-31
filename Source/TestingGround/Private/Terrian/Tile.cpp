// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

#include "Components/BoxComponent.h"
#include "Engine/World.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<AActor*> tmpActors;
	this->GetAttachedActors(OUT tmpActors);
	for (AActor* tmpActor : tmpActors)
	{
		tmpActor->Destroy();
	}
}

void ATile::SpawnProps(TArray< TSubclassOf<AActor> > SpawnActors, int MinSpawn, int MaxSpawn, float ScaleMin, float ScaleMax)
{
	auto tmpArr = GetComponentsByTag(UBoxComponent::StaticClass(), TEXT("SpawnBox"));
	if (tmpArr.Num() <= 0)
	{
		return;
	}

	this->SpawnBox = Cast<UBoxComponent>(tmpArr[0]);
	FBox tmpBox = this->SpawnBox->Bounds.GetBox();

	int8 tmpMaxSpawnTypeNum = SpawnActors.Num() - 1;

	int8 tmpSpawnNum = FMath::RandRange(MinSpawn, MaxSpawn);
		
	for (int32 i = 0; i < tmpSpawnNum; i++)
	{
		int8 tmpSpawnTypeNum = FMath::RandRange(0, tmpMaxSpawnTypeNum);

		auto tmpActorClass = SpawnActors[tmpSpawnTypeNum];
		PlaceActor(tmpActorClass, tmpBox, FVector2DHalf(ScaleMin, ScaleMax));
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> &ActorClass, FBox &SpawnBod, FVector2DHalf ScaleRange, int RetryTimes)
{
	UWorld* tmpWorld = GetWorld();
	if (tmpWorld)
	{
		AActor* tmpSpawnActor = tmpWorld->SpawnActor(ActorClass);
		if (tmpSpawnActor)
		{
			FVector tmpLoaction;
			float tmpRotate, tmpScalar;

			float tmpRadius = tmpSpawnActor->GetRootComponent()->Bounds.SphereRadius;

			for (int i = 0; i < RetryTimes; i++)
			{
				tmpLoaction = FMath::RandPointInBox(SpawnBod);
				tmpRotate = FMath::RandRange(-180, 180);
				tmpScalar = FMath::RandRange(ScaleRange.X, ScaleRange.Y);

				bool tmpIsBlock = CastSphere(tmpLoaction, tmpRadius * tmpScalar);
				if (!tmpIsBlock)
				{
					tmpSpawnActor->SetActorLocationAndRotation(tmpLoaction, FRotator(0, tmpRotate, 0));
					tmpSpawnActor->SetActorRelativeScale3D(FVector(tmpScalar));
					tmpSpawnActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					break;
				}

				if (i >= RetryTimes)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s Can't Spawn!!"), *tmpSpawnActor->GetName());
					tmpSpawnActor->Destroy();
				}
			}
		}
	}
}

bool ATile::CastSphere(FVector Location, float Radius)
{
	UWorld* tmpWorld = GetWorld();
	if (!tmpWorld)
	{
		return false;
	}
	bool bIsHit = false;

	FHitResult tmpHitResult;
	bIsHit = tmpWorld->SweepSingleByChannel(OUT tmpHitResult, Location, Location, FQuat::Identity, ECC_GameTraceChannel_SpawnProps, FCollisionShape::MakeSphere(Radius));

	FColor tmpColor = bIsHit ? FColor::Red : FColor::Green;
	
	return bIsHit;
}
