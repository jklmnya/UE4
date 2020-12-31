// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/FloorActor.h"
#include "../RunGameCharacter.h"
#include "../RunGameGameMode.h"
#include "Engine.h"
#include "Engine/World.h"

// Sets default values
AFloorActor::AFloorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxTurnComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnBox"));

	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrow"));
	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrow"));

	RootComponent = SceneComponent;
	StaticMesh->SetupAttachment(RootComponent);
	BoxComponent->SetupAttachment(RootComponent);
	BoxTurnComponent->SetupAttachment(RootComponent);

	ForwardArrow->SetupAttachment(RootComponent);
	RightArrow->SetupAttachment(RootComponent);
	LeftArrow->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<AActor> CoinClass(TEXT("/Game/Actor/Coin/Blueprint/MyCoinActor"));	
	if (CoinClass.Class != NULL) {
		CoinActorClass = CoinClass.Class;
	}
	// CoinActorClass = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Coin/Blueprint/MyCoinActor.MyCoinActor_C'"));

}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay()
{
	Super::BeginPlay();
	// BoxComponent's Collision
	// Ensure the Bind successfully
	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFloorActor::BoxComponentBeginOverLap);

	BoxTurnComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFloorActor::BoxTurnComponentBeginOverLap);
	BoxTurnComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AFloorActor::BoxTurnComponentEndOverLap);
	// Create Coins
	CreateCoinRandom();
}

// Called every frame
void AFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform AFloorActor::GetTransfrom(FVector Location) {

	FTransform Transform;
	Transform.SetLocation(ForwardArrow->GetComponentToWorld().GetLocation());
	Transform.SetRotation(ForwardArrow->GetComponentQuat());
	return Transform;

}

void AFloorActor::BoxComponentBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OverLap"));
	// Judge is RunGameCharacter overlap or not
	if (OtherActor->IsA(ARunGameCharacter::StaticClass())) {
		UWorld* World = GetWorld();
		if (World) {
			ARunGameGameMode* GameMode = Cast<ARunGameGameMode>(World->GetAuthGameMode());
			if (GameMode) {
				// Create Floor
				GameMode->CreateFloor();
				if (DestroyHandler.IsValid()) {
					GetWorldTimerManager().ClearTimer(DestroyHandler);
				}
				GetWorldTimerManager().SetTimer(DestroyHandler, this, &AFloorActor::DestroySelf, 1.0f, false);
			}
		}
	}

}

void AFloorActor::BoxTurnComponentBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA(ARunGameCharacter::StaticClass()) && FloorTy == FloorType::Turn) {
		ARunGameCharacter* Actor = Cast<ARunGameCharacter>(OtherActor);
		if (Actor) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can Turn"));
			Actor->CanTurn = true;
		}
	}
}

void AFloorActor::BoxTurnComponentEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (OtherActor->IsA(ARunGameCharacter::StaticClass()) && FloorTy == FloorType::Turn) {
		ARunGameCharacter* Actor = Cast<ARunGameCharacter>(OtherActor);
		if (Actor) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can not turn"));
			Actor->CanTurn = false;
		}
	}

}

void AFloorActor::DestroySelf() {
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Destroied"));
	// Delete Coin Memory
	if (CoinArray.Num() > 0) {
		for (AActor* Coin : CoinArray) {
			if (Coin) {
				Coin->Destroy(true);
			}
		}
	} 
	Destroy(true);
	
}

void AFloorActor::CreateCoinRandom() {

	// get random int between 0 - 4
	// 0 --> Do not create coin, 1 --> left, 2 -->right, 3 --> middle, 4 --> up
	int32 random = FMath::RandRange(0, 4);
	switch (random)
	{
	case(0):
		break;
	case(1):
		SpawnCoin(LeftArrow);
		break;
	case(2):
		SpawnCoin(RightArrow);
		break;
	case(3):
		break;
	case(4):
		break;
	default:
		break;
	}
}

void AFloorActor::SpawnCoin(UArrowComponent* Arrow) {
	
	// CoinArray.Empty();
	UWorld* World = GetWorld();
	if (World && FloorTy == FloorType::Straight) {
		int32 CoinNum = FMath::RandRange(4, 10);
		FVector Location = Arrow->GetComponentLocation();
		Location.X -= 300.f;
		FRotator Rotation = Arrow->GetComponentRotation();
		for (int32 i = 0; i < CoinNum; i++) {
			AActor* Coin = World->SpawnActor<AActor>(CoinActorClass, Location, Rotation);
			Location.X -= 200.f;
			if (Coin) {
				CoinArray.Emplace(Coin);
			}
		}
	}
}
