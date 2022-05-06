// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/UMG/Public/Components/Image.h>
#include "InteractiveWidget.generated.h"


/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UInteractiveWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RESOURCE", meta = (BindWidget))
	class UTextBlock * m_Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RESOURCE", meta = (BindWidget))
	UImage* m_Image;
	UInteractiveWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
	UFUNCTION()
		void SetDropIconTex(UTexture2D* IconImage);
};
