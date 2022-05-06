// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemEnum.h"
#include "DragItemOperation.h"
#include "SlotBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API USlotBaseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UImage* m_IconImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_HoverClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_DragVisualClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotCategory SlotType;

	class UItemHoverWidget* m_WidgetHoverRef;
	class UItemDragWidget* m_WidgetDragRef;

	FTimerHandle m_Handler;
	FVector2D m_VecDragOffset;
	int m_iIndex;
	class ABattleGroundPlayer* m_Player;
	class ABattleGroundPlayerController* m_PlayerController;
	FItemData m_Data;
	bool m_bisDrag;
	class ABaseItem* ItemRef;
	USlotBaseWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable, Category = "Setting")
	void SetData(FItemData Data);
	void UpdatePosition();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, UDragDropOperation*& InOperation) override;
};
