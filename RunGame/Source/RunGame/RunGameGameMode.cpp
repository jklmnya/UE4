// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RunGameGameMode.h"
#include "RunGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Engine.h"

#include "Public/FloorActor.h"
#include "Public/MainUserWidget.h"
#include "Public/RunGameGameStateBase.h"

ARunGameGameMode::ARunGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// set default game state class
	GameStateClass = ARunGameGameStateBase::StaticClass();

	// Get FloatActorClass
	TSubclassOf<AActor> StrightFloorClass_1 = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Floor/Blueprint/FloorActor_BP.FloorActor_BP_C'"));
	TSubclassOf<AActor> StrightFloorClass_2 = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Floor/Blueprint/FloorActor_BP_2.FloorActor_BP_2_C'"));
	TSubclassOf<AActor> StrightFloorClass_3 = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Floor/Blueprint/FloorActor_BP_3.FloorActor_BP_3_C'"));
	TSubclassOf<AActor> StrightFloorClass_4 = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Floor/Blueprint/FloorActor_BP_4.FloorActor_BP_4_C'"));
	
	StrightFloorClass.Emplace(StrightFloorClass_1);
	StrightFloorClass.Emplace(StrightFloorClass_2);
	StrightFloorClass.Emplace(StrightFloorClass_3);
	StrightFloorClass.Emplace(StrightFloorClass_4);

	TSubclassOf<AActor> FloorClass_Left = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Floor/Blueprint/FloorActor_BP_6.FloorActor_BP_6_C'"));
	TSubclassOf<AActor> FloorClass_Right = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Floor/Blueprint/FloorActor_BP_7.FloorActor_BP_7_C'"));

	TurnFloorClass.Emplace(FloorClass_Left);
	TurnFloorClass.Emplace(FloorClass_Right);

	TSubclassOf<AActor> FloorClass_Up = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Floor/Blueprint/FloorActor_BP_8.FloorActor_BP_8_C'"));
	TSubclassOf<AActor> FloorClass_Down = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Actor/Floor/Blueprint/FloorActor_BP_5.FloorActor_BP_5_C'"));

	StrightFloorClass.Emplace(FloorClass_Up);
	StrightFloorClass.Emplace(FloorClass_Down);

	// Get UserWidgetClass
	UserWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("BWidgetBlueprint'/Game/UMG/MainUserWidget.MainUserWidget_C'"));

}

void ARunGameGameMode::BeginPlay() {

	Super::BeginPlay();

	LoadUI();

	if (GetWorld()) {
		for (int16 i = 0; i < 7; i++) {
			CreateFloor();
		}
	}

}

void ARunGameGameMode::CreateFloor() {

	NewFloorClass = GetFloorClass();

	if (NewFloorClass) {
		FVector Location = NextTransform.GetLocation();
		FRotator Rotation = NextTransform.Rotator();
		// Create Floor Dynamicly
		AFloorActor* NewFloor = GetWorld()->SpawnActor<AFloorActor>(NewFloorClass, Location, Rotation);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Create"));
		NextTransform = NewFloor->GetTransfrom(Location);
	}

}

TSubclassOf<AActor> ARunGameGameMode::GetFloorClass() {

	int32 Choose = FMath::RandRange(1, 10);
	if (Choose < 8) {
		if (StrightFloorClass.Num() > 0) {
			int32 Num = FMath::RandRange(0, StrightFloorClass.Num() - 1);
			// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Stright"));
			return StrightFloorClass[Num];
		} 
	}
	else {
		if (TurnFloorClass.Num() > 0) {
			int32 Num = FMath::RandRange(0, TurnFloorClass.Num() - 1);
			// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Turn"));
			return TurnFloorClass[Num];
		}
	}
	return NULL;
	// return TSubclassOf<AActor>();

}

void ARunGameGameMode::LoadUI() {

	if (UserWidgetClass) {

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("UI"));

		UWorld* World = GetWorld();
		if (World) {
			CurrentUI = CreateWidget<UUserWidget>(World, UserWidgetClass);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("CREATE UI"));
			if (CurrentUI) {
				UMainUserWidget* CUI = Cast<UMainUserWidget>(CurrentUI);
				if (CUI) {
					CUI->CoinNum = FText::FromString(TEXT("0"));
					CUI->CharacterDistance = FText::FromString(TEXT("0"));
				}
				CurrentUI->AddToViewport();
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ADD UI"));
			}
		}
	}
}
