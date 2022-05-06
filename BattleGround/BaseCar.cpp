// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCar.h"
#include "Materials/MaterialInterface.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SpotLightComponent.h"
#include "Internationalization/Text.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "WheeledVehicleMovementComponent.h"
#include "VehicleAnimInstance.h"
#include "BattleGroundPlayerController.h"
#include "BattleGroundPlayer.h"
#include "CarAnimInstance.h"
#include "InteractiveWidget.h"
#include "Camera/CameraComponent.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TextBlock.h"

void ABaseCar::BeginPlay()
{
	GetWorld()->GetTimerManager().SetTimer(m_DelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			m_SkeltalDriver->SetHiddenInGame(true, false);
			m_ControllerCaracter = Cast<ABattleGroundPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			GetWorld()->GetTimerManager().ClearTimer(m_DelayHandle);
		}), 0.05f, false);

	m_TimeOpenDoor->AddInterpFloat(m_TLOpenDoorCurve, m_TimeLineUpdateEvent);
	m_TimeCloseDoor->AddInterpFloat(m_TLCloseDoorCurve, m_TimeLineUpdateEvent);
	m_TimeOpenDoor->SetTimelineFinishedFunc(m_TimeLineFinisihEvent);
	m_TimeCloseDoor->SetTimelineFinishedFunc(m_TimeLineCloseFinisihEvent);
	m_CollisionLeftDoor->SetGenerateOverlapEvents(true);
	m_CollisionRightDoor->SetGenerateOverlapEvents(true);

	//이벤트 델리게이트 함수 설정
	m_CollisionLeftDoor->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ABaseCar::OnLeftCollisionBeginOverlap, TEXT("OnLeftCollisionBeginOverlap"));
	m_CollisionLeftDoor->OnComponentEndOverlap.__Internal_AddDynamic(this, &ABaseCar::OnLeftCollisionEndOverlap, TEXT("OnLeftCollisionEndOverlap"));
	m_CollisionRightDoor->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ABaseCar::OnRightCollisionBeginOverlap, TEXT("OnRightCollisionBeginOverlap"));
	m_CollisionRightDoor->OnComponentEndOverlap.__Internal_AddDynamic(this, &ABaseCar::OnRightCollisionEndOverlap, TEXT("OnRightCollisionEndOverlap"));
	GetMesh()->OnComponentHit.__Internal_AddDynamic(this, &ABaseCar::OnMeshHit, TEXT("OnMeshHit"));
	if (m_DoorSound == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<USoundWave> DoorTempSound(TEXT("Game/Car/Sound/cues/general/door_Cue"));
		m_DoorSound = DoorTempSound.Object;
	}

	m_DoOnce.Reset();
	m_SmokeDoOnce.Reset();
	m_StillSmokeDoOnce.Reset();
	m_LightDoOnce.Reset();
}

ABaseCar::ABaseCar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_iBreakLight = 12;
	m_iReversBreakLightID = 10;
	m_iRearPositionLightID = 11;
	m_iFrontPositionLightID = 0;
	m_bisPostionLight = false;
	m_fImpact = 0.f;
	m_fminImpact = 10.f;
	m_fmedImpact = 100.f;
	m_fhighImpact = 1000.f;

	m_MatBreakLightOn = nullptr;
	m_MatBreakLigtOff = nullptr;
	m_MatReversBreakLightOn = nullptr;
	m_MatReversBreakLightOff = nullptr;
	m_MatRearPositionLightOn = nullptr;
	m_MatRearPositionLightOff = nullptr;
	m_MatFrontPositionLightOn = nullptr;
	m_MatFrontPositionLightOff = nullptr;
	m_HitActor = nullptr;
	m_vecHitNormalImpuluse = FVector(0.f, 0.f, 0.f);
	m_HitResult = FHitResult();
	m_vecLHandlerPos = FVector(0.f, 0.f, 0.f);
	m_rotLHandler = FRotator(0.0f, 0.0f, 0.f);
	m_vecRHandlerPos = FVector(0.f, 0.f, 0.f);
	m_rotRHandler = FRotator(0.0f, 0.0f, 0.f);
	m_EngineSound = nullptr;
	m_StartSound = nullptr;
	m_ImpactSound = nullptr;
	m_MeshRpmMaker = nullptr;
	m_MeshSpeedMaker = nullptr;
	m_ParticleSmokeL = nullptr;
	m_ParticleSmokeR = nullptr;
	m_SpotRearLightL = nullptr;
	m_SpotRearLightR = nullptr;
	m_SpotFrontLightL = nullptr;
	m_SpotFrontLightR = nullptr;
	// Cliper 초기화
	m_MeshLeftCaliperFront = nullptr;
	m_MeshLeftCaliperRear = nullptr;
	m_MeshRightCaliperFront = nullptr;
	m_MeshRightCaliperRear = nullptr;
	//Handler 초기화
	m_MeshWheelHandlerR = nullptr;
	m_MeshWheelHandlerL = nullptr;
	m_MeshStreeringWheel = nullptr;
	// 문 충돌 박스
	m_CollisionRightDoor = nullptr;
	m_CollisionLeftDoor = nullptr;
	// 등
	m_SpotReverseLightR = nullptr;
	m_SpotReverseLightL = nullptr;
	m_SpotBreakLightR = nullptr;
	m_SpotBreakLightL = nullptr;
	// 드라이버
	m_SkeltalDriver = nullptr;
	m_CamInCar = nullptr;
	m_CamRear = nullptr;
	m_SpringArm = nullptr;
	m_CarName = FText();
	m_ControllerCaracter = nullptr;
	m_iCurrentGear = 0;
	m_fMaxSpeed = 3500.f;
	m_AnimInstance = nullptr;
	m_InteracetWidget = nullptr;
	m_fBaseTurnRate = 0.f;
	m_fBaseLookUpRate = 0.f;
	m_fThrottle = 0.f;
	m_fLookUpInputMouse = 0.f;
	m_TimeOpenDoor = CreateDefaultSubobject<UTimelineComponent>(TEXT("OpenDoorTL"));
	m_TimeCloseDoor = CreateDefaultSubobject<UTimelineComponent>(TEXT("CloseDoorTL"));
	m_TimeLineUpdateEvent.BindUFunction(this, TEXT("DoorOpenTimeEvent"));
	m_TimeLineFinisihEvent.BindUFunction(this, TEXT("DoorOpenFinishEvent"));
	m_TimeLineCloseFinisihEvent.BindUFunction(this, TEXT("CarDoorCloseFinish"));
	m_DoorSound = nullptr;
	m_TLOpenDoorCurve = nullptr;
	m_DoOnce = FDoOnce(false);
	m_SmokeDoOnce = FDoOnce(false);
	m_StillSmokeDoOnce = FDoOnce(false);
	m_LightDoOnce = FDoOnce(false);
	m_Gate = FGate(false);
	m_LightFlip = FFlipFlop(false);
	m_CameraFlip = FFlipFlop(false);
	m_FPSocketName = "FP_Camera";
	m_fFirstFov = 90.0f;
	m_fThirdFov = 90.0f;
}

void ABaseCar::EnterExitCar_Implementation()
{
	m_AnimInstance = Cast<UCarAnimInstance>(GetMesh()->GetAnimInstance());

	UGameplayStatics::PlaySound2D(this, m_DoorSound);
	if (m_TLOpenDoorCurve != nullptr)
	{
		m_TimeOpenDoor->SetFloatCurve(m_TLOpenDoorCurve, TEXT("OpenCurve"));
		m_TimeOpenDoor->PlayFromStart();
	}
}
/// <summary>
/// 차량이 멈추지 않았더
/// </summary>
void ABaseCar::UnStopCar_Implementation()
{
	m_Gate.Open();
}
/// <summary>
/// 차량이 정지했다
/// </summary>
void ABaseCar::StopCar_Implementation()
{
	GetVehicleMovement()->SetThrottleInput(0.f);
	m_Gate.Closed();
}
/// <summary>
/// 임팩트 사운드가 정지했기에 리셋해준다
/// </summary>
void ABaseCar::ResetDoOnceImpactSound_Implementation()
{
	static FTimerHandle DelayHandle;

	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			m_DoOnce.Reset();
			GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
		}), 0.666f, false);
}

void ABaseCar::ResetDoOnceSmoke_Implementation()
{
	m_SmokeDoOnce.Reset();
}

void ABaseCar::ResetDoOnceStillSmoke_Implementation()
{
	m_StillSmokeDoOnce.Reset();
}

void ABaseCar::TurnBrakeRearLightOff_Implementation()
{
	if (m_LightDoOnce.Execute())
	{
		SetLight(m_bisPostionLight, false, false);
	}
}
FVector ABaseCar::GetFPCameraTarget_Implementation()
{
	return GetMesh()->GetSocketLocation(m_FPSocketName);
}
FTransform ABaseCar::GetTPPivotTaget_Implementation()
{
	return GetActorTransform();
}
FVector ABaseCar::GetTPTraceLocation_Implementation()
{
	return GetActorLocation();
}
void ABaseCar::GetCameraParameter_Implementation(float& ThirdFOV, float& FirstFOV)
{
	ThirdFOV = m_fThirdFov;
	FirstFOV = m_fFirstFov;
}
/// <summary>
/// 앞으로 이동!!!!!!
/// </summary>
/// <param name="AxisValue"></param>
void ABaseCar::MoveForward(float AxisValue)
{
	m_fThrottle = AxisValue;

	if (m_Gate.Execute())
	{
		GetVehicleMovement()->SetThrottleInput(m_fThrottle);
		float fSpeed = UKismetMathLibrary::Abs(GetVehicleMovement()->GetEngineRotationSpeed());  //회전속도의 절대값을 가져온다
		// 엔진 사운드 
		m_EngineSound->SetFloatParameter(TEXT("RPM"), fSpeed);

		// RPM
		float fRotation = fSpeed / GetVehicleMovement()->GetEngineMaxRotationSpeed();
		SetMarkerRotation(m_MeshRpmMaker, fRotation, 250.0f);

		float fForward = (UKismetMathLibrary::Abs(GetVehicleMovement()->GetForwardSpeed()) / m_fMaxSpeed);
		SetMarkerRotation(m_MeshSpeedMaker, fForward, 250.0f);
		CarSmoke();
		TurnOnOff();
	}
}

void ABaseCar::DoorOpenTimeEvent(float fAlpha)
{
	if (m_ControllerCaracter->GetIsLeftDoor())
	{
		m_AnimInstance->SetLeftDoorAlpha(fAlpha);
	}
	else
	{
		m_AnimInstance->SetRightDoorAlpha(fAlpha);
	}
}
/// <summary>
/// 문을 다열었다
/// </summary>
void ABaseCar::DoorOpenFinishEvent()
{
	if (m_ControllerCaracter->GetIsEnterCar())
	{
		UnStopCar();
		m_ControllerCaracter->PossesCar();
		UGameplayStatics::PlaySound2D(this, m_StartSound);

		GetWorld()->GetTimerManager().SetTimer(m_DelayHandle, FTimerDelegate::CreateLambda([&]()
			{
				m_EngineSound->Activate(false);
				m_ParticleSmokeL->SetVisibility(true, false);
				m_ParticleSmokeR->SetVisibility(true, false);
				CarCloseDoorAction();
				GetWorld()->GetTimerManager().ClearTimer(m_DelayHandle);
			}), 0.25f, false);
	}
	else
	{
		StopCar();
		m_ControllerCaracter->PossesPawn();
		m_EngineSound->Deactivate();

		m_ParticleSmokeL->SetVisibility(false, false);
		m_ParticleSmokeR->SetVisibility(false, false);

		CarCloseDoorAction();
	}
}
/// <summary>
/// 문을 닫는 액션을 취해야겠군
/// </summary>
void ABaseCar::CarCloseDoorAction()
{
	UGameplayStatics::PlaySound2D(this, m_DoorSound);

	if (m_TLCloseDoorCurve != nullptr)
	{
		m_TimeCloseDoor->SetFloatCurve(m_TLCloseDoorCurve, TEXT("CloseCurve"));
		m_TimeCloseDoor->PlayFromStart();
	}
}
/// <summary>
/// 문 닫는 액션 끝
/// </summary>
void ABaseCar::CarDoorCloseFinish()
{
	m_ControllerCaracter->SetIsLeftDoor(true);
}

void ABaseCar::OnLeftCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCharOnDoorArea(OtherActor, true, true);
}

void ABaseCar::OnLeftCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCharOnDoorArea(OtherActor, false, true);
}

void ABaseCar::OnRightCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCharOnDoorArea(OtherActor, true, false);
}

void ABaseCar::OnRightCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCharOnDoorArea(OtherActor, false, false);
}
/// <summary>
/// 문의 충돌 범위에 부딪혔다
/// </summary>
/// <param name="object">부딫힌 인간</param>
/// <param name="isCanEnterCar">차량 탑승이 가능한지 여부</param>
/// <param name="isNearLeftDoor">왼쪽 문 근처인지 여부</param>
void ABaseCar::CheckCharOnDoorArea(AActor* object, bool isCanEnterCar, bool isNearLeftDoor)
{
	if (isCanEnterCar)
	{
		if (m_InteracetWidget == nullptr)
		{
			m_InteracetWidget = CreateDefaultSubobject<UWidgetComponent>("CarWidget");
			UInteractiveWidget* widget;
			widget = CreateWidget<UInteractiveWidget>(m_ControllerCaracter, m_InteractWidgetClass);

			widget->m_Text->SetText(FText::Format(NSLOCTEXT("Enter", "Enter", "{0} Enter"), m_CarName));
			widget->m_Image->SetVisibility(ESlateVisibility::Collapsed);
			m_InteracetWidget->RegisterComponent();
			FTransform transform;
			transform.SetLocation(FVector(0.f, 0.f, 0.f));
			transform.SetRotation(FQuat(0.f, 0.f, 0.f, 0.f));
			transform.SetScale3D(FVector(1.f, 1.f, 1.f));
			m_InteracetWidget->SetWidgetSpace(EWidgetSpace::Screen);
			m_InteracetWidget->SetRelativeTransform(transform);
			m_InteracetWidget->SetWidget(widget);
			m_InteracetWidget->SetDrawAtDesiredSize(true);
			m_InteracetWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			m_InteracetWidget->AttachToComponent(GetMesh(), rule);
			m_ControllerCaracter->SetMainCar(this);
		}
	}
	else
	{
		m_InteracetWidget->DestroyComponent();
		m_ControllerCaracter->SetMainCar(nullptr);
	}

	m_ControllerCaracter->SetIsEnterCar(isCanEnterCar);
	m_ControllerCaracter->SetIsLeftDoor(isNearLeftDoor);
}

void ABaseCar::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	m_HitActor = OtherActor;
	m_vecHitNormalImpuluse = NormalImpulse;
	memcpy(&m_HitResult, &Hit, sizeof(FHitResult));

	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (character != m_HitActor)       //캐릭터가 아닌 물체와 충돌했을 경우 캐릭터의 경우는 else로 해서 계산하면 될듯
	{
		if (IsValid(m_HitActor))          //해당 엑터가 사용가능한지 여부 확인
		{
			if (m_DoOnce.Execute())             //무조건 한번만 출력되도록 한다.........
			{
				FVector HitVel = m_HitActor->GetVelocity();
				FVector CarVel = this->GetVelocity();

				m_fImpact = (HitVel - CarVel).Size();            //길이를 구한다

				if (m_fImpact > m_fminImpact)
				{
					EPhysicalSurface surfacetype = UGameplayStatics::GetSurfaceType(m_HitResult);
					if (surfacetype == EPhysicalSurface::SurfaceType_Default)
					{
						if (m_fImpact > m_fmedImpact)
						{
							if (m_fImpact > m_fhighImpact)
							{
								ConstructorHelpers::FObjectFinder<USoundBase> sound(TEXT("SoundCue'Game/Car/Sound/cues/impacts/high_impact_Cue'"));
								m_ImpactSound = sound.Object;
							}
							else
							{
								ConstructorHelpers::FObjectFinder<USoundBase> sound(TEXT("SoundCue'Game/Car/Sound/cues/impacts/medium_Cue'"));
								m_ImpactSound = sound.Object;
							}
						}
						else
						{
							ConstructorHelpers::FObjectFinder<USoundBase> sound(TEXT("SoundCue'Game/Car/Sound/cues/impacts/low1_Cue'"));
							m_ImpactSound = sound.Object;
						}

						UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_ImpactSound, m_vecHitNormalImpuluse);
						ResetDoOnceImpactSound();
					}
					else if (surfacetype == EPhysicalSurface::SurfaceType1)
					{
						ConstructorHelpers::FObjectFinder<USoundBase> sound(TEXT("SoundCue'Game/Car/Sound/cues/impacts/plastic_Cue'"));
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound.Object, m_vecHitNormalImpuluse);
						ResetDoOnceImpactSound();
					}
				}
				else
				{
					ResetDoOnceImpactSound();
				}
			}
		}
	}
}

void ABaseCar::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	m_SkeltalDriver->SetHiddenInGame(false, false);
}

void ABaseCar::UnPossessed()
{
	Super::UnPossessed();

	m_SkeltalDriver->SetHiddenInGame(true, false);
}

void ABaseCar::SetMarkerRotation(USceneComponent* Maker, float fRotation, float fRotMul)
{
	Maker->SetRelativeRotation(FQuat(FRotator(3.0f, 0.0f, ((fRotation + fRotMul) - 127.0f))));
}

void ABaseCar::CarSmoke()
{
	// 차량의 속도를 체크한다
	if (GetVehicleMovement()->GetForwardSpeed() > 66.599999f)
	{
		/// 차량의 속도가 일정 수치 이상이므로 연기도 따라서 이동한다
		if (m_SmokeDoOnce.Execute())
		{
			UParticleSystem* LParticle = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem 'Game/Car/Particles/P_smoke_move'"));
			m_ParticleSmokeL->SetTemplate(LParticle);
			m_ParticleSmokeR->SetTemplate(LParticle);

			ResetDoOnceSmoke();
		}
	}
	else
	{
		/// 차량의 속도가 일정 수치 이하이므로 연기를 유지한다
		if (m_StillSmokeDoOnce.Execute())
		{
			UParticleSystem* LParticle = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem 'Game/Car/Particles/P_smoke_still'"));
			m_ParticleSmokeL->SetTemplate(LParticle);
			m_ParticleSmokeR->SetTemplate(LParticle);

			ResetDoOnceStillSmoke();
		}
	}
}

void ABaseCar::TurnOnOff()
{
	// 기어를 올렸니?
	if (m_fThrottle < 0.0f)
	{
		// 차량이 현재 후진 중인지 확인
		if (GetVehicleMovement()->GetForwardSpeed() < -1.0f)
		{
			//후진등을 켜야징
			SetLight(m_bisPostionLight, false, true);
		}
		else
		{
			// 후진은 아닌데.... 브레이크 등을 켜야겠군
			SetLight(m_bisPostionLight, true, false);
		}

		m_LightDoOnce.Reset();
	}
	else
	{
		// 달리는 중이다 후진 등하고 브레이크 등 둘다 끄자
		TurnBrakeRearLightOff();
	}
}
/// <summary>
/// 라이트 설정
/// </summary>
/// <param name="bisPositionLight">정면등 여부</param>
/// <param name="bisBrakeLight">브레이크 등 여부</param>
/// <param name="bisReverseLight">후진 등 여부</param>
void ABaseCar::SetLight(bool bisPositionLight, bool bisBrakeLight, bool bisReverseLight)
{
	m_bisPostionLight = bisPositionLight;

	//방향지시등
	if (m_bisPostionLight)
	{
		GetMesh()->SetMaterial(m_iRearPositionLightID, m_MatRearPositionLightOn);
		GetMesh()->SetMaterial(m_iFrontPositionLightID, m_MatFrontPositionLightOn);
	}
	else
	{
		GetMesh()->SetMaterial(m_iRearPositionLightID, m_MatRearPositionLightOff);
		GetMesh()->SetMaterial(m_iFrontPositionLightID, m_MatFrontPositionLightOff);
	}

	if (bisReverseLight)
		GetMesh()->SetMaterial(m_iReversBreakLightID, m_MatReversBreakLightOn);
	else
		GetMesh()->SetMaterial(m_iReversBreakLightID, m_MatReversBreakLightOff);

	if (bisBrakeLight)
		GetMesh()->SetMaterial(m_iBreakLight, m_MatBreakLightOn);
	else
		GetMesh()->SetMaterial(m_iBreakLight, m_MatBreakLigtOff);

	m_SpotFrontLightL->SetVisibility(m_bisPostionLight);
	m_SpotFrontLightR->SetVisibility(m_bisPostionLight);
	m_SpotRearLightL->SetVisibility(m_bisPostionLight);
	m_SpotRearLightR->SetVisibility(m_bisPostionLight);

	m_SpotBreakLightL->SetVisibility(bisBrakeLight);
	m_SpotBreakLightR->SetVisibility(bisBrakeLight);

	m_SpotReverseLightL->SetVisibility(bisReverseLight);
	m_SpotReverseLightR->SetVisibility(bisReverseLight);
}

void ABaseCar::MoveRight(float AxisValue)
{
	GetVehicleMovement()->SetSteeringInput(AxisValue);  //입력
	// 브레이크 Caliper
	CaliperRotation(m_MeshRightCaliperFront, TEXT("Front_Right_caliper"), false, true);
	CaliperRotation(m_MeshLeftCaliperFront, TEXT("Front_Left_caliper"), true, true);
	CaliperRotation(m_MeshLeftCaliperRear, TEXT("Rear_Left_caliper"), false, false);
	CaliperRotation(m_MeshRightCaliperRear, TEXT("Rear_Left_caliper"), true, false);

	// 메시 본의 회전 transform값을 구한다
	float fRoll = GetMesh()->GetSocketTransform(TEXT("Front_Right_Wheel"), ERelativeTransformSpace::RTS_Component).GetRotation().Z;

	m_MeshStreeringWheel->SetRelativeRotation(FQuat(FRotator(0.0f, 0.0f, fRoll)));

	//왼쪽 핸들
	m_vecLHandlerPos = m_MeshWheelHandlerL->GetComponentLocation();
	m_rotLHandler = m_MeshWheelHandlerL->GetComponentRotation();
	//오른쪽 핸들
	m_vecRHandlerPos = m_MeshWheelHandlerR->GetComponentLocation();
	m_rotRHandler = m_MeshWheelHandlerR->GetComponentRotation();
}

void ABaseCar::CaliperRotation(USceneComponent* Caliper, FName SocketName, bool bisLeftSide, bool bisFront)
{
	float fRoll = 0.f;
	float fWheelYaw = 0.f;
	FVector vecWheelLocation = FVector();

	// 초기화
	if (bisLeftSide)
		fRoll = 180.0f;

	//메시 본의 위치값과 회전값을 가져온다
	FTransform transform = GetMesh()->GetSocketTransform(SocketName, ERelativeTransformSpace::RTS_Component);
	vecWheelLocation = transform.GetLocation();
	fWheelYaw = transform.GetRotation().Z;

	//Z값에 제한을 건다
	if (!(fWheelYaw < 60.f && fWheelYaw > -60.f))
		fWheelYaw += 180.f;

	//뒤쪽휠의 z값은 0.0이다
	if (!bisFront)
		fWheelYaw = 0.f;

	//회전!!!
	Caliper->SetRelativeLocationAndRotation(vecWheelLocation, FQuat(FRotator(0.0f, fWheelYaw, fRoll)));
}

void ABaseCar::CarBreakPressed()
{
	GetVehicleMovement()->SetHandbrakeInput(true);
	SetLight(m_bisPostionLight, true, false);
}

void ABaseCar::CarBreakUnPressed()
{
	GetVehicleMovement()->SetHandbrakeInput(false);
	SetLight(m_bisPostionLight, false, false);
}

void ABaseCar::CarLightPressed()
{
	SetLight(m_LightFlip.Execute(), false, false);
}

void ABaseCar::CameraPressed()
{
	if (m_CameraFlip.Execute())
	{
		m_CamInCar->Activate(false);
		m_CamRear->Deactivate();
	}
	else
	{
		m_CamRear->Activate(false);
		m_CamInCar->Deactivate();
	}
}

void ABaseCar::MouseTurn(float AxisValue)
{
	FRotator rot = FRotator(0.0f, AxisValue, 0.0f);
	m_CamInCar->AddRelativeRotation(FQuat(rot));
	m_SpringArm->AddRelativeRotation(FQuat(rot));
}

void ABaseCar::MouseLookUp(float AxisValue)
{
	m_fLookUpInputMouse = AxisValue;
	float fSpringArmPitch = m_SpringArm->GetRelativeRotation().Pitch;

	if (!(fSpringArmPitch < -66.599998f && m_fLookUpInputMouse < 0.0f))
	{
		if (!(fSpringArmPitch > 0.f && m_fLookUpInputMouse > 0.0f))
		{
			FRotator rot = FRotator(m_fLookUpInputMouse, 0.0f, 0.0f);

			m_CamInCar->AddRelativeRotation(FQuat(rot));
			m_SpringArm->AddRelativeRotation(FQuat(rot));
		}
	}
}

FTransform ABaseCar::GetCameraTransform_Implementation()
{
	return FTransform();
}

UCameraComponent* ABaseCar::GetCamera_Implementation()
{
	return nullptr;
}
