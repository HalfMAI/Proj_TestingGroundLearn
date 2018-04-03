// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "ActorPool.h"

#include "AI/Navigation/NavigationSystem.h"

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

	if (this->ActorPool)
	{
		this->ActorPool->Return(this->TitleNavMeshBoundsVolume);
	}
}

void ATile::SpawnProps(TArray< TSubclassOf<AActor> > SpawnActors, int MinSpawn, int MaxSpawn, float ScaleMin, float ScaleMax)
{
	FVector2DHalf tmp = FVector2DHalf(ScaleMin, ScaleMax);

	FBox tmpSpawnBox = this->_GetSpawnBox();

	int8 tmpSpawnNum = FMath::RandRange(MinSpawn, MaxSpawn);

	int8 tmpMaxSpawnTypeNum = SpawnActors.Num() - 1;
	int8 tmpSpawnTypeNum = FMath::RandRange(0, tmpMaxSpawnTypeNum);

	auto tmpActorClass = SpawnActors[tmpSpawnTypeNum];
	AActor* tmpSpawnActor = GetWorld()->SpawnActor(tmpActorClass);
	if (tmpSpawnActor)
	{
		float tmpRadius = tmpSpawnActor->GetRootComponent()->Bounds.SphereRadius;
		tmpSpawnActor->Destroy();

		TArray<FSpawnPosition> tmpSpawnPositions = GetSpawnPoints(tmpSpawnNum, 10, tmpSpawnBox, tmp, tmpRadius);
		for (FSpawnPosition tmpPos : tmpSpawnPositions)
		{
			this->_SpawnProps(tmpActorClass, tmpPos);
		}
	}

	//this->PlaceActor< TSubclassOf<AActor> >(SpawnActors, &ATile::_SpawnProps, MinSpawn, MaxSpawn, tmp, 10);
}

void ATile::SpawnAIPawns(TArray< TSubclassOf<APawn> > SpawnPawns, int MinSpawn, int MaxSpawn)
{
	FVector2DHalf tmp = FVector2DHalf(1, 1);

	FBox tmpSpawnBox = this->_GetSpawnBox();

	int8 tmpSpawnNum = FMath::RandRange(MinSpawn, MaxSpawn);

	int8 tmpMaxSpawnTypeNum = SpawnPawns.Num() - 1;
	int8 tmpSpawnTypeNum = FMath::RandRange(0, tmpMaxSpawnTypeNum);

	auto tmpActorClass = SpawnPawns[tmpSpawnTypeNum];
	AActor* tmpSpawnActor = GetWorld()->SpawnActor(tmpActorClass);
	if (tmpSpawnActor)
	{
		float tmpRadius = tmpSpawnActor->GetRootComponent()->Bounds.SphereRadius;
		tmpSpawnActor->Destroy();

		TArray<FSpawnPosition> tmpSpawnPositions = GetSpawnPoints(tmpSpawnNum, 10, tmpSpawnBox, tmp, tmpRadius);
		for (FSpawnPosition tmpPos : tmpSpawnPositions)
		{
			this->_SpawnAI(tmpActorClass, tmpPos);
		}
	}

	//this->PlaceActor< TSubclassOf<APawn> >(SpawnPawns, &ATile::_SpawnAI, MinSpawn, MaxSpawn, tmp, 10);
}

template<class T>
void ATile::PlaceActor(TArray<T> SpawnActors, SpawnFuncType<T> SpawnFunc, int MinSpawn, int MaxSpawn, FVector2DHalf ScaleRange, int RetryTimes)
{
	FBox tmpSpawnBox = this->_GetSpawnBox();

	int8 tmpSpawnNum = FMath::RandRange(MinSpawn, MaxSpawn);

	int8 tmpMaxSpawnTypeNum = SpawnActors.Num() - 1;
	int8 tmpSpawnTypeNum = FMath::RandRange(0, tmpMaxSpawnTypeNum);

	auto tmpActorClass = SpawnActors[tmpSpawnTypeNum];
	AActor* tmpSpawnActor = GetWorld()->SpawnActor(tmpActorClass);
	if (tmpSpawnActor)
	{
		float tmpRadius = tmpSpawnActor->GetRootComponent()->Bounds.SphereRadius;
		tmpSpawnActor->Destroy();

		TArray<FSpawnPosition> tmpSpawnPositions = GetSpawnPoints(tmpSpawnNum, RetryTimes, tmpSpawnBox, ScaleRange, tmpRadius);
		for (FSpawnPosition tmpPos : tmpSpawnPositions)
		{
			this->*SpawnFunc(tmpActorClass, tmpPos);
		}
	}
}

TArray<FSpawnPosition> ATile::GetSpawnPoints(int SpawnNum, int RetryTimes, FBox & SpawnBox, FVector2DHalf &ScaleRange, float Radius)
{
	TArray<FSpawnPosition> retSpawnPoints;

	for (int32 i = 0; i < SpawnNum; i++)
	{
		for (int j = 0; j <= RetryTimes; j++)
		{
			FVector tmpLoaction = FMath::RandPointInBox(SpawnBox);
			float tmpRotate = FMath::RandRange(-180, 180);
			float tmpScalar = FMath::RandRange(ScaleRange.X, ScaleRange.Y);

			bool tmpIsBlock = CastSphere(tmpLoaction, Radius * tmpScalar);
			if (!tmpIsBlock)
			{
				FSpawnPosition tmpSpawnPosition;
				tmpSpawnPosition.Location = tmpLoaction;
				tmpSpawnPosition.Rotation = tmpRotate;
				tmpSpawnPosition.Scale = tmpScalar;

				retSpawnPoints.Add(tmpSpawnPosition);
				break;
			}
		}
	}
		
	
	return retSpawnPoints;
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

void ATile::SetupPool(UActorPool * InPool)
{
	this->ActorPool = InPool;

	this->PositionNavMeshBoundsVolume();
}

FBox ATile::_GetSpawnBox()
{
	auto tmpArr = GetComponentsByTag(UBoxComponent::StaticClass(), TEXT("SpawnBox"));
	if (tmpArr.Num() <= 0)
	{
		return FBox();
	}

	this->SpawnBox = Cast<UBoxComponent>(tmpArr[0]);
	FBox tmpBox = this->SpawnBox->Bounds.GetBox();

	return tmpBox;
}

AActor* ATile::_SpawnProps(TSubclassOf<AActor> SpawnPropClass, FSpawnPosition SpawnPosition)
{
	UWorld* tmpWorld = GetWorld();
	if (tmpWorld)
	{
		AActor* tmpSpawnActor = tmpWorld->SpawnActor(SpawnPropClass);
		if (tmpSpawnActor)
		{
			tmpSpawnActor->SetActorLocationAndRotation(SpawnPosition.Location, FRotator(0, SpawnPosition.Rotation, 0));
			tmpSpawnActor->SetActorRelativeScale3D(FVector(SpawnPosition.Scale));
			tmpSpawnActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		return tmpSpawnActor;
	}
	return nullptr;
}

AActor* ATile::_SpawnAI(TSubclassOf<APawn> SpawnAIPawnClass, FSpawnPosition SpawnPosition)
{
	UWorld* tmpWorld = GetWorld();
	if (tmpWorld)
	{
		AActor* tmpSpawnActor = tmpWorld->SpawnActor(SpawnAIPawnClass);
		if (tmpSpawnActor)
		{
			tmpSpawnActor->SetActorLocationAndRotation(SpawnPosition.Location, FRotator(0, SpawnPosition.Rotation, 0));
			tmpSpawnActor->SetActorRelativeScale3D(FVector(SpawnPosition.Scale));
			tmpSpawnActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			tmpSpawnActor->Tags.Add(FName("'Enemy'"));
		}
		return tmpSpawnActor;
	}
	return nullptr;
}

void ATile::PositionNavMeshBoundsVolume()
{
	this->TitleNavMeshBoundsVolume = this->ActorPool->Checkout();

	if (this->TitleNavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not Enough actors in Pool"), *GetName());
		return;	
	}
	UE_LOG(LogTemp, Error, TEXT("[%s] Check out %s."), *GetName(), *this->TitleNavMeshBoundsVolume->GetName());
	this->TitleNavMeshBoundsVolume->SetActorLocation(GetActorLocation() + FVector(2000, 0, 0));
	GetWorld()->GetNavigationSystem()->Build();
}