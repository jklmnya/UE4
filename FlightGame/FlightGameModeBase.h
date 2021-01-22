// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlightGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTGAME_API AFlightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	int Score;

public:

	AFlightGameModeBase();

	FORCEINLINE void IncreaseScore() {
		Score += 10;
	}

};
