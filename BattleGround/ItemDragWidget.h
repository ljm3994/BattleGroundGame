// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemEnum.h"
#include "ItemDragWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UItemDragWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UImage* m_IconImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_ItemCountText;

	UItemDragWidget(const FObjectInitializer& ObjectInitializer);
	UItemDragWidget* SetDrag(FItemData Data);
};
