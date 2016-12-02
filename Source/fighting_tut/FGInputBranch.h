// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FGAtoms.h"
#include "FGInputBranch.generated.h"


/**
 * 
 */
UCLASS()
class FIGHTING_TUT_API UFGInputBranch : public USM_BranchBase
{
	GENERATED_BODY()
public:

	virtual USM_State* TryBranch(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 &OutDataIndex) override;

protected:

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EFGInputButtons"))
	int32 RequiredButtons;

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EFGInputButtons"))
	int32 ForbiddenButtons;

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EFGInputDirection"))
	int32 AcceptableDirections;


};
