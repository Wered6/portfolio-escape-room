// Fill out your copyright notice in the Description page of Project Settings.


#include "ERKeypadPassword.h"
#include "EscapeRoom/Components/ERKeyComponent.h"


AERKeypadPassword::AERKeypadPassword()
{
	KeyComponent = CreateDefaultSubobject<UERKeyComponent>(TEXT("KeyComponent"));
}

void AERKeypadPassword::BeginPlay()
{
	Super::BeginPlay();

	OnFinishProcessing.BindUObject(this, &AERKeypadPassword::CheckPassword);
}

void AERKeypadPassword::ButtonPressHandle_Implementation(const UStaticMeshComponent* ButtonMesh, const uint8 ButtonValue, const EKeypadButtonName ButtonName)
{
	Super::ButtonPressHandle_Implementation(ButtonMesh, ButtonValue, ButtonName);

	switch (ButtonName)
	{
	case EKeypadButtonName::Digit:
		UserPassword = UserPassword + FString::FromInt(ButtonValue);
		break;
	case EKeypadButtonName::DEL:
		if (!UserPassword.IsEmpty())
		{
			UserPassword = UserPassword.LeftChop(1);
		}
		break;
	case EKeypadButtonName::OK:
		UE_LOG(LogTemp, Warning, TEXT("UserPassword: %s"), *UserPassword)
	// In parent here is OnFinishProcessing, if you want to do something after OK, bind to OnFinishProcessing
		break;
	}
}

void AERKeypadPassword::CheckPassword()
{
	if (Password == UserPassword)
	{
		KeyComponent->UnlockLockedItems();
		LedFlash(ELedColor::Green, LedLongFlashTime);
		ExitKeypadMode();
		Execute_SetCanInteract(this, false);

		if (OnCorrectPassword.IsBound())
		{
			OnCorrectPassword.Execute();
		}
	}
	else
	{
		LedFlash(ELedColor::Red, LedLongFlashTime);
		UserPassword.Empty();

		if (OnWrongPassword.IsBound())
		{
			OnWrongPassword.Execute();
		}
	}
}
