// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ÀÚ½ÄÀ§Á¬", meta = (BindWidget))
	class UImage* m_MapImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		float m_fDemisions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		float m_fZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	class UTexture* m_RendtargetImage;
	class UMaterialParameterCollection* m_MatParam;
	class UMaterialInstanceDynamic* m_MapMat;
	class ABattleGroundPlayer* m_Player;

	UMiniMapWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
};
