// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemy;
class UBoxComponent;
class ASpaceShipPawn;

UCLASS()
class FLIGHTGAME_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnEnemy();

	FVector FindGenerateLocation();

	FTimerHandle EnemyGeneratorHandle;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AEnemy> Enemy;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UBoxComponent* SpawnArea;		// 生成区域

	// 生成时与玩家的最小距离
	UPROPERTY(EditAnywhere, Category = "Enemy")
	float MinmumDistanceToPlayer;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float GenerateDeltaTime;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int MaxEnemyNum;
	int CurrentEnemyNum;

	ASpaceShipPawn* Player;	//玩家

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void DecreaseEnemyNum() {
		CurrentEnemyNum--;
	}

};
