// Fill out your copyright notice in the Description page of Project Settings.


#include "ERInteractableActorBase.h"
#include "ERInteractableComponent.h"


AERInteractableActorBase::AERInteractableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UERInteractableComponent>(TEXT("InteractableComponent"));
}

void AERInteractableActorBase::DisplayInteractionUI_Implementation(const bool bShowInteract)
{
	InteractableComponent->DisplayInteractionUI(bShowInteract);
}

void AERInteractableActorBase::InteractPressStarted_Implementation(AActor* OtherInstigator)
{
	InteractableComponent->InteractPressStarted(OtherInstigator);
}

void AERInteractableActorBase::InteractPressTriggered_Implementation()
{
	InteractableComponent->InteractPressTriggered();
}

void AERInteractableActorBase::InteractPressCompleted_Implementation()
{
	InteractableComponent->InteractPressCompleted();
}

float AERInteractableActorBase::InteractHoldStarted_Implementation(AActor* OtherInstigator)
{
	return InteractableComponent->InteractHoldStarted(OtherInstigator);
}

void AERInteractableActorBase::InteractHoldOngoing_Implementation(const float ElapsedSeconds)
{
	InteractableComponent->InteractHoldOngoing(ElapsedSeconds);
}

void AERInteractableActorBase::InteractHoldTriggered_Implementation()
{
	InteractableComponent->InteractHoldTriggered();
}

void AERInteractableActorBase::InteractHoldCanceled_Implementation()
{
	InteractableComponent->InteractHoldCanceled();
}

void AERInteractableActorBase::InteractHoldCompleted_Implementation()
{
	InteractableComponent->InteractHoldCompleted();
}

bool AERInteractableActorBase::DoesUseCustomInteractArea_Implementation()
{
	return InteractableComponent->DoesUseCustomInteractArea();
}

void AERInteractableActorBase::SetCanInteract_Implementation(const bool bNewCanInteract)
{
	InteractableComponent->SetCanInteract(bNewCanInteract);
}

bool AERInteractableActorBase::GetCanInteract_Implementation()
{
	return InteractableComponent->GetCanInteract();
}

EERInteractType AERInteractableActorBase::GetInteractType_Implementation()
{
	return InteractableComponent->GetInteractType();
}
