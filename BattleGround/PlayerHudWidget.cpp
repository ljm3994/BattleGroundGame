// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHudWidget.h"
#include "RecordingActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "BattleGroundPlayer.h"
#include "BattleGroundPlayerController.h"
UPlayerHudWidget::UPlayerHudWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UPlayerHudWidget::NativeConstruct()
{
	ABattleGroundPlayer * player = Cast< ABattleGroundPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (player != nullptr)
		m_RecordingRef = player->GetRecordingRef();

	Super::NativeConstruct();
}

FReply UPlayerHudWidget::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D MousePos;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePos.X, MousePos.Y);

	m_RecordingRef->StartClick(MousePos);

	return Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply UPlayerHudWidget::NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	m_RecordingRef->EndClick();

	return Super::NativeOnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply UPlayerHudWidget::NativeOnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D MousePos;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UGameplayStatics::GetPlayerController(GetWorld(), 0), MousePos.X, MousePos.Y);

	m_RecordingRef->MouseMove(MousePos);

	return Super::NativeOnMouseMove(MyGeometry, MouseEvent);
}
