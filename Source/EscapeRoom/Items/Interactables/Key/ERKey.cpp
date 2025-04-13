// Fill out your copyright notice in the Description page of Project Settings.


#include "ERKey.h"
#include "EscapeRoom/InteractionSystem/ERInteractableComponent.h"
#include "EscapeRoom/LockSystem/ERUnlockerComponent.h"


AERKey::AERKey()
{
	PrimaryActorTick.bCanEverTick = false;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(KeyMesh);

	UnlockerComponent = CreateDefaultSubobject<UERUnlockerComponent>(TEXT("UnlockerComponent"));
}

void AERKey::BeginPlay()
{
	Super::BeginPlay();

	InteractableComponent->AddOutlineMeshComponent(KeyMesh);
}

void AERKey::InteractHoldTriggered_Implementation()
{
	Super::InteractHoldTriggered_Implementation();

	UnlockerComponent->UnlockObjects();
}

void AERKey::InteractHoldCompleted_Implementation()
{
	Super::InteractHoldCompleted_Implementation();

	Destroy();
}
