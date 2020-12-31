// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/RunGameGameStateBase.h"
#include "../RunGameCharacter.h"
#include "../RunGameGameMode.h"
#include "../Public/MainUserWidget.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

ARunGameGameStateBase::ARunGameGameStateBase() {

	CoinNum = 0;
	Distance = 0;

}

void ARunGameGameStateBase::IncreaseDistance() {

	if (GetWorld()) {
		ARunGameCharacter* Character = Cast<ARunGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		ARunGameGameMode* GameMode = Cast<ARunGameGameMode>(GetWorld()->GetAuthGameMode());
		if (Character) {
			FVector Velocity = Character->GetVelocity();
			Distance += Velocity.Size();
		}
		if (GameMode) {
			UMainUserWidget* CUI = Cast<UMainUserWidget>(GameMode->CurrentUI);
			if (CUI) {
				CUI->CharacterDistance = FText::FromString(FString::FromInt(Distance));
			}
		}
	}
	
}

void ARunGameGameStateBase::BeginPlay() {

	Super::BeginPlay();

	// Set Distance Handler
	GetWorldTimerManager().SetTimer(DistanceHandler, this, &ARunGameGameStateBase::IncreaseDistance, 2.f, true);
}
