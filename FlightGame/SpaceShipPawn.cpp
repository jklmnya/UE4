// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/SpaceShipPawn.h"
#include "../Public/Bullet.h"
#include "../Public/Enemy.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"

#include "Camera/CameraComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "TimerManager.h"

#include "Sound/SoundCue.h"

#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
// #include "Engine.h"



// Sets default values
ASpaceShipPawn::ASpaceShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMeshComponent"));
	ShipMeshComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	BulletPoint = CreateDefaultSubobject <USceneComponent> (TEXT("BulletPointComponent"));
	BulletPoint->SetupAttachment(ShipMeshComponent);

	ThrusterComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThrusterPartical"));
	ThrusterComponent->SetupAttachment(RootComponent);

	Speed = 2500.f;
	FireDeltaTime = 0.2f;
	Dead = false;

}

// Called when the game starts or when spawned
void ASpaceShipPawn::BeginPlay()
{
	// ���캯�����BeginPlay()����ִ�У���ȡController����BeginPlay
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(GetController());
	PlayerController->bShowMouseCursor = true;	// ��ʾ���
}

void ASpaceShipPawn::LookAtCursor() {
	FVector MouseLocation, MouseDirection;
	// ��ȡ�������������ϵ�µķ���ͽǶ�
	PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	// FVector TargetLocation = FVector(MouseLocation.X, MouseLocation.Y, GetActorLocation().Z);
	MouseLocation.Z = GetActorLocation().Z;
	// �������嵱ǰ�������������������Ҫ��ת�ĽǶ�
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MouseLocation);
	SetActorRotation(TargetRotation);
}

void ASpaceShipPawn::MoveUp(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
}

void ASpaceShipPawn::MoveRight(float Value)
{
	AddMovementInput(FVector::RightVector, Value);
}

void ASpaceShipPawn::Move(float DeltaTime)
{
	AddActorWorldOffset(ConsumeMovementInputVector() * DeltaTime * Speed, true);
}

void ASpaceShipPawn::Fire()
{
	if (Bullets && !Dead) {
		FActorSpawnParameters BulletParameters;
		// Engine/World.h
		GetWorld()->SpawnActor<ABullet>(Bullets, BulletPoint->GetComponentLocation(),
			BulletPoint->GetComponentRotation(), BulletParameters);
		if (ShootCue) {
			UGameplayStatics::PlaySoundAtLocation(this, ShootCue, GetActorLocation());
		}
	}
}

// ��������ʹ��TimerManager��ÿ��һ��ʱ��������Fire
void ASpaceShipPawn::StartFire()
{
	// ���ö�ʱ��
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASpaceShipPawn::Fire, FireDeltaTime, true, 0.0f);
}

void ASpaceShipPawn::EndFire()
{
	// �����ʱ��
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void ASpaceShipPawn::RestartGame()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("RestartGame"));
	UGameplayStatics::OpenLevel(this, "MainMap");
}

void ASpaceShipPawn::OnDead()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("On Dead"));
	Dead = true;
	// ��������ɼ�������Ϊ���ɼ����������
	CollisionComponent->SetVisibility(false, true);
	// ��������Sound
	if (GameOverCue) {
		UGameplayStatics::PlaySoundAtLocation(this, GameOverCue, GetActorLocation());
	}
	// ����������ը��Ч
	if (ExplosionComponent) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionComponent, GetActorLocation());
	}
	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &ASpaceShipPawn::RestartGame,
		2.0f, false);
}

// Called every frame
void ASpaceShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Dead) {
		LookAtCursor();
		Move(DeltaTime);
	}

}

// Called to bind functionality to input
void ASpaceShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// ����ӳ��, ��Ҫ����float
	PlayerInputComponent->BindAxis("MoveUp", this, &ASpaceShipPawn::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceShipPawn::MoveRight);

	// ��Actionӳ��
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceShipPawn::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASpaceShipPawn::EndFire);
}

void ASpaceShipPawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy) {
		Enemy->Destroy();
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Enemy Destroy"));
		OnDead();
	}
}

