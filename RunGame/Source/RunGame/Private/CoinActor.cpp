// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/CoinActor.h"
#include "../RunGameGameMode.h"
#include "../RunGameCharacter.h"
#include "../Public/RunGameGameStateBase.h"
#include "../Public/MainUserWidget.h"

// Sets default values
ACoinActor::ACoinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	RootComponent = BoxComponent;
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACoinActor::BeginPlay()
{

	Super::BeginPlay();
	// Bind Hit Event
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoinActor::AddandDestroyCoin);

}

// Called every frame
void ACoinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoinActor::AddandDestroyCoin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA(ARunGameCharacter::StaticClass())) {
		if (GetWorld()) {
			ARunGameGameMode* GameMode = Cast<ARunGameGameMode>(GetWorld()->GetAuthGameMode());
			ARunGameGameStateBase* GameState = Cast<ARunGameGameStateBase>(GetWorld()->GetGameState());
			if (GameMode && GameState) {
				GameState->CoinNum++;
				UMainUserWidget* CUI = Cast<UMainUserWidget>(GameMode->CurrentUI);
				if (CUI) {
					CUI->CoinNum = FText::FromString(FString::FromInt(GameState->CoinNum));
				}
			}
		}
		Destroy(true);
	}
	
}

