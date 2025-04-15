// Fill out your copyright notice in the Description page of Project Settings.


#include "ERUVGlass.h"
#include "EscapeRoom/Character/ERCharacter.h"
#include "EscapeRoom/InteractionSystem/ERInteractableComponent.h"
#include "EscapeRoom/Utility/WeredMacros.h"


AERUVGlass::AERUVGlass()
{
	PrimaryActorTick.bCanEverTick = false;

	GlassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassMesh"));
	GlassMesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(GlassMesh);
}

void AERUVGlass::InteractHoldTriggered_Implementation()
{
	Super::InteractHoldTriggered_Implementation();

	AERCharacter* Character{Cast<AERCharacter>(InteractableComponent->GetInteractInstigator())};

	UVALID_LOG_DEBUG(Character)

	Character->CollectUVGlassData(Data);
}

void AERUVGlass::InteractHoldCompleted_Implementation()
{
	Super::InteractHoldCompleted_Implementation();

	Destroy();
}
