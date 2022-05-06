// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GroundItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UGroundItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UScrollBox* m_ScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf< UUserWidget> m_SlotWidget;
	class ABattleGroundPlayer* m_Player;
	class ABattleGroundPlayerController* m_PlayerController;

	UGroundItemWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void BuildGround();
};
