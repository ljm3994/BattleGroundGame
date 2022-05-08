// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotBaseWidget.h"
#include <BattleGround/PlayerStateEnum.h>
#include "WeaponEquipmentWidget.generated.h"


/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UWeaponEquipmentWidget : public USlotBaseWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_ItemNumText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_BulletCountNum;

	EquipmentSlotCategory m_Category;

	UWeaponEquipmentWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void BuildWeapon();
};
