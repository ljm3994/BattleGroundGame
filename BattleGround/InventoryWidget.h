// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateEnum.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_ArmorSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_BackSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_ClothSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_EyeSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_GrandingSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_GrandNumImg;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UCanvasPanel* m_GrandSubPannel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_HandSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_HeadSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_HeriSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_JoggiSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_MeelSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_MeelNumImg;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UCanvasPanel* m_MeelWeaponPannel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_PantsSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_ShoesSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_SubWeaponSlotBG;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_SubWeaponSlotBG_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_WeaponSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class USizeBox* m_GroundSizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class USizeBox* m_InventorySizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class USizeBox* m_OneWeaponSizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class USizeBox* m_ThridSizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class USizeBox* m_TwoSizeBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "플레이어", meta = (BindWidget))
		class UPlayerHudWidget* m_player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_InvenListClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_GroundListClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_WeaponListClass;

	class ABattleGroundPlayer* m_OwningPlayer;
	class ABattleGroundPlayerController * m_OwningPlayerController;

	class UInventoryItemWidget* m_InventoryList;
	class UGroundItemWidget* m_GroundList;
	class UWeaponEquipmentWidget* m_FirstWeaponSlot;
	class UWeaponEquipmentWidget* m_SecondWeaponSlot;
	class UWeaponEquipmentWidget* m_ThirdWeaponSlot;

	virtual void NativeConstruct() override;

	void BuildInventory(class ABattleGroundPlayer* player, class ABattleGroundPlayerController * controller);
	UFUNCTION(BlueprintNativeEvent)
		void BuildEquipmentWeapon(const EquipmentSlotCategory& param);
	virtual EquipmentSlotCategory BuildEquipmentWeapon_Implementation(EquipmentSlotCategory& param);
	UFUNCTION(BlueprintNativeEvent)
		void BuildInventory();
	virtual void BuildInventory_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void BuildGroundItem();
	virtual void BuildGroundItem_Implementation();
};
