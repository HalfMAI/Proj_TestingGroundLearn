// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BlackboardComponent.h"





#include "AIController.h"
#include "PatrolTaskPointComponent.h"
//#include "GameFramework/Character.h"
#include "BTTaskNode_FindNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API UBTTaskNode_FindNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	

protected:
	UPROPERTY(EditAnywhere,	Category = "Blackboard")
	struct FBlackboardKeySelector NextIndex;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector CurPatrolPoint;
};
