// Fill out your copyright notice in the Description page of Project Settings.


#include "ERKeypadConverter.h"
#include "EscapeRoom/Items/NonInteractables/TV/ERTV.h"


AERKeypadConverter::AERKeypadConverter()
{
	PrimaryActorTick.bCanEverTick = false;

	bProcessing = false;
}

void AERKeypadConverter::BeginPlay()
{
	Super::BeginPlay();

	OnFinishProcessing.BindUObject(this, &AERKeypadConverter::Convert);
}

void AERKeypadConverter::ButtonPressHandle_Implementation(const UStaticMeshComponent* ButtonMesh, const uint8 ButtonValue, const EKeypadButtonName ButtonName)
{
	Super::ButtonPressHandle_Implementation(ButtonMesh, ButtonValue, ButtonName);

#pragma region Nullchecks
	if (!TV)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|TV is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	switch (ButtonName)
	{
	case EKeypadButtonName::Digit:
		TV->SendNumberToConverter(ButtonValue);
		break;
	case EKeypadButtonName::DEL:
		break;
	case EKeypadButtonName::OK:
		if (!TV->NextRGBField())
		{
			bProcessing = true;
		}
		break;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AERKeypadConverter::Convert()
{
#pragma region Nullchecks
	if (!TV)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|TV is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	TV->ConvertRGBToHSV();
	TV->ResetConverter();

	bProcessing = false;
}
