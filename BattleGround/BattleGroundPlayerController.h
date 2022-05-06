// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Utility.h"
#include "BattleGroundPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API ABattleGroundPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	bool m_bisLeftDoor;
	bool m_bisCanEnterCar;
	class APawn* m_MainCar;
	class ACharacter* m_MainPlayer;
	FTimerHandle m_TimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UInventoryWidget> m_InventroyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		class UInventoryWidget* m_InventoryRef;

	class UMainHud* m_MainHud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UMainHud> m_MainHudlass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		class ARecordingActor* RecordingRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ARecordingActor> m_RecodrdingClass;
	FFlipFlop m_InventoryFilpFlop;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	ABattleGroundPlayerController();

	void SetPlayer(class ACharacter* BattlePlayer) { m_MainPlayer = BattlePlayer; }
	class UInventoryWidget* GetInventoryWidget() { return m_InventoryRef; }
	class UMainHud* GetHudWidget() { return m_MainHud; }
	UFUNCTION(BlueprintCallable, Category = "Setting")
		bool GetIsLeftDoor() { return m_bisLeftDoor; }
	UFUNCTION(BlueprintCallable, Category = "Setting")
		bool GetIsEnterCar() { return m_bisCanEnterCar; }
	UFUNCTION(BlueprintCallable, Category = "Setting")
		void SetIsLeftDoor(bool val) { m_bisLeftDoor = val; }
	UFUNCTION(BlueprintCallable, Category = "Setting")
		void SetIsEnterCar(bool val) { m_bisCanEnterCar = val; }
	UFUNCTION(BlueprintCallable, Category = "Setting")
		void SetMainCar(class APawn* val) { m_MainCar = val; }
	UFUNCTION(BlueprintCallable, Category = "Car")
		void PossesCar();
	UFUNCTION(BlueprintCallable, Category = "Car")
		void PossesPawn();
	UFUNCTION(BlueprintCallable, Category = "Init")
		void InventoryUIInit();
	UFUNCTION(BlueprintCallable, Category = "Init")
		void MainUIInit();
	UFUNCTION(BlueprintCallable, Category = "Car")
		void Interactive();

	ARecordingActor* GetRecordingActor() { return RecordingRef; }
	void InventoryPress();
	virtual void SetupInputComponent() override;
	void TimerEvent();
};
