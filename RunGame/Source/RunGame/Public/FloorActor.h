// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FloorActor.generated.h"

// Floor's Kinds
UENUM() 
namespace FloorType {
	enum Type {
		Straight,
		Turn,
		UpAndDown
	};
}

UCLASS()
class RUNGAME_API AFloorActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxTurnComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ForwardArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* RightArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* LeftArrow;

	UPROPERTY(EditDefaultsOnly, Category = "FloorKind")
	TEnumAsByte<FloorType::Type> FloorTy;

public:	
	// Sets default values for this actor's properties
	AFloorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get Transfrom
	FTransform GetTransfrom(FVector Location);

	// Destroy Floor Timer
	FTimerHandle DestroyHandler;

	UFUNCTION()
	void BoxComponentBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxTurnComponentBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BoxTurnComponentEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void DestroySelf();

	// Coin Actor Class 
	TSubclassOf<AActor> CoinActorClass;
	// Create Coin Randomly
	void CreateCoinRandom();
	// Coins TArray
	TArray<AActor*> CoinArray;
	// SpawnCoin
	void SpawnCoin(UArrowComponent* Arrow);

};
