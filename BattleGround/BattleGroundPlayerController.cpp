// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGroundPlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "BaseCar.h"
#include "Kismet/GameplayStatics.h"
#include "BattleGroundPlayer.h"
#include "MainHud.h"
#include "InventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

void ABattleGroundPlayerController::BeginPlay()
{
	GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, FTimerDelegate::CreateLambda([&]() {
		TimerEvent();
		}), 1.0f, true);

	InventoryUIInit();
	MainUIInit();
}
ABattleGroundPlayerController::ABattleGroundPlayerController()
	:Super()
{
	m_bisCanEnterCar = false;
	m_InventoryFilpFlop = FFlipFlop(false);
}

void ABattleGroundPlayerController::PossesCar()
{
	this->OnPossess(m_MainCar);    //컨트롤의 빙의를 차량으로 옮긴다.

	m_MainPlayer->SetActorHiddenInGame(true);        //플레이어 액터를 숨긴다.
	m_MainPlayer->SetActorEnableCollision(false);        //플레이어의 콜리전도 꺼준다
	m_bisCanEnterCar = false;   //차량 탑승 가능 여부는 이미 차량을 탑승했으므로 불가능으로 변경         
}

void ABattleGroundPlayerController::PossesPawn()
{
	this->OnPossess(m_MainPlayer);

	m_MainPlayer->SetActorHiddenInGame(false);        //플레이어 액터를 보여준다
	m_MainPlayer->SetActorEnableCollision(true);        //플레이어의 콜리전도 킨다
	ABaseCar* car = Cast<ABaseCar>(m_MainCar);

	if (car != nullptr)
	{
		m_MainPlayer->SetActorLocation(car->m_CollisionLeftDoor->GetComponentLocation(), false, nullptr, ETeleportType::TeleportPhysics);

		m_bisCanEnterCar = true;
	}
}

void ABattleGroundPlayerController::InventoryUIInit()
{
	if (m_MainPlayer == nullptr)
	{
		m_MainPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	ABattleGroundPlayer* player = Cast<ABattleGroundPlayer>(m_MainPlayer);
	

	FActorSpawnParameters param;

	RecordingRef = GetWorld()->SpawnActor< ARecordingActor>(m_RecodrdingClass,
		UKismetMathLibrary::MakeTransform(FVector(10000000.0f, 0.0f, 10000.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f)),
		param);

	player->SetRecordingActor(RecordingRef);

	m_InventoryRef = CreateWidget<UInventoryWidget>(this, m_InventroyClass);
	m_InventoryRef->BuildInventory(player, this);
	m_InventoryRef->AddToViewport();
	m_InventoryRef->BuildInventory();
	m_InventoryRef->BuildGroundItem();
	m_InventoryRef->BuildEquipmentWeapon(EquipmentSlotCategory::Slot_PrimaryWeapon);
	m_InventoryRef->BuildEquipmentWeapon(EquipmentSlotCategory::Slot_SecondWeapon);
	m_InventoryRef->BuildEquipmentWeapon(EquipmentSlotCategory::Slot_PistolWeapon);
	m_InventoryRef->SetVisibility(ESlateVisibility::Collapsed);

}

void ABattleGroundPlayerController::MainUIInit()
{
	ABattleGroundPlayer* player = Cast<ABattleGroundPlayer>(m_MainPlayer);

	m_MainHud = CreateWidget<UMainHud>(this, m_MainHudlass);
	m_MainHud->BuildHud(player);
	m_MainHud->AddToViewport();
}

void ABattleGroundPlayerController::Interactive()
{
	APawn* pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (m_MainCar == pawn || m_bisCanEnterCar)
	{
		ABaseCar* car = Cast<ABaseCar>(m_MainCar);

		if (car != nullptr)
		{
			car->EnterExitCar();
		}
	}
}

void ABattleGroundPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ABattleGroundPlayerController::Interactive);
	InputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ABattleGroundPlayerController::InventoryPress);
}

void ABattleGroundPlayerController::TimerEvent()
{
	ABattleGroundPlayer* player = Cast<ABattleGroundPlayer>(m_MainPlayer);

	if (player != nullptr)
	{
		player->DealDamage();
		player->ChangeColor();
	}
}

void ABattleGroundPlayerController::InventoryPress()
{
	ABattleGroundPlayer* player = Cast<ABattleGroundPlayer>(m_MainPlayer);
	if(player != nullptr)
		player->GroundItemAdd();

	if (m_InventoryFilpFlop.Execute())
	{
		m_InventoryRef->SetVisibility(ESlateVisibility::Visible);
		player->InputModeUI(m_InventoryRef);
		m_InventoryRef->BuildInventory();
		m_InventoryRef->BuildGroundItem();
		m_InventoryRef->BuildEquipmentWeapon(EquipmentSlotCategory::Slot_PrimaryWeapon);
		m_InventoryRef->BuildEquipmentWeapon(EquipmentSlotCategory::Slot_SecondWeapon);
		m_InventoryRef->BuildEquipmentWeapon(EquipmentSlotCategory::Slot_PistolWeapon);
		
	}
	else
	{
		m_InventoryRef->SetVisibility(ESlateVisibility::Collapsed);
		player->InputModeUI(nullptr, false);
	}
}