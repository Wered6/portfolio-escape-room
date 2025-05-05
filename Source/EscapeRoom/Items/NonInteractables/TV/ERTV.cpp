// Fill out your copyright notice in the Description page of Project Settings.


#include "ERTV.h"
#include "ERTVConverterScreenWidget.h"
#include "ERTVScreenSignWidget.h"
#include "ERTVScreenWidget.h"
#include "FileMediaSource.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "Components/WidgetComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "EscapeRoom/Character/ERCharacter.h"
#include "EscapeRoom/InteractionSystem/ERInteractComponent.h"
#include "EscapeRoom/Items/Interactables/Flashlight/ERFlashlight.h"
#include "EscapeRoom/Items/Interactables/Keypad/ERKeypadPassword.h"
#include "EscapeRoom/Items/NonInteractables/AlarmClock/ERAlarmClock.h"
#include "EscapeRoom/LockSystem/ERUnlockerComponent.h"
#include "EscapeRoom/Utility/WeredMacros.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/MediaAssets/Public/MediaSoundComponent.h"


AERTV::AERTV()
{
	PrimaryActorTick.bCanEverTick = false;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(RootMesh);
	RootMesh->SetCollisionProfileName(TEXT("BlockAll"));

	HangmanWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HangmanWidgetComp"));
	HangmanWidgetComp->SetupAttachment(RootMesh);
	// Hide widget from screen in scene
	HangmanWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));
	HangmanWidgetComp->SetDrawSize(FVector2D(1440.f, 1440.f));

	ConverterWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ConverterWidgetComp"));
	ConverterWidgetComp->SetupAttachment(RootMesh);
	// Hide widget from screen in scene
	ConverterWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));
	ConverterWidgetComp->SetDrawSize(FVector2D(1440.f, 1440.f));

	TVSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("FilmSound"));
	TVSound->SetupAttachment(RootMesh);
	TVSound->bAllowSpatialization = true;
	TVSound->bOverrideAttenuation = true;
	FSoundAttenuationSettings AttenuationSettings;
	AttenuationSettings.FalloffDistance = 400.f;
	TVSound->AttenuationOverrides = AttenuationSettings;

	UnlockerComponent = CreateDefaultSubobject<UERUnlockerComponent>(TEXT("UnlockerComponent"));
}

void AERTV::BeginPlay()
{
	Super::BeginPlay();

	HangmanWidget = Cast<UERTVScreenWidget>(HangmanWidgetComp->GetWidget());
	ConverterWidget = Cast<UERTVConverterScreenWidget>(ConverterWidgetComp->GetWidget());

	ScreenDynMat = RootMesh->CreateDynamicMaterialInstance(1);

	UVALID_LOG_DEBUG(HangmanWidget)
	UVALID_LOG_DEBUG(TVSound)
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(NoSignalMediaSource)

	HangmanWidget->Password = Password;

	TVSound->SetMediaPlayer(TVMediaPlayer);
	// OpenIntro1();
	// CDPR
	TVMediaPlayer->SetLooping(true);
	TVMediaPlayer->OpenSource(NoSignalMediaSource);

	//CDPR
	// Flashlight->OnFlashlightEquipped.BindUObject(this, &AERTV::OpenStage1);
	// Keypad->OnCorrectPassword.BindUObject(this, &AERTV::OpenStage2);
}

void AERTV::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(NoSignalMediaSource)

	TVMediaPlayer->OpenSource(NoSignalMediaSource);
	TVMediaPlayer->SetLooping(false);
}

bool AERTV::EnterSignToHangman(const FString& Sign) const
{
	UVALID_LOG_DEBUGB(HangmanWidget)

	const bool CorrectSign{HangmanWidget->EnterSignToPassword(Sign)};
	if (HangmanWidget->Password == HangmanWidget->UserPassword)
	{
		UnlockerComponent->UnlockObjects();
		if (OnCorrectHangmanPassword.IsBound())
		{
			OnCorrectHangmanPassword.Execute();
			HangmanWidget->BravoWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	return CorrectSign;
}

void AERTV::SendNumberToConverter(const uint8 Number) const
{
	UVALID_LOG_DEBUG(ConverterWidget)

	ConverterWidget->UpdateCurrentRGBArrayElement(Number);
}

bool AERTV::NextRGBField() const
{
	UVALID_LOG_DEBUGB(ConverterWidget)

	return ConverterWidget->NextRGBField();
}

void AERTV::ConvertRGBToHSV() const
{
	UVALID_LOG_DEBUG(ConverterWidget)

	ConverterWidget->Convert();
}

void AERTV::ResetConverter() const
{
	UVALID_LOG_DEBUG(ConverterWidget)

	ConverterWidget->ResetRGBArrayIndex();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AERTV::StartAlarmClock()
{
	UVALID_LOG_DEBUG(AlarmClock)

	AlarmClock->StartClock();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AERTV::ShowHangmanWidgetOnScreen()
{
	UVALID_LOG_DEBUG(ScreenDynMat)
	UVALID_LOG_DEBUG(HangmanWidgetComp)
	UVALID_LOG_DEBUG(HangmanWidgetComp->GetRenderTarget())

	ScreenDynMat->SetTextureParameterValue(FName("Texture"), HangmanWidgetComp->GetRenderTarget());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AERTV::ShowConverterWidgetOnScreen()
{
	UVALID_LOG_DEBUG(ScreenDynMat)
	UVALID_LOG_DEBUG(ConverterWidgetComp)
	UVALID_LOG_DEBUG(ConverterWidgetComp->GetRenderTarget())

	ScreenDynMat->SetTextureParameterValue(FName("Texture"), ConverterWidgetComp->GetRenderTarget());
}

void AERTV::OpenIntro1()
{
	UVALID_LOG_DEBUG(TVMediaPlayer)

	TVMediaPlayer->OpenSource(Intro1MediaSource);
	TVMediaPlayer->OnEndReached.AddDynamic(this, &AERTV::OpenIntro2);
}

void AERTV::OpenIntro2()
{
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(Intro2MediaSource)

	TVMediaPlayer->OpenSource(Intro2MediaSource);
	TVMediaPlayer->OnEndReached.Clear();
	TVMediaPlayer->OnEndReached.AddDynamic(this, &AERTV::OpenIntro3);
	TVMediaPlayer->OnEndReached.AddDynamic(this, &AERTV::StartAlarmClock);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AERTV::OpenIntro3()
{
	AERCharacter* Character{Cast<AERCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0))};

	UVALID_LOG_DEBUG(Character)
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(NoSignalMediaSource)

	TVMediaPlayer->OpenSource(NoSignalMediaSource);
	TVMediaPlayer->OnEndReached.Clear();
	TVMediaPlayer->SetLooping(true);

	Character->SetLimitMovement(false);
	Character->SetIndicatorVisibility(true);
	Character->GetInteractComponent()->SetCanCheckInteraction(true);
}

void AERTV::OpenStage1()
{
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(FlashlightClueMediaSource)

	TVMediaPlayer->SetLooping(false);
	TVMediaPlayer->OpenSource(FlashlightClueMediaSource);
	TVMediaPlayer->OnEndReached.Clear();
	TVMediaPlayer->OnEndReached.AddDynamic(this, &AERTV::ShowConverterWidgetOnScreen);
}

void AERTV::OpenStage2()
{
	UVALID_LOG_DEBUG(ScreenDynMat)
	UVALID_LOG_DEBUG(TVMediaTexture)
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(PasswordClueMediaSource)

	ScreenDynMat->SetTextureParameterValue(FName("Texture"), TVMediaTexture);

	TVMediaPlayer->OpenSource(PasswordClueMediaSource);
	TVMediaPlayer->OnEndReached.Clear();
	TVMediaPlayer->OnEndReached.AddDynamic(this, &AERTV::ShowHangmanWidgetOnScreen);
}

void AERTV::OpenToBeContinued()
{
	UVALID_LOG_DEBUG(ScreenDynMat)
	UVALID_LOG_DEBUG(TVMediaTexture)
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(ToBeContinuedMediaSource)

	ScreenDynMat->SetTextureParameterValue(FName("Texture"), TVMediaTexture);

	TVMediaPlayer->OpenSource(ToBeContinuedMediaSource);
}

void AERTV::OpenCardinalBirdVideo()
{
	UVALID_LOG_DEBUG(ScreenDynMat)
	UVALID_LOG_DEBUG(TVMediaTexture)
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(CardinalBirdMediaSource)

	ScreenDynMat->SetTextureParameterValue(FName("Texture"), TVMediaTexture);

	TVMediaPlayer->SetLooping(false);
	TVMediaPlayer->OpenSource(CardinalBirdMediaSource);
}

void AERTV::OpenHangman()
{
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(ScreenDynMat)
	UVALID_LOG_DEBUG(HangmanWidgetComp)
	UVALID_LOG_DEBUG(HangmanWidgetComp->GetRenderTarget())

	TVMediaPlayer->Seek(TVMediaPlayer->GetDuration());
	TVMediaPlayer->SetLooping(false);

	ScreenDynMat->SetTextureParameterValue(FName("Texture"), HangmanWidgetComp->GetRenderTarget());
}

void AERTV::OpenTechlandEndingVideo()
{
	UVALID_LOG_DEBUG(ScreenDynMat)
	UVALID_LOG_DEBUG(TVMediaTexture)
	UVALID_LOG_DEBUG(TVMediaPlayer)
	UVALID_LOG_DEBUG(TechlandEndingVideo)

	ScreenDynMat->SetTextureParameterValue(FName("Texture"), TVMediaTexture);

	TVMediaPlayer->SetLooping(false);
	TVMediaPlayer->OpenSource(TechlandEndingVideo);
}
