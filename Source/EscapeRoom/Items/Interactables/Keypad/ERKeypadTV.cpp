// Fill out your copyright notice in the Description page of Project Settings.


#include "ERKeypadTV.h"
#include "EscapeRoom/Items/NonInteractables/TV/ERTV.h"
#include "EscapeRoom/Utility/WeredMacros.h"


AERKeypadTV::AERKeypadTV()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AERKeypadTV::ButtonPressHandle_Implementation(const UStaticMeshComponent* ButtonMesh, const uint8 ButtonValue, const EKeypadButtonName ButtonName)
{
	Super::ButtonPressHandle_Implementation(ButtonMesh, ButtonValue, ButtonName);

	UVALID_LOG_DEBUG(TV)

	switch (ButtonName)
	{
	case EKeypadButtonName::Digit:
		Channel *= 10;
		Channel += ButtonValue;
		break;
	case EKeypadButtonName::DEL:
		Channel /= 10;
		break;
	case EKeypadButtonName::OK:
		switch (Channel)
		{
		case 14:
			TV->OpenCardinalBirdVideo();
			break;
		case 2:
			TV->OpenHangman();
			break;
		}
		Channel = 0;
		Exit();
		break;
	}
}
