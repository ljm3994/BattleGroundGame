// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "InteractiveWidget.h"
#include "BattleGroundPlayerController.h"
#include "BattleGroundPlayer.h"
#include "Components/WidgetComponent.h"
#include "InteractiveWidget.h"

// Sets default values
ABaseItem::ABaseItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_bisinitdata = true;
	m_ItemInfo.m_ItemClass = this;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	bisCollision = true;
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::OnInteractiveEnter(ABattleGroundPlayer* Player)
{
	if (m_InteractiveWidget == nullptr)    //위젯이 생성되어있지 않으면 생성하고 컴포넌트를 붙인다....
	{
		ABattleGroundPlayerController* Controller = Cast<ABattleGroundPlayerController>(Player->GetController());
		m_InteractiveWidget = NewObject<UWidgetComponent>(Controller, UWidgetComponent::StaticClass());
		
		UInteractiveWidget* widget = CreateWidget<UInteractiveWidget>(Controller, m_InteractiveWidgetClass);
		widget->SetDropIconTex(m_ItemInfo.m_Image);
		m_InteractiveWidget->RegisterComponent();
		FTransform transform;
		transform.SetLocation(FVector(0.f, 0.f, 200.f));
		transform.SetRotation(FQuat(0.f, 0.f, 0.f, 0.f));
		transform.SetScale3D(FVector(1.f, 1.f, 1.f));
		m_InteractiveWidget->SetRelativeTransform(transform);
		m_InteractiveWidget->SetWidgetSpace(EWidgetSpace::Screen);
		m_InteractiveWidget->SetWidget(widget);
		m_InteractiveWidget->SetDrawAtDesiredSize(true);
		m_InteractiveWidget->SetVisibility(true, true);
		m_InteractiveWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		m_InteractiveWidget->AttachToComponent(RootComponent, rule);
		
	}
}

void ABaseItem::OnInteractiveExited()
{
	if (m_InteractiveWidget != nullptr)
	{
		m_InteractiveWidget->DestroyComponent();  //위젯이 생성되어 있으면 파괴한다.
		m_InteractiveWidget = NULL;
	}
}

void ABaseItem::OnInteractive(ABattleGroundPlayer* Player)
{
	if (Player != NULL)
	{
		bool isSucced = Player->InteractiveAddItem(this);
		if (isSucced)
		{
			Destroy();
		}
	}
}

