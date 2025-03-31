// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EscapeRoom/InteractionSystem/ERInteractableActorBase.h"
#include "ERDeskLamp.generated.h"

class UBoxComponent;
class USpotLightComponent;

UCLASS()
class ESCAPEROOM_API AERDeskLamp : public AERInteractableActorBase
{
	GENERATED_BODY()

public:
	AERDeskLamp();

	virtual void InteractPressTriggered_Implementation() override;
	virtual void InteractHoldTriggered_Implementation() override;
	virtual USceneComponent* GetWidgetAttachmentComponent_Implementation() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LampMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpotLightComponent> LightComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> WidgetAttachment;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> InteractArea;
};
