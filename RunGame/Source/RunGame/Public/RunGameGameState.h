// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RunGameGameState.generated.h"

/**
 * 
 */
UCLASS()
class RUNGAME_API ARunGameGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	ARunGameGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	int32 CoinNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	int32 Distance;

};
