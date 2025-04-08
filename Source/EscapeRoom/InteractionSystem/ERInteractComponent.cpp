// Fill out your copyright notice in the Description page of Project Settings.


#include "ERInteractComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EscapeRoom/InteractionSystem/ERInteractInterface.h"
#include "GameFramework/Character.h"


UERInteractComponent::UERInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UERInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<ACharacter>(GetOwner());

#pragma region Nullchecks
	if (!CharacterOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|CharacterOwner is nullptr"), *FString(__FUNCTION__))
		return;
	}
	if (!InteractMappingContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|InteractMappingContext is nullptr"), *FString(__FUNCTION__))
		return;
	}
	if (!InteractPressAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|InteractPressAction is nullptr"), *FString(__FUNCTION__))
		return;
	}
	if (!InteractHoldAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|InteractHoldAction is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	const APlayerController* PlayerController{CharacterOwner->GetController<APlayerController>()};

#pragma region Nullchecks
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|PlayerController is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	UEnhancedInputLocalPlayerSubsystem* Subsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())};
	UEnhancedInputComponent* EnhancedInputComponent{Cast<UEnhancedInputComponent>(PlayerController->InputComponent)};

#pragma region Nullchecks
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|Subsystem is nullptr"), *FString(__FUNCTION__))
		return;
	}
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|EnhancedInputComponent is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	Subsystem->AddMappingContext(InteractMappingContext, 0);

	EnhancedInputComponent->BindAction(InteractPressAction, ETriggerEvent::Started, this, &UERInteractComponent::InteractPressStarted);
	EnhancedInputComponent->BindAction(InteractPressAction, ETriggerEvent::Triggered, this, &UERInteractComponent::InteractPressTriggered);
	EnhancedInputComponent->BindAction(InteractPressAction, ETriggerEvent::Completed, this, &UERInteractComponent::InteractPressCompleted);

	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Started, this, &UERInteractComponent::InteractHoldStarted);
	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Ongoing, this, &UERInteractComponent::InteractHoldOngoing);
	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Canceled, this, &UERInteractComponent::InteractHoldCanceled);
	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Triggered, this, &UERInteractComponent::InteractHoldTriggered);
	EnhancedInputComponent->BindAction(InteractHoldAction, ETriggerEvent::Completed, this, &UERInteractComponent::InteractHoldCompleted);
}

void UERInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PerformInteractionCheck();
}

void UERInteractComponent::InteractPressStarted()
{
#pragma region Nullchecks
	if (!CharacterOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|CharacterOwner is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	if (InteractableActor && InteractableActor->Implements<UERInteractInterface>() && IERInteractInterface::Execute_GetInteractType(InteractableActor) == EERInteractType::Press)
	{
		IERInteractInterface::Execute_InteractPressStarted(InteractableActor, CharacterOwner);
	}
}

void UERInteractComponent::InteractPressTriggered()
{
	if (InteractableActor && InteractableActor->Implements<UERInteractInterface>() && IERInteractInterface::Execute_GetInteractType(InteractableActor) == EERInteractType::Press)
	{
		IERInteractInterface::Execute_InteractPressTriggered(InteractableActor);
	}
}

void UERInteractComponent::InteractPressCompleted()
{
	if (InteractableActor && InteractableActor->Implements<UERInteractInterface>() && IERInteractInterface::Execute_GetInteractType(InteractableActor) == EERInteractType::Press)
	{
		IERInteractInterface::Execute_InteractPressCompleted(InteractableActor);
	}
}

void UERInteractComponent::InteractHoldStarted(const FInputActionInstance& Instance)
{
#pragma region Nullchecks
	if (!CharacterOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|CharacterOwner is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	if (!InteractableActor || !InteractableActor->Implements<UERInteractInterface>() || IERInteractInterface::Execute_GetInteractType(InteractableActor) != EERInteractType::Hold)
	{
		return;
	}

	for (UInputTrigger* Trigger : Instance.GetTriggers())
	{
		if (UInputTriggerHold* TriggerHold{Cast<UInputTriggerHold>(Trigger)})
		{
			TriggerHold->HoldTimeThreshold = IERInteractInterface::Execute_InteractHoldStarted(InteractableActor, CharacterOwner);
			UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *FString(__FUNCTION__), *TriggerHold->GetDebugState())
			break;
		}
	}

	bIsHolding = true;
}

void UERInteractComponent::InteractHoldOngoing(const FInputActionInstance& Instance)
{
	if (bIsHolding && InteractableActor && InteractableActor->Implements<UERInteractInterface>() && IERInteractInterface::Execute_GetInteractType(InteractableActor) == EERInteractType::Hold)
	{
		IERInteractInterface::Execute_InteractHoldOngoing(InteractableActor, Instance.GetElapsedTime());
	}
}

void UERInteractComponent::InteractHoldCanceled()
{
	if (bIsHolding && InteractableActor && InteractableActor->Implements<UERInteractInterface>() && IERInteractInterface::Execute_GetInteractType(InteractableActor) == EERInteractType::Hold)
	{
		bIsHolding = false;

		IERInteractInterface::Execute_InteractHoldCanceled(InteractableActor);
	}
}

void UERInteractComponent::InteractHoldTriggered()
{
	if (bIsHolding && InteractableActor && InteractableActor->Implements<UERInteractInterface>() && IERInteractInterface::Execute_GetInteractType(InteractableActor) == EERInteractType::Hold)
	{
		IERInteractInterface::Execute_InteractHoldTriggered(InteractableActor);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UERInteractComponent::InteractHoldCompleted()
{
	if (bIsHolding && InteractableActor && InteractableActor->Implements<UERInteractInterface>() && IERInteractInterface::Execute_GetInteractType(InteractableActor) == EERInteractType::Hold)
	{
		bIsHolding = false;

		IERInteractInterface::Execute_InteractHoldCompleted(InteractableActor);
	}
}

void UERInteractComponent::PerformInteractionCheck()
{
	if (!bCanCheckInteraction)
	{
		return;
	}

#pragma region Nullchecks
	if (!CharacterOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|CharacterOwner is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	const APlayerController* PlayerController{CharacterOwner->GetController<APlayerController>()};

#pragma region Nullchecks
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|PlayerController is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	// Get camera location and direction
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Define the start and end points of the line trace
	const FVector Start{CameraLocation};
	const FVector End{Start + CameraRotation.Vector() * InteractionDistance};

	// Perform the line trace
	FHitResult HitResult;
	const bool bHit
	{
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility)
	};

	/**
	 * Clear interaction if at least one of below conditions is met:
	 * - nothing was hit
	 * - HitResult is not an actor
	 * - HitActor doesn't implement interact interface
	 * - HitActor is not allowed to interact
	 */
	AActor* HitActor{HitResult.GetActor()};
	UPrimitiveComponent* HitComponent{HitResult.GetComponent()};
	if (!bHit || !HitActor || !HitActor->Implements<UERInteractInterface>() || !IERInteractInterface::Execute_GetCanInteract(HitActor))
	{
		ClearInteraction();
		return;
	}

	const bool bUsesCustomInteractArea{IERInteractInterface::Execute_DoesUseCustomInteractArea(HitActor)};
	// Check if a custom interaction area is used:
	// compare components in this case
	if (bUsesCustomInteractArea && HitComponent == InteractableHitComponent)
	{
		return;
	}
	// otherwise, compare actors
	if (!bUsesCustomInteractArea && HitActor == InteractableActor)
	{
		return;
	}

	// Hide UI from previously focused interactable object
	if (InteractableActor)
	{
		IERInteractInterface::Execute_DisplayInteractionUI(InteractableActor, false);
	}

	// Depending on custom area usage, set or clear interaction
	if (bUsesCustomInteractArea)
	{
		if (HitComponent->GetCollisionProfileName() == FName("InteractArea"))
		{
			SetInteraction(HitActor, HitComponent);
		}
		else
		{
			ClearInteraction();
		}
	}
	else
	{
		SetInteraction(HitActor, HitComponent);
	}
}

void UERInteractComponent::SetInteraction(AActor* InteractedActor, UPrimitiveComponent* InteractedComponent)
{
	IERInteractInterface::Execute_DisplayInteractionUI(InteractedActor, true);
	InteractableActor = InteractedActor;
	InteractableHitComponent = InteractedComponent;
}

void UERInteractComponent::ClearInteraction()
{
	if (bIsHolding)
	{
		InteractHoldCanceled();
	}

	if (InteractableActor)
	{
		IERInteractInterface::Execute_DisplayInteractionUI(InteractableActor, false);
		InteractableActor = nullptr;
		InteractableHitComponent = nullptr;
	}
}
