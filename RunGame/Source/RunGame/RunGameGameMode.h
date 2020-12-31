// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunGameGameMode.generated.h"

UCLASS(minimalapi)
class ARunGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TSubclassOf<AActor>> StrightFloorClass;

	UPROPERTY()
	TArray<TSubclassOf<AActor>> TurnFloorClass;

public:
	ARunGameGameMode();

	virtual void BeginPlay() override;

	void CreateFloor();

	// Floor Class
	TSubclassOf<AActor> NewFloorClass;
	TSubclassOf<AActor> GetFloorClass();

	FTransform NextTransform;

	// UI
	UUserWidget* CurrentUI;
	TSubclassOf<UUserWidget> UserWidgetClass;
	
	// Load UI To Screen
	void LoadUI();

};



