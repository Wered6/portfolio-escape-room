// Fill out your copyright notice in the Description page of Project Settings.


#include "ERInteractablePawnBase.h"
#include "ERInteractableComponent.h"


AERInteractablePawnBase::AERInteractablePawnBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UERInteractableComponent>(TEXT("InteractableComponent"));
}

void AERInteractablePawnBase::DisplayInteractionUI_Implementation(const bool bShowInteract)
{
	InteractableComponent->DisplayInteractionUI(bShowInteract);
}

void AERInteractablePawnBase::InteractPressStarted_Implementation(AActor* OtherInstigator)
{
	InteractableComponent->InteractPressStarted(OtherInstigator);
}

void AERInteractablePawnBase::InteractPressTriggered_Implementation()
{
	InteractableComponent->InteractPressTriggered();
}

void AERInteractablePawnBase::InteractPressCompleted_Implementation()
{
	InteractableComponent->InteractPressCompleted();
}

float AERInteractablePawnBase::InteractHoldStarted_Implementation(AActor* OtherInstigator)
{
	return InteractableComponent->InteractHoldStarted(OtherInstigator);
}

void AERInteractablePawnBase::InteractHoldOngoing_Implementation(const float ElapsedSeconds)
{
	InteractableComponent->InteractHoldOngoing(ElapsedSeconds);
}

void AERInteractablePawnBase::InteractHoldTriggered_Implementation()
{
	InteractableComponent->InteractHoldTriggered();
}

void AERInteractablePawnBase::InteractHoldCanceled_Implementation()
{
	InteractableComponent->InteractHoldCanceled();
}

void AERInteractablePawnBase::InteractHoldCompleted_Implementation()
{
	InteractableComponent->InteractHoldCompleted();
}

bool AERInteractablePawnBase::DoesUseCustomInteractArea_Implementation()
{
	return InteractableComponent->DoesUseCustomInteractArea();
}

void AERInteractablePawnBase::SetCanInteract_Implementation(const bool bNewCanInteract)
{
	InteractableComponent->SetCanInteract(bNewCanInteract);
}

bool AERInteractablePawnBase::GetCanInteract_Implementation()
{
	return InteractableComponent->GetCanInteract();
}

EERInteractType AERInteractablePawnBase::GetInteractType_Implementation()
{
	return InteractableComponent->GetInteractType();
}
