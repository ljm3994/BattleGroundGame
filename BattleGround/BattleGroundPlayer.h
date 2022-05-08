// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "PlayerStateEnum.h"
#include "ItemEnum.h"
#include "Utility.h"
#include "CameraInterface.h"
#include "BattleGroundPlayer.generated.h"

class ABaseItem;
class ABaseWeapone;
UCLASS()
class BATTLEGROUND_API ABattleGroundPlayer : public ACharacter, public ICameraInterface
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
		PlayerStance m_PlayerStance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
		PlayerMoveState m_PlayerState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		PlayerViewMode m_PlayerViewMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
		PlayerGait m_Gait;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		bool m_bisFirstPerson;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_TPCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_FPCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* m_Arm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* m_CurveVertical;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool m_bisReload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
		TArray<FEquipmentSlot> m_ArrayEquipment;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
		EquipmentSlotCategory m_CurrentEquipmentSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		FName WeaponSocketName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool m_bisRifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		TArray<FItemData> m_ArrayInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* m_ReloadMotage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TArray<ABaseItem*> m_ArrGroundItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool m_bisInWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float m_fMoveRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float m_fMoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	bool m_bisFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	bool m_bisAmining;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class ARecordingActor* RecordingRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UInteractiveWidget> m_InteractiveClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		class UMainHud * m_MainHud;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		class UPostProcessComponent* m_PostProcess;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		float m_fMaxWeight;

	bool m_bisDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int m_iPlayerHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float m_iHeadPlayerDefece;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float m_iBodyPlayerDefece;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float m_fFirstFov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float m_fThirdFov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		FName m_FPSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* m_TLAimingCurve;

	class UTimelineComponent* m_TimeVertical;
	class UTimelineComponent* m_TimeAimingVertical;

	FOnTimelineFloat m_TimeLineUpdateEvent;
	FOnTimelineFloat m_TimeLineAimingUpdateEvent;
	FMinMaxValue m_MaxMinVertical;
	bool bisInit;
	int m_iDoubleClick;
	FTimerHandle m_DoubleClickHandle;
	bool m_bisDoubleLB;
public:
	bool m_bIsIncir;
	// Sets default values for this character's properties
	ABattleGroundPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	FVector NavPoint;
	bool m_bisMaker;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	FVector GetFPCameraTarget();
	virtual FVector GetFPCameraTarget_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	FTransform GetTPPivotTaget();
	virtual FTransform GetTPPivotTaget_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	FVector GetTPTraceLocation();
	virtual FVector GetTPTraceLocation_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	void GetCameraParameter(float& ThirdFOV, float& FirstFOV);
	virtual void GetCameraParameter_Implementation(float& ThirdFOV, float& FirstFOV);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	FTransform GetCameraTransform();
	virtual FTransform GetCameraTransform_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
	UCameraComponent* GetCamera();
	virtual UCameraComponent* GetCamera_Implementation();
	ARecordingActor* GetRecordingRef() { return RecordingRef; }
	TArray<FItemData>* GetInventory() { return &m_ArrayInventory; } 
	TArray< ABaseItem*>& GetGroundItem() { return m_ArrGroundItem; }
	TArray<FEquipmentSlot>& GetEquipmentArray() { return m_ArrayEquipment; }
	EquipmentSlotCategory GetCurrentEquipmentCategory() { return m_CurrentEquipmentSlot; }
	void GetPlayerState(PlayerStance& stance, PlayerMoveState& MoveState, PlayerViewMode& ViewMode, PlayerGait& Gait, bool& bisFire, bool& bisAiming);
	UFUNCTION()
	float GetMoveRight() { return m_fMoveRight; }
	UFUNCTION()
	float GetMoveForward() { return m_fMoveForward; }
	UFUNCTION()
	PlayerGait GetPlayerGait() {return m_Gait;}
	UFUNCTION()
	bool GetPlayerRifle() {return m_bisRifle;}
	UFUNCTION()
	PlayerStance GetPlayterStance() { return m_PlayerStance; }
	void SetRecordingActor(class ARecordingActor* val) { RecordingRef = val; }
	UFUNCTION()
	bool GetIsFire() { return m_bisFire; }
	void DealDamage();
	void ChangeColor();
	void SetIsRifle(bool val) { m_bisRifle = val; }
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		/// <summary>
		/// 아이템 상호작용 추가
		/// </summary>
		/// <param name="item"> 상호작용할 아이템</param>
		/// <returns></returns>
		bool InteractiveAddItem(ABaseItem* item);
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		/// <summary>
		/// 
		/// </summary>
		/// <param name="item"></param>
		/// <returns></returns>
		bool ItemToInventory(ABaseItem* item);
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		/// <summary>
		/// 무기 줍기
		/// </summary>
		/// <param name="weapon">주울 무기</param>
		void PickUpWeapon(ABaseWeapone* weapon);
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 마우스 위 방향
		/// </summary>
		/// <param name="AxisValue"></param>
		void LookUp(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 마우스 턴
		/// </summary>
		/// <param name="AxisValue"></param>
		void Turn(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 앞으로 이동!!!!
		/// </summary>
		/// <param name="AxisValue"></param>
		void MoveForward(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 오른쪽으로 이동!!!!
		/// </summary>
		/// <param name="AxisValue"></param>
		void MoveRight(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 점프 시작
		/// </summary>
		void StartJump();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 점프 중단
		/// </summary>
		void StopJump();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 앉기 시작
		/// </summary>
		void StartCrouch();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 1인칭 3인칭 변경
		/// </summary>
		void CameraChange();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 무기 재장전
		/// </summary>
		void PlayerReload();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 아이템 상호작용키 입력
		/// </summary>
		void Interact();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 뛰기 버튼
		/// </summary>
		void SprintPress();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 뛰기 버튼 그만
		/// </summary>
		void SprintUnPress();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 발사버튼 입력
		/// </summary>
		void FirePress();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 발사 버튼 중기
		/// </summary>
		void FireUnPress();
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		/// <summary>
		/// 해당 슬롯의 장착아이템 인덱스를 가져온다
		/// </summary>
		/// <param name="slot">가져올 슬롯</param>
		/// <param name="iIndex">반환할 인덱스 번호</param>
		/// <returns>아이템 존재 여부</returns>
		bool GetIsEquipment(EquipmentSlotCategory slot, int& iIndex);
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		/// <summary>
		/// 아이템 땅에 떨구기
		/// </summary>
		/// <param name="info">아이템 정보</param>
		/// <param name="weapon">떨굴 아이템</param>
		void DropItem(FItemData info, ABaseWeapone* weapon, bool bisStack = false, int iStackCnt = 0);

	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 에임 버튼 누르기
		/// </summary>
		void AimingPress();
	UFUNCTION(BlueprintCallable, Category = "Action")
		/// <summary>
		/// 에임 버튼 비활성화 누르기
		/// </summary>
		void AimingUnPress();
	UFUNCTION(BlueprintCallable, Category = "Camera")
		UCameraComponent* GetCamer();
	UFUNCTION(BlueprintCallable, Category = "Camera")
		void CameraVerticalIncrease(FMinMaxValue val);
	UFUNCTION()
		void CameraVerticalTLEvent(float fAlpha);
	UFUNCTION(BlueprintCallable, Category = "Collision")
		bool CheckForGround(FVector Position, FVector& GetLocation);
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void UpdateGunAttachMent();
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		bool EquipmentWeapon();
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void InWeapon(EquipmentSlotCategory slot);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		ABaseWeapone* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		int GetInvenItem(FName iItemID, int& iIndex, bool& bisItem);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void RemoveInven(int iIndex);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void GroundItemAdd();
	UFUNCTION(BlueprintCallable, Category = "UI")
		void InputModeUI(class UWidget* widget, bool bisInput = true);
	UFUNCTION(BlueprintCallable, Category = "Init")
		void EquipmentSlotInit();
	UFUNCTION(BlueprintCallable, Category = "Init")
		void SetTimerInit();
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void SelectSlotEquipment(EquipmentSlotCategory slot, FItemData& itemData);
	UFUNCTION()
		void AimingVerticalTLEvent(float fAlpha);

	int GetIsEquipmentSlot(class ABaseItem* item);
	float CurrentInventoryWeight();
	bool IsHasItem(FItemData* data);
	UFUNCTION()
	void OnItemCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnItemCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
