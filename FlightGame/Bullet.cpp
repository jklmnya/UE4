// Fill out your copyright notice in the Description page of Project Settings.

/*
	�ӵ���
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

	// MovementComponent���������в���Ҫ��
	BulletMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComponent"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	// ��ȡ�����е�EnemySpwaner

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ��Ҫ�����˺�����Bullet����ײԤ��ֵӦ������ΪOverlapDynamic
void ABullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// �ȵ��ø���
	Super::NotifyActorBeginOverlap(OtherActor);
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy) {
		Enemy->OnDeath();
		Destroy();
	}	// �����������ǽ
	else if (Cast<ABlockingVolume>(OtherActor)) {
		Destroy();
	}
}

