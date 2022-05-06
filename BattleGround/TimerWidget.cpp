// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerWidget.h"
#include "MainHud.h"
#include "BlueZone.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TextBlock.h"

UTimerWidget::UTimerWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartTimer();
}

void UTimerWidget::SetTimer()
{
	m_iCurrentTimer--;
	m_TimerText->SetText(FText::Format(NSLOCTEXT("Timer", "Timer", "{0} 분 {1} 초"), m_iCurrentTimer / 60, m_iCurrentTimer % 60));
	
	if (m_iCurrentTimer % 30 == 0 || m_iCurrentTimer % 30 == 10)
	{
		Owner->NotifyEvent(m_iCurrentTimer);
	}
	
	if (m_iCurrentTimer <= 0)
	{
		if (IsValid(m_blueZone))
		{
			GetWorld()->GetTimerManager().ClearTimer(m_DelayHandle);
			m_blueZone->StageStart();
		}
		else
		{
			FTransform transform = UKismetMathLibrary::MakeTransform(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(150.0f, 150.0f, 150.0f));
			m_blueZone = GetWorld()->SpawnActor<ABlueZone>(ABlueZone::StaticClass(), transform);

			m_blueZone->m_Timer = this;
			GetWorld()->GetTimerManager().ClearTimer(m_DelayHandle);
			m_blueZone->StageStart();
		}
	}
}

void UTimerWidget::StartTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(m_DelayHandle);
	m_iCurrentTimer = m_iFullTimer;
	GetWorld()->GetTimerManager().SetTimer(m_DelayHandle, FTimerDelegate::CreateLambda([&]() {
		SetTimer();
		}), 1.0f, true);
}
