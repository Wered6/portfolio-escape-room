// Fill out your copyright notice in the Description page of Project Settings.


#include "ERDeskLamp.h"

#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "EscapeRoom/InteractionSystem/ERInteractableComponent.h"


AERDeskLamp::AERDeskLamp()
{
	PrimaryActorTick.bCanEverTick = false;

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh"));
	SetRootComponent(LampMesh);

	LightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(LampMesh);
	LightComp->SetVisibility(false);

	InteractableComp->AddOutlineMeshComponent(LampMesh);
	InteractableComp->bUseCustomInteractArea = true;
	InteractableComp->InteractType = EERInteractType::Hold;
	InteractableComp->InteractCategory = EERInteractCategory::Open;
	InteractableComp->HoldTimeThreshold = 0.5f;

	WidgetAttachment = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetAttachment"));
	WidgetAttachment->SetupAttachment(LampMesh);

	InteractArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractArea"));
	InteractArea->SetCollisionProfileName(TEXT("InteractArea"));
	InteractArea->SetupAttachment(LampMesh);
}

void AERDeskLamp::InteractPressTriggered_Implementation()
{
	Super::InteractPressTriggered_Implementation();

	LightComp->ToggleVisibility();
}

void AERDeskLamp::InteractHoldTriggered_Implementation()
{
	Super::InteractHoldTriggered_Implementation();

	LightComp->ToggleVisibility();
}

USceneComponent* AERDeskLamp::GetWidgetAttachmentComponent_Implementation()
{
	return WidgetAttachment;
}
