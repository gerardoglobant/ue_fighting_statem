// Fill out your copyright notice in the Description page of Project Settings.

#include "fighting_tut.h"
#include "FGState.h"




// Loop. Used when input atom being processed isn't recognized.
FStateMachineResult UFGState::LoopState(const UObject* RefObject,
	const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps)
{
	return RunState(RefObject, DataSource, DataIndex+1+ (int32)EFGInputButtons::Count, RemainingSteps);
}