// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enemy.h"
#include "../Public/SpaceShipPawn.h"
#include "../Public/FlightGameModeBase.h"
#include "../Public/EnemySpawner.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StatcMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	Speed = 1000.f;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	// 获取玩家对象，游戏模式
	Player = Cast<ASpaceShipPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	GameMode = Cast<AFlightGameModeBase>(UGameplayStatics::GetGameMode(this));
	EnemySpawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(this, AEnemySpawner::StaticClass()));
	SetColor();
}

void AEnemy::MoveTowardsPlayer(float DeltaTime)
{
	// 设置敌人的位置和旋转
	if (Player) {
		FVector Target = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		AddActorWorldOffset(Target * Speed * DeltaTime, true);
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()));
	}

}

void AEnemy::OnDeath()
{
	if (GameMode) {
		GameMode->IncreaseScore();
	}
	if (EnemySpawner) {
		EnemySpawner->DecreaseEnemyNum();
	}
	// 创建死亡爆炸特效
	if (ExplosionComponent) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionComponent, GetActorLocation());
	}
	Destroy();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Player && !Player->IsDead()) {
		MoveTowardsPlayer(DeltaTime);
	}
	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

