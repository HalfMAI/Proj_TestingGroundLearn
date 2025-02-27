// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

AActor * UActorPool::Checkout()
{
	if (this->Pool.Num() == 0)
	{
		return nullptr;
	}

	return this->Pool.Pop();
}

void UActorPool::Return(AActor * ActorToReturn)
{
	if (ActorToReturn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Returned null actor."), *GetName());
		return;
	}

	this->Add(ActorToReturn);
}

void UActorPool::Add(AActor * ActorToAdd)
{
	if (ActorToAdd == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Add null actor."), *GetName());
		return;
	}

	this->Pool.Push(ActorToAdd);
}
