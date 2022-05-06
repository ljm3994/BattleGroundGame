// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Components/TimelineComponent.h"
#include "Utility.h"
#include "CameraInterface.h"
#include "BaseCar.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGROUND_API ABaseCar : public AWheeledVehicle, public ICameraInterface
{
	GENERATED_BODY()
private:
	// 라이트 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		int m_iBreakLight; //브레이크 라이트 아이디 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		int m_iReversBreakLightID;       //후면 브레이크 라이트 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		int m_iRearPositionLightID;      // 후면 라이트 아이디 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		int m_iFrontPositionLightID;    //전면 라이트 아이디 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* m_MatBreakLightOn;      //브레이크 온
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* m_MatBreakLigtOff;     // 브레이크 오프
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* m_MatReversBreakLightOn;  //리버스온
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* m_MatReversBreakLightOff;  //리버스 오프
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* m_MatRearPositionLightOn;  //리어 온
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* m_MatRearPositionLightOff; //리어 오프
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* m_MatFrontPositionLightOn;  //프론트 온
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* m_MatFrontPositionLightOff;  // 프론트 오프
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light", meta = (AllowPrivateAccess = "true"))
		bool m_bisPostionLight;    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_SpotRearLightL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_SpotRearLightR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_SpotFrontLightL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_SpotFrontLightR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_SpotReverseLightR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_SpotReverseLightL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_SpotBreakLightR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_SpotBreakLightL;
	// 충돌 관련 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impact", meta = (AllowPrivateAccess = "true"))
		float m_fImpact;          
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact", meta = (AllowPrivateAccess = "true"))
		float m_fminImpact;    //최소
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact", meta = (AllowPrivateAccess = "true"))
		float m_fmedImpact;    //중간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact", meta = (AllowPrivateAccess = "true"))
		float m_fhighImpact;   //최대
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact", meta = (AllowPrivateAccess = "true"))
		class AActor* m_HitActor;         //충돌한 액터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impact", meta = (AllowPrivateAccess = "true"))
		FVector m_vecHitNormalImpuluse;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impact", meta = (AllowPrivateAccess = "true"))
		FHitResult m_HitResult;
	// 휠 관련 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel", meta = (AllowPrivateAccess = "true"))
		FVector m_vecLHandlerPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel", meta = (AllowPrivateAccess = "true"))
		FRotator m_rotLHandler;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel", meta = (AllowPrivateAccess = "true"))
		FVector m_vecRHandlerPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel", meta = (AllowPrivateAccess = "true"))
		FRotator m_rotRHandler;
	// 소리 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* m_EngineSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundBase* m_StartSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundBase* m_ImpactSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundWave* m_DoorSound;
	// 파티클 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paticle", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* m_ParticleSmokeL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paticle", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* m_ParticleSmokeR;
	// 메시 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshSpeedMaker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshRpmMaker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshRightCaliperRear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshLeftCaliperRear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshLeftCaliperFront;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshWheelHandlerR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshWheelHandlerL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshStreeringWheel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_MeshRightCaliperFront;
	// 카메라 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* m_CamInCar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* m_CamRear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* m_SpringArm;
	//입력 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
		float m_fBaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
		float m_fBaseLookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
		float m_fThrottle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
		float m_fLookUpInputMouse;
	// 데이터 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		FText m_CarName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
		class ABattleGroundPlayerController* m_ControllerCaracter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
		int m_iCurrentGear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		float m_fMaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		class UCarAnimInstance* m_AnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* m_InteracetWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* m_SkeltalDriver;
	//시간 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* m_TLOpenDoorCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* m_TLCloseDoorCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "위젯클래스", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> m_InteractWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float m_fFirstFov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float m_fThirdFov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		FName m_FPSocketName;
	FOnTimelineFloat m_TimeLineUpdateEvent;
	FOnTimelineEvent m_TimeLineFinisihEvent;
	FOnTimelineEvent m_TimeLineCloseFinisihEvent;
	FTimerHandle m_DelayHandle;
	UTimelineComponent* m_TimeCloseDoor;
	UTimelineComponent* m_TimeOpenDoor;

	FDoOnce m_DoOnce;
	FDoOnce m_SmokeDoOnce;
	FDoOnce m_StillSmokeDoOnce;
	FDoOnce m_LightDoOnce;
	FGate m_Gate;
	FFlipFlop m_LightFlip;
	FFlipFlop m_CameraFlip;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// 콜리전 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* m_CollisionRightDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* m_CollisionLeftDoor;
	ABaseCar(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent)
		void EnterExitCar();
	virtual void EnterExitCar_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void UnStopCar();
	virtual void UnStopCar_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void StopCar();
	virtual void StopCar_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void ResetDoOnceImpactSound();
	virtual void ResetDoOnceImpactSound_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void ResetDoOnceSmoke();
	virtual void ResetDoOnceSmoke_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void ResetDoOnceStillSmoke();
	virtual void ResetDoOnceStillSmoke_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void TurnBrakeRearLightOff();
	virtual void TurnBrakeRearLightOff_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
		FVector GetFPCameraTarget();
	virtual FVector GetFPCameraTarget_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
		FTransform GetTPPivotTaget();
	virtual FTransform GetTPPivotTaget_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
		FVector GetTPTraceLocation();
	virtual FVector GetTPTraceLocation_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
		void GetCameraParameter(float& ThirdFOV, float& FirstFOV);
	virtual void GetCameraParameter_Implementation(float& ThirdFOV, float& FirstFOV);
	UFUNCTION(BlueprintCallable, Category = "Action")
		void MoveForward(float AxisValue);
	UFUNCTION()
		void DoorOpenTimeEvent(float fAlpha);
	UFUNCTION()
		void DoorOpenFinishEvent();
	UFUNCTION()
		void CarCloseDoorAction();
	UFUNCTION()
		void CarDoorCloseFinish();
	UFUNCTION()
	void OnLeftCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnLeftCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnRightCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnRightCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// 문 근처에 플레이어가 충돌 할 경우 발생하는 이벤트
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Door")
		void CheckCharOnDoorArea(class AActor* object, bool isCanEnterCar, bool isNearLeftDoor);
	/// <summary>
	/// 메시 충돌 이벤트 함수
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Mesh")
		void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	/// <summary>
	/// 빙의 시 이벤트
	/// </summary>
	/// <param name="NewController">빙의할 컨트롤러</param>
	virtual void PossessedBy(AController* NewController) override;
	/// <summary>
	/// 빙의 해제시 이벤트
	/// </summary>
	virtual void UnPossessed() override;
	/// <summary>
	/// 마커 회전 값 세팅
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Mesh")
		void SetMarkerRotation(USceneComponent* Maker, float fRotation, float fRotMul);

	UFUNCTION(BlueprintCallable, Category = "Action")
		void CarSmoke();      //차량의 연기를 만든다
	UFUNCTION(BlueprintCallable, Category = "Action")
		void TurnOnOff();           // 라이트 켜고 끄기
		/// <summary>
		/// 라이트 메테리얼 세팅 함수
		/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Action")
		void SetLight(bool bisPositionLight, bool bisBrakeLight, bool bisReverseLight);
	UFUNCTION(BlueprintCallable, Category = "Action")
		void MoveRight(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Action")
		void CaliperRotation(USceneComponent* Caliper, FName SocketName, bool bisLeftSide, bool bisFront);
	UFUNCTION(BlueprintCallable, Category = "Action")
		void CarBreakPressed();
	UFUNCTION(BlueprintCallable, Category = "Action")
		void CarBreakUnPressed();
	UFUNCTION(BlueprintCallable, Category = "Action")
		void CarLightPressed();
	UFUNCTION(BlueprintCallable, Category = "Camera")
		void CameraPressed();
	UFUNCTION(BlueprintCallable, Category = "Action")
		void MouseTurn(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Action")
		void MouseLookUp(float AxisValue);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
		FTransform GetCameraTransform();
	virtual FTransform GetCameraTransform_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera")
		UCameraComponent* GetCamera();
	virtual UCameraComponent* GetCamera_Implementation();
};
