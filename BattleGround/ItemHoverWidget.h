// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemEnum.h"
#include "ItemHoverWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UItemHoverWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UImage* m_IconImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
		class UTextBlock* m_ItemWeight;

	UItemHoverWidget(const FObjectInitializer& ObjectInitializer);
	void SetHoverData(FItemData Data);
};
