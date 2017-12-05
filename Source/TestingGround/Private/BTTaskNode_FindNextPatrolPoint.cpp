// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_FindNextPatrolPoint.h"

EBTNodeResult::Type UBTTaskNode_FindNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UBlackboardComponent *tmpBlackboardCom = OwnerComp.GetBlackboardComponent();

	int32 tmpCurrent = tmpBlackboardCom->GetValueAsInt(this->NextIndex.SelectedKeyName);
	
	auto tmpAIController = OwnerComp.GetAIOwner();
	ACharacter* tmpChara = Cast<ACharacter>(tmpAIController->GetPawn());
	if (tmpChara != NULL)
	{		
		auto tmpCom = tmpChara->FindComponentByClass<UPatrolTaskPointComponent>();

		if (tmpCom)
		{
			int tmpNextIndex = tmpBlackboardCom->GetValueAsInt(this->NextIndex.SelectedKeyName);

			TArray<AActor*> tmpPointArr = tmpCom->PatrolPoints;
			tmpBlackboardCom->SetValueAsObject(this->CurPatrolPoint.SelectedKeyName, tmpPointArr[tmpNextIndex]);

			tmpNextIndex = (tmpNextIndex + 1) % tmpPointArr.Num();
			tmpBlackboardCom->SetValueAsInt(this->NextIndex.SelectedKeyName, tmpNextIndex);
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
