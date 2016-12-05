// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DefaultPawn.h"

#include "FGDefaultPawn.generated.h"


class UFGDirectionalInputAtom;
class UFGButtonInputAtom;
class UFGMove;

/**
 * 
 */
UCLASS()
class FIGHTING_TUT_API AFGDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()
	
public:
	AFGDefaultPawn();
	FORCEINLINE float GetTimeInMove() const { return TimeInCurrentMove;	}
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaSeconds) override;
		virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent)override;
protected:
	void LeftButtonPressed();
	void LeftButtonReleased();
	void TopButtonPressed();
	void TopButtonReleased();

	void ReadXAxis(float Value);
	void ReadYAxis(float Value);

	FVector2D DirectionInput;
	float TimeInCurrentMove;
	uint32 ButtonsDown;
	uint32 ButtonsDown_Old;


	UPROPERTY(EditAnywhere)
		UFGMove* CurrentMove;
	// Input atoms are removed when they pass this age threshold. All moves must be executed in under this time.
	UPROPERTY(EditAnywhere)
		float InputExpirationTime;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionDownBackAtom;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionDownAtom;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionDownForwardAtom;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionBackAtom;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionNeutralAtom;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionForwardAtom;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionUpBackAtom;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionUpAtom;

	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		UFGDirectionalInputAtom* DirectionUpForwardAtom;

	// Order in this array is the same as EFGButtonState: Up, JustPressed, Held
	UPROPERTY(EditAnywhere, Category = "Input Atoms")
		TArray<UFGButtonInputAtom*> ButtonAtoms;


	UFUNCTION(BlueprintImplementableEvent)
		void DoMove(UFGMove* NewMove);
private:


	UPROPERTY(VisibleInstanceOnly)
		TArray<float> InputTimeStamps;

	UPROPERTY(VisibleInstanceOnly)
		TArray<USM_InputAtom*> InputStream;
};