// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/EnemySpawner.h"
#include "../Public/Enemy.h"
#include "../Public/SpaceShipPawn.h"

#include "Components/BoxComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"

#include "TimerManager.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;

	MinmumDistanceToPlayer = 500.f;

	GenerateDeltaTime = 2.0f;

	MaxEnemyNum = 50;
	CurrentEnemyNum = 0;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	// 获取玩家
	Player = Cast<ASpaceShipPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	// 随机生成敌人的定时器
	if (Player) {
		GetWorldTimerManager().SetTimer(EnemyGeneratorHandle, this, &AEnemySpawner::SpawnEnemy,
			GenerateDeltaTime, true, 0.0f);
	}
	
}

void AEnemySpawner::SpawnEnemy()
{
	if (Enemy && !Player->IsDead() && CurrentEnemyNum < MaxEnemyNum) {
		GetWorld()->SpawnActor<AEnemy>(Enemy, FindGenerateLocation(), FRotator::ZeroRotator);
		CurrentEnemyNum++;
	}
}

FVector AEnemySpawner::FindGenerateLocation()
{
	float CurrentDistacne = 0.f;
	FVector Location;
	while (CurrentDistacne < MinmumDistanceToPlayer) {
		Location = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);
		CurrentDistacne = (Location - Player->GetActorLocation()).Size();
	}
	return Location;

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

