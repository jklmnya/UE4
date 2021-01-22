// Fill out your copyright notice in the Description page of Project Settings.

/*
	子弹类
*/

#include "../Public/Bullet.h"
#include "../Public/Enemy.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

#include "Engine/BlockingVolume.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	RootComponent = SceneComponent;
	StaticMeshComponent->SetupAttachment(RootComponent);

	// MovementComponent与根组件并列不需要绑定
	BulletMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComponent"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	// 获取场景中的EnemySpwaner

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 若要触发此函数，Bullet的碰撞预设值应该设置为OverlapDynamic
void ABullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// 先调用父类
	Super::NotifyActorBeginOverlap(OtherActor);
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy) {
		Enemy->OnDeath();
		Destroy();
	}	// 如果碰到空气墙
	else if (Cast<ABlockingVolume>(OtherActor)) {
		Destroy();
	}
}

