// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class UTextBlock* m_TimerText;
	
	int m_iFullTimer;
	int m_iCurrentTimer;
	FTimerHandle m_DelayHandle;
	class UMainHud* Owner;
	class ABlueZone* m_blueZone;
	UTimerWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void SetTimer();
	void StartTimer();
};
