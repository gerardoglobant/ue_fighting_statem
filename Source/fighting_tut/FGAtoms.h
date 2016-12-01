// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SM_State.h"
#include "FGAtoms.generated.h"

/**
 * 
 */
UENUM(Blueprintable, Meta = (Bitflags))
enum class EFGInputDirection : uint8
{
	DownBack, 
	Down, 
	DownForward,
	Back, 
	Neutral,
	Forward, 
	UpBack,
	Up,
	UpForward
};

UENUM()
enum class EFGButtonState : uint8
{
	Up,
	JustPressed,
	HeldDown,
	Count UMETA(Hidden)
};

UENUM(Blueprintable, Meta = (Bitflags))
enum class EFGInputButtons : uint8
{
	LeftFace,
	TopFace,
	Count UMETA(Hidden)
};

UCLASS()
class FIGHTING_TUT_API UFGDirectionalInputAtom : public USM_InputAtom
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EFGInputDirection InputDirection;
};
UCLASS()
class FIGHTING_TUT_API UFGButtonInputAtom : public USM_InputAtom
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EFGButtonState ButtonState;
};