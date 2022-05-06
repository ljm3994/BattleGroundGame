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
	Fire_Single UMETA(DisplayName = "�ܹ�"),
	Fire_Triple UMETA(DisplayName = "3����"),
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
		FName m_ItemName;                                  //���̵�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_iItemName;                             //�̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* m_Image;                          // ������ �̹��� �ؽ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* m_Icon;                          //������ �̹���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fWeight;                               //����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemCategory m_Category;                      // ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iItemAmount;                         // ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bisStack;                             // �����ؼ� ��� �ٴ� �� �մ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABaseItem* m_ItemClass;                         //������ Ŭ����
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABaseWeapone* m_Weapon;          //�ش� ������ Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iAmmoCount;                    //�ش� ������ �Ѿ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iAmmoMax;                      //�ִ� ���� ������ �Ѿ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName m_iBulletID;                      //����ϴ� �Ѿ� ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fWeaponReloadTime;             //������ �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fWeaponDelayFire;              //�߻� ������ �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_fWeaponDmg;                  //������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMinMaxValue m_HorizonRebound;         //�¿� �ݵ� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMinMaxValue m_VerticalRebound;        //���� �ݵ� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_BarrelSlot;               //�ѱ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_MagazinSlot;               //źâ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_AimingSlot;               //���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_GunHandleSlot;               //�� �׸� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotState m_BackSlot;               //���Ӹ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EBulletCategory m_BulletCategory;       //����� �Ѿ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFireCategory m_FireCategory;           //�Ѿ� �߻� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* m_Material;                 //����� ���׸��� ����
};
