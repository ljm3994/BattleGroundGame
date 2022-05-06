// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UPlayerHudWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	class ARecordingActor* m_RecordingRef;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "플레이어", meta = (BindWidget))
	class UImage* m_player;
	UPlayerHudWidget(const FObjectInitializer& ObjectInitializer);
	void SetRecording(class ARecordingActor* val) { m_RecordingRef = val; }
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
};
