// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ERKeypadBase.h"
#include "ERKeypadTV.generated.h"

class AERTV;

UCLASS()
class ESCAPEROOM_API AERKeypadTV : public AERKeypadBase
{
	GENERATED_BODY()

public:
	AERKeypadTV();

protected:
	virtual void ButtonPressHandle_Implementation(const UStaticMeshComponent* ButtonMesh, const uint8 ButtonValue, const EKeypadButtonName ButtonName) override;

private:
	UPROPERTY(EditInstanceOnly, Category="ER|KeypadTV")
	TObjectPtr<AERTV> TV;

	uint16 Channel{};
};
