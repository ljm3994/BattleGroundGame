// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecordingActor.generated.h"

UCLASS(config = Game, BlueprintType)
class BATTLEGROUND_API ARecordingActor : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* mesh;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* m_Cam;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* m_CaptureComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool m_bisRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FVector2D m_vecStartLoc;
public:	
	// Sets default values for this actor's properties
	ARecordingActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void StartClick(FVector2D Vec);
	virtual void StartClick_Implementation(FVector2D Vec);
	UFUNCTION(BlueprintNativeEvent)
	void EndClick();
	virtual void EndClick_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void MouseMove(FVector2D Vec);
	virtual void MouseMove_Implementation(FVector2D Vec);
};
