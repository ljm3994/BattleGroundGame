// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemEnum.generated.h"

class ABaseItem;
class ABaseWeapone;
class UTexture2D;

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Item_None UMETA(DisplayName = "ItemNone"),
	Item_Upper UMETA(DisplayName = "ItemUpper"),
	Item_Lower UMETA(DisplayName = "ItemLower"),
	Item_Head UMETA(DisplayName = "ItemHead"),
	Item_Feet UMETA(DisplayName = "ItemFeet"),
	Item_Hands UMETA(DisplayName = "ItemHand"),
	Item_Weapon UMETA(DisplayName = "ItemWeapon"),
	Item_Pistol UMETA(DisplayName = "ItemPistol"),
	Item_Meel UMETA(DisplayName = "ItemMeel"),
	Item_Grand UMETA(DisplayName = "ItemGrand"),
	Item_Back UMETA(DisplayName = "ItemBack"),
	Item_Consymable UMETA(DisplayName = "ItemConsumable"),
	Item_Ammo UMETA(DisplayName = "ItemAmmo"),
};

UENUM(BlueprintType)
enum class ESlotState : uint8
{
	Slot_Open UMETA(DisplayName = "Open"),
	Slot_Close UMETA(DisplayName = "Close"),
};

UENUM(BlueprintType)
enum class EBulletCategory : uint8
{
	Bullet_NONE UMETA(DisplayName = "None"),
	Bullet_5 UMETA(DisplayName = "5.56mm"),
	Bullet_7 UMETA(DisplayName = "7mm"),
	Bullet_4 UMETA(DisplayName = "4.56"),
};

UENUM(BlueprintType)
enum class EFireCategory : uint8
{
	Fire_Single UMETA(DisplayName = "단발"),
	Fire_Triple UMETA(DisplayName = "3점사"),
	Fire_Auto UMETA(DisplayName = "Auto"),
};

USTRUCT(BlueprintType)
struct FMinMaxValue
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fMinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fMaxValue;

	void Reset()
	{
		m_fMinValue = 0;
		m_fMaxValue = 0;
	}
};

USTRUCT(BlueprintType)
struct FWeaponSlotInfo
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fMinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fMaxValue;
};

USTRUCT(BlueprintType)
struct FBulletInfo
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fMaxValue;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
public:
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_ItemName;                                  //아이디
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_iItemName;                             //이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* m_Image;                          // 아이콘 이미지 텍스쳐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* m_Icon;                          //아이콘 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fWeight;                               //무게
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemCategory m_Category;                      // 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iItemAmount;                         // 갯수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bisStack;                             // 누적해서 들고 다닐 수 잇는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABaseItem* m_ItemClass;                         //아이템 클래스
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABaseWeapone* m_Weapon;          //해당 무기의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iAmmoCount;                    //해당 무기의 총알 갯수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iAmmoMax;                      //최대 장전 가능한 총알 갯수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_iBulletID;                      //사용하는 총알 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fWeaponReloadTime;             //재장전 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fWeaponDelayFire;              //발사 딜레이 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fWeaponDmg;                  //데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMinMaxValue m_HorizonRebound;         //좌우 반동 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMinMaxValue m_VerticalRebound;        //상하 반동 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_BarrelSlot;               //총구 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_MagazinSlot;               //탄창 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_AimingSlot;               //에임 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_GunHandleSlot;               //총 그립 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_BackSlot;               //개머리판 슬롯
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EBulletCategory m_BulletCategory;       //사용할 총알 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFireCategory m_FireCategory;           //총알 발사 모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* m_Material;                 //사용할 마테리얼 정보
};
