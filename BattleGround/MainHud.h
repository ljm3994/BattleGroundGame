// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHud.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API UMainHud : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UCanvasPanel* m_WeaponPannel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UCanvasPanel* m_CrossPannel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_WeaponImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_CrossTImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_CrossLImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_CrossBImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_CrossRImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_CompassBackImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_CompassPointerImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_CompassMakerImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "자식위젯", meta = (BindWidget))
		class UImage* m_CompassArrawImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class USizeBox* m_MapSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class USizeBox* m_TimerSizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class USizeBox* m_NotifySizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class UTextBlock * m_NotifyText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class UTextBlock * m_WeaponFireTypeText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class UTextBlock* m_CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "컨테이너", meta = (BindWidget))
		class UTextBlock* m_MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "위젯클래스")
	TSubclassOf<UUserWidget> m_WidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "위젯클래스")
		TSubclassOf<UUserWidget> m_TimerClass;

	class ABattleGroundPlayer* m_Player;
	class UCameraComponent* m_Camera;
	class UMaterialInstanceDynamic* m_PointerMat;
	class UMaterialInstanceDynamic* m_MakerMat;
	FTimerHandle m_DirectionTimerHandle;
	FTimerHandle m_MakerTimerHandle;
	UMainHud(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
	UFUNCTION(BlueprintNativeEvent)
	void NotifyEvent(int CurrentTime);
	virtual void NotifyEvent_Implementation(int CurrentTime);
	void BuildHud(class ABattleGroundPlayer* player);
	void HiddenText();
	void SetDirection();
	void SetMaker();
	bool isCheckNavPoint(float& fPoint);
};
