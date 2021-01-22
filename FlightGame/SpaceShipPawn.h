// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShipPawn.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class ABullet;
class USoundCue;

UCLASS()
class FLIGHTGAME_API ASpaceShipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LookAtCursor();

	void MoveUp(float Value);

	void MoveRight(float Value);

	void Move(float DeltaTime);

	void Fire();

	void StartFire();

	void EndFire();

	void RestartGame();
	void OnDead();

	FTimerHandle ShootTimerHandle;
	FTimerHandle RestartTimerHandle;

	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* GameOverCue;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* ShootCue;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ThrusterComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UParticleSystem* ExplosionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;	// 碰撞组件

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent;	// 弹簧臂组件

	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float FireDeltaTime;

	bool Dead;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* BulletPoint;		// 子弹生成的位置

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABullet> Bullets;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	FORCEINLINE bool IsDead() const {
		return Dead;
	}

};
