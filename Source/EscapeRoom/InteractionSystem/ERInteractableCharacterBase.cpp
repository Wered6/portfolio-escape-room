// Fill out your copyright notice in the Description page of Project Settings.


#include "ERInteractableCharacterBase.h"
#include "ERInteractableComponent.h"


AERInteractableCharacterBase::AERInteractableCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UERInteractableComponent>(TEXT("InteractableComponent"));
}

void AERInteractableCharacterBase::DisplayInteractionUI_Implementation(const bool bShowInteract)
{
	InteractableComponent->DisplayInteractionUI(bShowInteract);
}

void AERInteractableCharacterBase::InteractPressStarted_Implementation(AActor* OtherInstigator)
{
	InteractableComponent->InteractPressStarted(OtherInstigator);
}

void AERInteractableCharacterBase::InteractPressTriggered_Implementation()
{
	InteractableComponent->InteractPressTriggered();
}

void AERInteractableCharacterBase::InteractPressCompleted_Implementation()
{
	InteractableComponent->InteractPressCompleted();
}

float AERInteractableCharacterBase::InteractHoldStarted_Implementation(AActor* OtherInstigator)
{
	return InteractableComponent->InteractHoldStarted(OtherInstigator);
}

void AERInteractableCharacterBase::InteractHoldOngoing_Implementation(const float ElapsedSeconds)
{
	InteractableComponent->InteractHoldOngoing(ElapsedSeconds);
}

void AERInteractableCharacterBase::InteractHoldTriggered_Implementation()
{
	InteractableComponent->InteractHoldTriggered();
}

void AERInteractableCharacterBase::InteractHoldCanceled_Implementation()
{
	InteractableComponent->InteractHoldCanceled();
}

void AERInteractableCharacterBase::InteractHoldCompleted_Implementation()
{
	InteractableComponent->InteractHoldCompleted();
}

bool AERInteractableCharacterBase::DoesUseCustomInteractArea_Implementation()
{
	return InteractableComponent->DoesUseCustomInteractArea();
}

void AERInteractableCharacterBase::SetCanInteract_Implementation(const bool bNewCanInteract)
{
	InteractableComponent->SetCanInteract(bNewCanInteract);
}

bool AERInteractableCharacterBase::GetCanInteract_Implementation()
{
	return InteractableComponent->GetCanInteract();
}

EERInteractType AERInteractableCharacterBase::GetInteractType_Implementation()
{
	return InteractableComponent->GetInteractType();
}
