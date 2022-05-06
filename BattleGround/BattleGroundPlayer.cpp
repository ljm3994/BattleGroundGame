// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGroundPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BaseItem.h"
#include "BaseWeapone.h"
#include "InventoryWidget.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "RecordingActor.h"
#include "BattleGroundPlayerController.h"
#include "MainHud.h"
#include "RecordingActor.h"
#include "InteractiveWidget.h"
#include "Components/PostProcessComponent.h"

// Sets default values
ABattleGroundPlayer::ABattleGroundPlayer()
	:Super()
{
	m_CurveVertical = nullptr;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_bisMaker = false;
	m_TPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	m_FPCamera = CreateOptionalDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));

	m_FPCamera->bAutoActivate = false;
	m_FPCamera->bUsePawnControlRotation = true;
	m_FPCamera->SetWorldRotation(FQuat(FRotator(0.0f, 90.0f, 0.0f)));
	m_Arm->AttachTo(RootComponent);
	m_Arm->TargetArmLength = 300.f;
	m_Arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	m_Arm->bUsePawnControlRotation = true;
	PrimaryActorTick.bCanEverTick = true;
	m_PlayerState = PlayerMoveState::State_Ground;
	m_PlayerStance = PlayerStance::Stance_Standing;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	JumpMaxCount = 1;
	m_PlayerViewMode = PlayerViewMode::View_Third;
	m_TPCamera->bUsePawnControlRotation = false;
	m_TPCamera->AttachTo(m_Arm, USpringArmComponent::SocketName);
	m_FPCamera->AttachTo(GetMesh(), TEXT("FP_Camera"));
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	m_CurrentEquipmentSlot = EquipmentSlotCategory::Slot_PrimaryWeapon;

	WeaponSocketName = TEXT("WeaponHoldSocket");
	m_bisRifle = false;
	bisInit = false;
	m_TimeLineUpdateEvent.BindUFunction(this, TEXT("CameraVerticalTLEvent"));

	m_iPlayerHP = 100;
	m_iHeadPlayerDefece = 0.0f;
	m_iBodyPlayerDefece = 0.0f;
	m_fThirdFov = 90.0f;
	m_fFirstFov = 90.0f;
	m_FPSocketName = "FP_Camera";

	m_TimeVertical = CreateDefaultSubobject<UTimelineComponent>(TEXT("VerticalTL"));
}

// Called when the game starts or when spawned
void ABattleGroundPlayer::BeginPlay()
{
	Super::BeginPlay();

	m_Arm->TargetArmLength = 300.f;
	m_Arm->SetRelativeRotation(FRotator(-45, 0, 0));
	m_TPCamera->SetRelativeRotation(FRotator(0, 0, 0));
	bUseControllerRotationYaw = true;
	m_bisFirstPerson = true;
	CameraChange();
	m_bIsIncir = false;
	m_bisDead = false;
	
	m_FPCamera->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	EquipmentSlotInit();
	SetTimerInit();
	m_ArrayInventory.Reset(0);
	m_ArrGroundItem.Reset(0);
	m_bisInWidget = false;
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ABattleGroundPlayer::OnItemCollisionBeginOverlap, TEXT("OnItemCollisionBeginOverlap"));
	GetCapsuleComponent()->OnComponentEndOverlap.__Internal_AddDynamic(this, &ABattleGroundPlayer::OnItemCollisionEndOverlap, TEXT("OnItemCollisionEndOverlap"));

	if(m_CurveVertical != nullptr)
	    m_TimeVertical->AddInterpFloat(m_CurveVertical, m_TimeLineUpdateEvent);
}

void ABattleGroundPlayer::EquipmentSlotInit()
{
	m_ArrayEquipment.Reset(0);
	UDataTable* DT_WeaponData = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Data/EquipmentSlotDataTable.EquipmentSlotDataTable'"));
	TArray<FName> names = DT_WeaponData->GetRowNames();

	for (auto& data : names)
	{
		FEquipmentSlot SlotData = *(DT_WeaponData->FindRow<FEquipmentSlot>(data, data.ToString()));
		m_ArrayEquipment.Add(SlotData);
	}
}

void ABattleGroundPlayer::SetTimerInit()
{

}
float ABattleGroundPlayer::CurrentInventoryWeight()
{
	float CurrentWeight = 0;

	for (auto& Data : m_ArrayInventory)
	{
		CurrentWeight += (Data.m_fWeight * Data.m_iItemAmount);
	}

	return CurrentWeight;
}
bool ABattleGroundPlayer::IsHasItem(FItemData* data)
{
	return false;
}
void ABattleGroundPlayer::OnItemCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IItemInterface* item = Cast< IItemInterface>(OtherActor);

	if (item != nullptr)
	{
		item->OnInteractiveEnter(this);
	}
}
void ABattleGroundPlayer::OnItemCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IItemInterface* item = Cast< IItemInterface>(OtherActor);

	if (item != nullptr)
	{
		item->OnInteractiveExited();
	}
}
// Called every frame
void ABattleGroundPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattleGroundPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	m_PlayerState = PlayerMoveState::State_Ground;
}

// Called to bind functionality to input
void ABattleGroundPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &ABattleGroundPlayer::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ABattleGroundPlayer::Turn);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABattleGroundPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABattleGroundPlayer::MoveRight);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ABattleGroundPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ABattleGroundPlayer::StopJump);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ABattleGroundPlayer::StartCrouch);
	PlayerInputComponent->BindAction("Carmera", EInputEvent::IE_Pressed, this, &ABattleGroundPlayer::CameraChange);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ABattleGroundPlayer::Interact);
}

FVector ABattleGroundPlayer::GetFPCameraTarget_Implementation()
{
	return GetMesh()->GetSocketLocation(m_FPSocketName);
}

FTransform ABattleGroundPlayer::GetTPPivotTaget_Implementation()
{
	return GetActorTransform();
}

FVector ABattleGroundPlayer::GetTPTraceLocation_Implementation()
{
	return GetActorLocation();
}

void ABattleGroundPlayer::GetCameraParameter_Implementation(float& ThirdFOV, float& FirstFOV)
{
	ThirdFOV = m_fThirdFov;
	FirstFOV = m_fFirstFov;
}

FTransform ABattleGroundPlayer::GetCameraTransform_Implementation()
{
	if (m_PlayerViewMode == PlayerViewMode::View_First)
	{
		return GetMesh()->GetSocketTransform(m_FPSocketName);
	}
	else if (m_PlayerViewMode == PlayerViewMode::View_Third)
	{
		return m_TPCamera->GetComponentTransform();
	}

	return FTransform();
}

UCameraComponent* ABattleGroundPlayer::GetCamera_Implementation()
{
	if (m_PlayerViewMode == PlayerViewMode::View_First)
	{
		return m_FPCamera;
	}
	else if (m_PlayerViewMode == PlayerViewMode::View_Third)
	{
		return m_TPCamera;
	}

	return nullptr;
}

void ABattleGroundPlayer::GetPlayerState(PlayerStance& stance, PlayerMoveState& MoveState, PlayerViewMode& ViewMode, PlayerGait& Gait, bool& bisFire, bool& bisAiming)
{
	stance = m_PlayerStance;
	MoveState = m_PlayerState;
	ViewMode = m_PlayerViewMode;
	Gait = m_Gait;
	bisFire = m_bisFire;
	bisAiming = m_bisAmining;
}

void ABattleGroundPlayer::DealDamage()
{
	if (m_bIsIncir && !m_bisDead)
	{
		m_iPlayerHP -= 3;
		if (m_iPlayerHP <= 0)
		{
			m_bisDead = true;
		}
	}
}

void ABattleGroundPlayer::ChangeColor()
{
	if (m_bIsIncir && !m_bisDead)
	{
		m_PostProcess->bEnabled = false;
	}
	else
		m_PostProcess->bEnabled = true;
}

bool ABattleGroundPlayer::InteractiveAddItem(ABaseItem* item)
{
	FItemData ItemData = item->m_ItemInfo;
	bool isReturnSucc = false;
	switch (ItemData.m_Category)
	{
	case EItemCategory::Item_Weapon:
	{
		ABaseWeapone* weapon = Cast<ABaseWeapone>(item);
		if (weapon != NULL)
		{
			FTransform transform;
			transform.SetIdentity();
			FActorSpawnParameters parameter;
			parameter.Template = weapon;
			ABaseWeapone* Temp = Cast<ABaseWeapone>(parameter.Template);
			if (Temp != NULL)
			{
				Temp->m_bisinitdata = false;
				Temp->SetOwner(this);
				ABaseWeapone* SpawnWeapon = Cast<ABaseWeapone>(GetWorld()->SpawnActor(weapon->m_WeaponData.m_Weapon->StaticClass(), &transform, parameter));
				memcpy(&SpawnWeapon->m_WeaponData, &weapon->m_WeaponData, sizeof(FWeaponData));
				memcpy(&SpawnWeapon->m_ItemInfo, &weapon->m_ItemInfo, sizeof(FItemData));
				PickUpWeapon(SpawnWeapon);
				isReturnSucc = true;
			}

		}
	}
	break;
	case EItemCategory::Item_Consymable:
	case EItemCategory::Item_Ammo:
		return ItemToInventory(item);
		break;
	default:
		return false;
		break;
	}

	return isReturnSucc;
}

bool ABattleGroundPlayer::ItemToInventory(ABaseItem* item)
{
	bool bisSucc = false;
	float fCurrentWeight = CurrentInventoryWeight();
	
	if (fCurrentWeight + (item->m_ItemInfo.m_fWeight * item->m_ItemInfo.m_iItemAmount) <= m_fMaxWeight)
	{
		if (item->m_ItemInfo.m_bisStack)
		{
			bool bisInven = false;
			int iIndex = 0;
			GetInvenItem(item->m_ItemInfo.m_ItemName, iIndex, bisInven);
			if (bisInven)
			{
				m_ArrayInventory[iIndex].m_iItemAmount += item->m_ItemInfo.m_iItemAmount;
			}
			else
			{
				m_ArrayInventory.Emplace(item->m_ItemInfo);
			}
		}
		else
		{
			m_ArrayInventory.Emplace(item->m_ItemInfo);
		}

		bisSucc = true;
	}
	return bisSucc;
}

void ABattleGroundPlayer::PickUpWeapon(ABaseWeapone* weapon)
{
	int Index = -1;

	if (GetIsEquipment(EquipmentSlotCategory::Slot_PrimaryWeapon, Index))
	{
		if (GetIsEquipment(EquipmentSlotCategory::Slot_SecondWeapon, Index))
		{
			ABaseWeapone* gun = Cast< ABaseWeapone>(m_ArrayEquipment[Index].m_Item);
			if (gun)
			{
				DropItem(m_ArrayEquipment[Index].m_Item->m_ItemInfo, gun);
				m_ArrayEquipment[Index].m_Item->Destroy();
				PickUpWeapon(weapon);
				return;
			}
		}
		else
		{
			if (Index != -1)
			{
				m_ArrayEquipment[Index].m_ItemName = weapon->m_ItemInfo.m_ItemName;
				m_ArrayEquipment[Index].m_Item = weapon;
				Cast<ABattleGroundPlayerController>(Controller)->GetInventoryWidget()->BuildEquipmentWeapon(EquipmentSlotCategory::Slot_SecondWeapon);
			}
		}
	}
	else
	{
		if (Index != -1)
		{
			m_ArrayEquipment[Index].m_ItemName = weapon->m_ItemInfo.m_ItemName;
			m_ArrayEquipment[Index].m_Item = weapon;
			Cast<ABattleGroundPlayerController>(Controller)->GetInventoryWidget()->BuildEquipmentWeapon(EquipmentSlotCategory::Slot_PrimaryWeapon);
		}
	}

	UpdateGunAttachMent();
}

void ABattleGroundPlayer::LookUp(float AxisValue)
{
	if (AxisValue)
	{
		AddControllerPitchInput(AxisValue);
	}
}

void ABattleGroundPlayer::Turn(float AxisValue)
{
	if (AxisValue)
	{
		AddActorLocalRotation(FRotator(0, AxisValue, 0));
		AddControllerYawInput(AxisValue);
	}
}

void ABattleGroundPlayer::MoveForward(float AxisValue)
{
	m_fMoveForward = AxisValue;
	FVector Dir = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	AddMovementInput(Dir, AxisValue);
}

void ABattleGroundPlayer::MoveRight(float AxisValue)
{
	m_fMoveRight = AxisValue;
	FVector Dir = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	AddMovementInput(Dir, AxisValue);
}

void ABattleGroundPlayer::StartJump()
{
	switch (m_PlayerStance)
	{
	case PlayerStance::Stance_Standing:
	{
		if (m_PlayerState == PlayerMoveState::State_Ground)
		{
			m_PlayerState = PlayerMoveState::State_Air;
			bPressedJump = true;
		}
	}
	break;
	case PlayerStance::Stance_Cruncing:
		UnCrouch();
		break;
	}
}

void ABattleGroundPlayer::StopJump()
{
	bPressedJump = false;
}

void ABattleGroundPlayer::StartCrouch()
{
	if (m_PlayerStance == PlayerStance::Stance_Cruncing)
	{
		m_PlayerStance = PlayerStance::Stance_Standing;
		UnCrouch();
	}
	else if (m_PlayerStance == PlayerStance::Stance_Standing)
	{
		m_PlayerStance = PlayerStance::Stance_Cruncing;
		Crouch();
	}
}

void ABattleGroundPlayer::CameraChange()
{
	if (m_bisFirstPerson)
	{
		m_Arm->TargetArmLength = 300.f;
		m_FPCamera->SetActive(false);
		m_TPCamera->SetActive(true);
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		m_PlayerViewMode = PlayerViewMode::View_Third;
		m_bisFirstPerson = false;
	}
	else
	{
		m_FPCamera->SetActive(true);
		m_TPCamera->SetActive(false);
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		m_bisFirstPerson = true;
		m_PlayerViewMode = PlayerViewMode::View_First;
	}
}

void ABattleGroundPlayer::PlayerReload()
{
	ABaseWeapone* weapon = GetCurrentWeapon();

	if (weapon != nullptr)
	{
		bool isFull = false;
		if (weapon->HaveAmmoInWeapon(isFull))
		{
			if (!isFull)
			{
				int iIndex = -1;
				bool bisInven = false;
				int Cnt = GetInvenItem(weapon->m_WeaponData.m_iBulletID, iIndex, bisInven);

				if (!m_bisReload && bisInven && Cnt > 0 && iIndex != -1)
				{
					m_bisReload = true;
					int iAmmoRest = weapon->m_WeaponData.m_iAmmoMax - weapon->m_WeaponData.m_iAmmoCount;
					if (Cnt > iAmmoRest)
					{
						Cnt -= iAmmoRest;
						weapon->m_WeaponData.m_iAmmoCount = weapon->m_WeaponData.m_iAmmoMax;
						m_ArrayInventory[iIndex].m_iItemAmount = Cnt;
					}
					else
					{
						Cnt = 0;
						weapon->m_WeaponData.m_iAmmoCount += Cnt;
						RemoveInven(iIndex);
					}

					PlayAnimMontage(m_ReloadMotage, weapon->m_WeaponData.m_fWeaponReloadTime);
				}
			}

		}
	}
}

void ABattleGroundPlayer::Interact()
{
	TSet<AActor*> OutActor;
	GetCapsuleComponent()->GetOverlappingActors(OutActor, ABaseItem::StaticClass());

	for (AActor* var : OutActor)
	{
		if (UKismetSystemLibrary::DoesImplementInterface(var, UItemInterface::StaticClass()))
		{
			IItemInterface * ItemInteface = Cast<IItemInterface>(var);

			if (ItemInteface != nullptr)
			{
				ItemInteface->OnInteractive(this);
			}
		}
	}
}

void ABattleGroundPlayer::SprintPress()
{
	if (m_PlayerStance == PlayerStance::Stance_Standing)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		m_Gait = PlayerGait::Gait_Running;
	}
	else if (m_PlayerStance == PlayerStance::Stance_Cruncing)
	{
		GetCharacterMovement()->MaxWalkSpeed = 350.0f;
		m_Gait = PlayerGait::Gait_Running;
	}
}

void ABattleGroundPlayer::SprintUnPress()
{
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
	m_Gait = PlayerGait::Gait_Walking;
}

void ABattleGroundPlayer::FirePress()
{
	if (m_bisAmining && m_bisRifle && !m_bisReload)
	{
		ABaseWeapone* weapon = GetCurrentWeapon();

		if (weapon != nullptr)
		{
			weapon->StartFire();
		}
	}
}

void ABattleGroundPlayer::FireUnPress()
{
	ABaseWeapone* weapon = GetCurrentWeapon();

	if (weapon != nullptr)
	{
		weapon->StopFire();
	}
}

bool ABattleGroundPlayer::GetIsEquipment(EquipmentSlotCategory slot, int& iIndex)
{
	for (int i = 0; i < m_ArrayEquipment.Num(); ++i)
	{
		if (m_ArrayEquipment[i].m_SlotCategory == slot)
		{
			iIndex = i;
			if (m_ArrayEquipment[i].m_Item != nullptr)
				return true;

			break;
		}
	}
	return false;
}

void ABattleGroundPlayer::DropItem(FItemData info, ABaseWeapone* weapon)
{
	FVector TraceLocation;
	UCameraComponent* cam = GetCamer();
	FVector vecWorld = cam->GetComponentLocation();
	FVector vecFoward = cam->GetForwardVector();

	vecFoward *= 100.0f;

	FVector ResultVec = vecWorld + vecFoward;  //캐릭터의 정면위치를 지정한다
	TraceLocation.X = UKismetMathLibrary::RandomFloatInRange(ResultVec.X - 50.0f, ResultVec.X + 50.0f);
	TraceLocation.Y = UKismetMathLibrary::RandomFloatInRange(ResultVec.Y - 50.0f, ResultVec.Y + 50.0f);
	TraceLocation.Z = GetMesh()->GetComponentLocation().Z + 100.0f;

	CheckForGround(ResultVec, TraceLocation);          //땅위에 출력해야 하기 떄문에 충돌 지점을 검사한다.
	FTransform transform = FTransform::Identity;
	transform.SetLocation(TraceLocation);                       //스폰위치를 지정한다....
	ABaseItem* item = GetWorld()->SpawnActor<ABaseItem>(info.m_ItemClass->StaticClass(), transform);

	memcpy(&item->m_ItemInfo, &info, sizeof(FItemData));
	if (info.m_Category == EItemCategory::Item_Weapon)
	{
		weapon = Cast<ABaseWeapone>(item);
	}
}


UCameraComponent* ABattleGroundPlayer::GetCamer()
{
	if (m_bisFirstPerson)
		return m_FPCamera;
	else
		return m_TPCamera;
}

void ABattleGroundPlayer::CameraVerticalIncrease(FMinMaxValue val)
{
	if (!m_TimeVertical->IsPlaying())
	{
		m_MaxMinVertical = val;
		m_TimeVertical->PlayFromStart();
	}
}

void ABattleGroundPlayer::CameraVerticalTLEvent(float fAlpha)
{
	AddControllerYawInput(UKismetMathLibrary::RandomFloatInRange(m_MaxMinVertical.m_fMinValue, m_MaxMinVertical.m_fMaxValue));
	AddControllerPitchInput(UKismetMathLibrary::RandomFloatInRange(-0.1, -0.4));
}

bool ABattleGroundPlayer::CheckForGround(FVector Position, FVector& GetLocation)
{
	FVector StartVec;
	FVector EndVec;

	EndVec.X = UKismetMathLibrary::RandomFloatInRange(Position.X - 50.0f, Position.X + 50.0f);
	EndVec.Y = UKismetMathLibrary::RandomFloatInRange(Position.Y - 50.0f, Position.Y + 50.0f);
	EndVec.Z = 0.0f;

	StartVec.X = UKismetMathLibrary::RandomFloatInRange(EndVec.X - 50.0f, EndVec.X + 50.0f);
	StartVec.Y = UKismetMathLibrary::RandomFloatInRange(EndVec.Y - 50.0f, EndVec.Y + 50.0f);
	StartVec.Z = 1000.0f;

	FHitResult OutResluts;
	bool bisTrace = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartVec, EndVec, 20.0f, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::Type::ForDuration, OutResluts, true);

	if (bisTrace)
		GetLocation = OutResluts.Location;

	return bisTrace;
}

void ABattleGroundPlayer::UpdateGunAttachMent()
{
	m_bisRifle = EquipmentWeapon();

	TArray<EquipmentSlotCategory> slots;
	slots.Add(EquipmentSlotCategory::Slot_PrimaryWeapon);
	slots.Add(EquipmentSlotCategory::Slot_SecondWeapon);
	slots.Add(EquipmentSlotCategory::Slot_PistolWeapon);
	slots.Add(EquipmentSlotCategory::Slot_Meel);

	for (int i = 0; i < slots.Num(); i++)
	{
		if (m_CurrentEquipmentSlot != slots[i])
		{
			InWeapon(slots[i]);
		}
	}
}

bool ABattleGroundPlayer::EquipmentWeapon()
{
	int Index = -1;

	GetIsEquipment(m_CurrentEquipmentSlot, Index);

	if (Index != -1)
	{
		ABaseItem* item = m_ArrayEquipment[Index].m_Item;
		if (item != nullptr)
		{
			if (m_ArrayEquipment[Index].m_ItemCategory == EItemCategory::Item_Pistol || m_ArrayEquipment[Index].m_ItemCategory == EItemCategory::Item_Meel)
			{
				item->GetRootComponent()->SetVisibility(true, true);
			}

			item->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false)
				, WeaponSocketName);   //아이템을 지정된 소켓에 붙인다.....

			return true;
		}
	}

	return false;
}

void ABattleGroundPlayer::InWeapon(EquipmentSlotCategory slot)
{
	int Index = -1;

	GetIsEquipment(slot, Index);

	if (Index != -1)
	{
		ABaseItem* item = m_ArrayEquipment[Index].m_Item;

		if (item != nullptr)
		{
			if (m_ArrayEquipment[Index].m_ItemCategory == EItemCategory::Item_Weapon)
			{
				item->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true)
					, m_ArrayEquipment[Index].m_strSocketName);
			}
			else if (m_ArrayEquipment[Index].m_ItemCategory == EItemCategory::Item_Pistol || m_ArrayEquipment[Index].m_ItemCategory == EItemCategory::Item_Meel)
			{
				item->GetRootComponent()->SetVisibility(false, true);
			}
		}
	}
}

ABaseWeapone* ABattleGroundPlayer::GetCurrentWeapon()
{
	if (m_bisRifle)
	{
		int Index = -1;

		GetIsEquipment(m_CurrentEquipmentSlot, Index);

		ABaseItem* item = m_ArrayEquipment[Index].m_Item;

		return Cast<ABaseWeapone>(item);
	}

	return nullptr;
}

int ABattleGroundPlayer::GetInvenItem(FName iItemName, int& iIndex, bool& bisItem)
{
	int Cnt = 0;
	bisItem = false;
	for (int i = 0; i < m_ArrayInventory.Num(); i++)
	{
		if (m_ArrayInventory[i].m_ItemName.IsEqual(iItemName))
		{
			bisItem = true;
			Cnt = m_ArrayInventory[i].m_iItemAmount;
			iIndex = i;
			break;
		}
	}
	return Cnt;
}

void ABattleGroundPlayer::RemoveInven(int iIndex)
{
	m_ArrayInventory.RemoveAt(iIndex);
}

void ABattleGroundPlayer::GroundItemAdd()
{
	m_ArrGroundItem.Reset(0);
	TArray<FHitResult> ResultHit;
	if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetMesh()->GetComponentLocation(), GetMesh()->GetComponentLocation(),
		350.0f, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::Type::None, ResultHit, true))
	{
		for (FHitResult var : ResultHit)
		{
			ABaseItem* item = Cast<ABaseItem>(var.Actor);

			if (item != nullptr && IsValid(item))
			{
				m_ArrGroundItem.Add(item);
			}
		}
	}
}

void ABattleGroundPlayer::InputModeUI(UWidget* widget, bool bisInput)
{
	APlayerController* con = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	con->SetShowMouseCursor(bisInput);

	if (bisInput)
	{
		FInputModeGameAndUI InputDatabase;
		InputDatabase.SetHideCursorDuringCapture(true);
		InputDatabase.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputDatabase.SetWidgetToFocus(widget->GetCachedWidget());
		con->SetInputMode(InputDatabase);

	}
	else
	{
		FInputModeGameOnly InputDatabase;
		con->SetInputMode(InputDatabase);
	}

	m_bisInWidget = bisInput;
}

