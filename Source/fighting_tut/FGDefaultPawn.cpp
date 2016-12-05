// Fill out your copyright notice in the Description page of Project Settings.
#include "fighting_tut.h"
#include "FGAtoms.h"
#include "FGMove.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FGDefaultPawn.h"



AFGDefaultPawn::AFGDefaultPawn()
{
	bAddDefaultMovementBindings = false;
	InputExpirationTime = 2.0f;
}
void AFGDefaultPawn::BeginPlay() 
{
	Super::BeginPlay();

	if (!CurrentMove)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Initial Move."));
		Destroy();
		return; 
	}

	if (!DirectionDownBackAtom || ! DirectionDownAtom || !DirectionDownForwardAtom ||
		!DirectionBackAtom || !DirectionNeutralAtom || !DirectionForwardAtom ||
		!DirectionUpBackAtom || !DirectionUpAtom || !DirectionUpForwardAtom)

	{
		UE_LOG(LogTemp, Warning, TEXT("Missing at least one directional atom. "));
		Destroy();
		return;
	}

	int32 ButtonInputCount = (int32)EFGButtonState::Count;
	for (int32 i = 0; i < ButtonInputCount; ++i)
	{
		if (!ButtonAtoms.IsValidIndex(i) || !ButtonAtoms[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough input button atoms, or something was null"));
			Destroy();
			return;
		}
	}


}

void AFGDefaultPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Process input

	// add one atom for stick direction
	const float DirectionThreshold = 0.5f;

	UFGDirectionalInputAtom* InputDirection = nullptr;
	if (DirectionInput.X < -DirectionThreshold)
	{
		if (DirectionInput.Y < -DirectionThreshold)
		{
			InputDirection = DirectionDownBackAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			InputDirection = DirectionBackAtom;
		}
		else 
		{
			InputDirection = DirectionUpBackAtom;
		}
	}
	else if (DirectionInput.X < DirectionThreshold)
	{
		if (DirectionInput.Y < -DirectionThreshold)
		{
			InputDirection = DirectionDownAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			InputDirection = DirectionNeutralAtom;
		}
		else
		{
			InputDirection = DirectionUpAtom;
		}
	}
	else
	{
		if (DirectionInput.Y < -DirectionThreshold)
		{
			InputDirection = DirectionDownForwardAtom;
		}
		else if (DirectionInput.Y < DirectionThreshold)
		{
			InputDirection = DirectionForwardAtom;
		}
		else
		{
			InputDirection = DirectionUpForwardAtom;
		}
	}

	InputStream.Add(InputDirection);

	for (int32 i = 0; i < (int32)EFGInputButtons::Count; ++i)
	{
		if (ButtonsDown & (1 << i))
		{
			if (ButtonsDown_Old & (1 << i))
			{
				InputStream.Add(ButtonAtoms[(int32)EFGButtonState::HeldDown]);
			}
			else
			{
				InputStream.Add(ButtonAtoms[(int32)EFGButtonState::JustPressed]);
			}
		}
		else
		{
			InputStream.Add(ButtonAtoms[(int32)EFGButtonState::Up]);
		}
	}

	ButtonsDown_Old = ButtonsDown;

	float CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);
	InputTimeStamps.Add(CurrentTime);

	for (int32 i = 0; i < InputStream.Num(); ++i)
	{
		if ((InputTimeStamps[i] + InputExpirationTime) >= CurrentTime)
		{
			if (i > 0)
			{
				InputTimeStamps.RemoveAt(0, i, false);
				InputStream.RemoveAt(0, i * ((int32)EFGInputButtons::Count + 1), false);
			}
			break;
		}
	}

	FFGMoveLinkToFollow MoveLinkToFollow = CurrentMove->TryLinks(this, InputStream);
	if (MoveLinkToFollow.Link)
	{
		UE_LOG(LogTemp, Warning, TEXT("Switching to state %s"), *MoveLinkToFollow.Link->Move->MoveName.ToString());
		if (MoveLinkToFollow.Link->bClearInput || MoveLinkToFollow.Link->Move->bClearInputOnEntry || CurrentMove->bClearInputOnExit)
		{
			InputStream.Reset();
			InputTimeStamps.Reset();
		}
		else if (MoveLinkToFollow.SMR.DataIndex)
		{
			check((MoveLinkToFollow.SMR.DataIndex % (1 + (int32)EFGInputButtons::Count)) == 0);
			InputTimeStamps.RemoveAt(0, MoveLinkToFollow.SMR.DataIndex / (1 + (int32)EFGInputButtons::Count), false);
			InputStream.RemoveAt(0, MoveLinkToFollow.SMR.DataIndex, false);
		}

		CurrentMove = MoveLinkToFollow.Link->Move;
		TimeInCurrentMove = 0.0f;
		DoMove(CurrentMove);
	}
	else
	{
		TimeInCurrentMove += DeltaSeconds; // modulate by move animation length
	}
}

void AFGDefaultPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);

	InInputComponent->BindAxis("XAxis", this, &AFGDefaultPawn::ReadXAxis);
	InInputComponent->BindAxis("YAxis", this, &AFGDefaultPawn::ReadYAxis);
	InInputComponent->BindAction("LeftButton", IE_Pressed, this, &AFGDefaultPawn::LeftButtonPressed);
	InInputComponent->BindAction("LeftButton", IE_Released, this, &AFGDefaultPawn::LeftButtonReleased);
	InInputComponent->BindAction("TopButton", IE_Pressed, this, &AFGDefaultPawn::TopButtonPressed);
	InInputComponent->BindAction("TopButton", IE_Released, this, &AFGDefaultPawn::TopButtonReleased);
}

void AFGDefaultPawn::ReadXAxis(float Value)
{
	DirectionInput.Y = Value;
}

void AFGDefaultPawn::ReadYAxis(float Value)
{
	DirectionInput.Y = Value;
}

void AFGDefaultPawn::LeftButtonPressed()
{
	ButtonsDown |= (1 << (int32)EFGInputButtons::LeftFace);
}
void AFGDefaultPawn::LeftButtonReleased()
{
	ButtonsDown &= ~(1 << (int32)EFGInputButtons::LeftFace);
}
void AFGDefaultPawn::TopButtonPressed()
{
	ButtonsDown |= (1 << (int32)EFGInputButtons::TopFace);
}
void AFGDefaultPawn::TopButtonReleased()
{
	ButtonsDown &= ~(1 << (int32)EFGInputButtons::LeftFace);
}