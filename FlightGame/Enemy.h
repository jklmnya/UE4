// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

class USphereComponent;
class ASpaceShipPawn;
class AFlightGameModeBase;
class AEnemySpawner;

UCLASS()
class FLIGHTGAME_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveTowardsPlayer(float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetColor();		// 在蓝图中可以修改敌人颜色的事件

	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystem* ExplosionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed;

	ASpaceShipPawn* Player;

	AFlightGameModeBase* GameMode;

	AEnemySpawner* EnemySpawner;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDeath();

};
