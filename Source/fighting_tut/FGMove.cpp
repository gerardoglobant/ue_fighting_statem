// Fill out your copyright notice in the Description page of Project Settings.

#include "fighting_tut.h"
#include "FGMove.h"
#include "FGDefaultPawn.h"

FFGMoveLinkToFollow UFGMove::TryLinks(const AFGDefaultPawn* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps)
{
	FFGMoveLinkToFollow ResultLink;
	
	for (int32 i = 0; i < LinkedMoves.Num(); ++i)
	{
		check(LinkedMoves[i]);
		ResultLink.SMR = LinkedMoves[i]->TryLink(RefObject, DataSource, DataIndex, RemainingSteps);

		if (ResultLink.SMR.CompletionType == EStateMachineCompletionType::Accepted)
		{
			ResultLink.Link = LinkedMoves[i];
			return ResultLink;
		}
	}

	return ResultLink;
}

FStateMachineResult UFGMoveLink::TryLink(const AFGDefaultPawn* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps)
{
	if (InputStateMachine && Move)
	{
		bool bCanCancel = false;
		for (const FVector2D& CancelWindow : CancelWindows)
		{
			if (RefObject->GetTimeInMove() == FMath::Clamp(RefObject->GetTimeInMove(), CancelWindow.X, CancelWindow.Y))
			{
				bCanCancel = true;
				break;
			}
		}

		if (bCanCancel == true || !CancelWindows.Num())
		{
			return InputStateMachine->RunState(RefObject, DataSource, DataIndex, RemainingSteps);
		}
	}
	return FStateMachineResult();
}