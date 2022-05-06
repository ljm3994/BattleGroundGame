// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "ItemEnum.h"
#include "BaseItem.generated.h"

UCLASS(config = Game, BlueprintType)
class BATTLEGROUND_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	bool m_bisinitdata;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
	FItemData m_ItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> m_InteractiveWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	class UWidgetComponent* m_InteractiveWidget;
	// Sets default values for this actor's properties
	ABaseItem(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteractiveEnter(class ABattleGroundPlayer* Player) override;
	virtual void OnInteractiveExited() override;
	virtual void OnInteractive(class ABattleGroundPlayer* Player) override;

};
