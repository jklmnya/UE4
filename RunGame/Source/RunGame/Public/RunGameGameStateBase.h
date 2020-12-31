// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RunGameGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class RUNGAME_API ARunGameGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	ARunGameGameStateBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	int32 CoinNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	int32 Distance;

	// Increase Distance
	void IncreaseDistance();

	// Distance Handler
	FTimerHandle DistanceHandler;

protected:

	virtual void BeginPlay() override;

};
