// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "SM_State.h"
#include "FGMove.generated.h"

class UFGMoveLink;
class AFGDefaultPawn;


USTRUCT()
struct FFGMoveLinkToFollow
{
	GENERATED_BODY()
		
	FFGMoveLinkToFollow()
	{
		Link = nullptr;
	}

	UPROPERTY()
		UFGMoveLink* Link;
	UPROPERTY()
		FStateMachineResult SMR;

};
/**
 * 
 */
UCLASS()
class FIGHTING_TUT_API UFGMove : public UDataAsset
{
	GENERATED_BODY()
public:
	FFGMoveLinkToFollow TryLinks(const AFGDefaultPawn* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MoveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Animation;

	UPROPERTY(EditAnywhere)
	TArray<UFGMoveLink*> LinkedMoves;

	UPROPERTY(EditAnywhere)
	uint32 bClearInputOnEntry : 1;
	UPROPERTY(EditAnywhere)
	uint32 bClearInputOnExit : 1;

};

class AFGDefaultPawn;
class UFGMove;

UCLASS()
class FIGHTING_TUT_API UFGMoveLink : public UDataAsset
{
	GENERATED_BODY()
public:

	FStateMachineResult TryLink(const AFGDefaultPawn* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps);

	UPROPERTY(EditAnywhere)
	UFGMove* move;
	
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> CancelWindows;
	
	UPROPERTY(EditAnywhere)
	uint32 bClearInput : 1;

	UPROPERTY(EditAnywhere)
	UFGState* InputStateMachine;
};
