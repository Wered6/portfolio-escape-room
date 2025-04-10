// Fill out your copyright notice in the Description page of Project Settings.


#include "ERInteractableComponent.h"
#include "ERInteractableActorBase.h"
#include "ERInteractableCharacterBase.h"
#include "ERInteractablePawnBase.h"
#include "ERInteractIconWidget.h"
#include "ERInteractInterface.h"
#include "Components/WidgetComponent.h"
#include "EscapeRoom/Utility/WeredMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteractableComponent, Log, All);


UERInteractableComponent::UERInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UERInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeInteractWidget();
	UpdateWidgetAttachment();
}

void UERInteractableComponent::AddOutlineMeshComponent(UMeshComponent* MeshComponent)
{
	OutlineMeshComps.Add(MeshComponent);
}

void UERInteractableComponent::SetOutlineMeshComponents(const TArray<UMeshComponent*>& NewOutlineMeshComponents)
{
	OutlineMeshComps = NewOutlineMeshComponents;
}

void UERInteractableComponent::DisplayInteractionUI(const bool bShowInteract)
{
	UVALID_LOG_DEBUG(InteractWidget)

	InteractWidget->SetVisibility(bShowInteract ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	UpdateOutline(bShowInteract);
}

void UERInteractableComponent::InteractPressStarted(AActor* OtherInstigator)
{
	InteractInstigator = OtherInstigator;
	UE_LOG(LogTemp, Warning, TEXT("PressStarted"))
}

void UERInteractableComponent::InteractPressTriggered()
{
	UE_LOG(LogTemp, Warning, TEXT("PressTriggered"))
}

void UERInteractableComponent::InteractPressCompleted()
{
	InteractInstigator = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("PressCompleted"))
}

float UERInteractableComponent::InteractHoldStarted(AActor* OtherInstigator)
{
	UVALID_LOG_DEBUGF(InteractWidget)

	InteractInstigator = OtherInstigator;
	UE_LOG(LogTemp, Warning, TEXT("HoldStarted"))

	return HoldTimeThreshold;
}

void UERInteractableComponent::InteractHoldOngoing(const float ElapsedSeconds)
{
	UVALID_LOG_DEBUG(InteractWidget)

	const float ProgressFraction{FMath::Clamp(ElapsedSeconds / HoldTimeThreshold, 0.f, 1.f)};
	InteractWidget->SetIconOpacity(ProgressFraction);
	InteractWidget->SetProgressCircleOpacity(ProgressFraction);
	InteractWidget->SetProgressCirclePercent(ProgressFraction);
}

void UERInteractableComponent::InteractHoldTriggered()
{
	UE_LOG(LogTemp, Warning, TEXT("HoldTriggered"))
}

void UERInteractableComponent::InteractHoldCanceled()
{
	UVALID_LOG_DEBUG(InteractWidget)

	InteractInstigator = nullptr;
	InteractWidget->SetProgressCircleOpacity(0.f);
	InteractWidget->SetProgressCirclePercent(0.f);
	InteractWidget->SetIconOpacity(0.f);
	UE_LOG(LogTemp, Warning, TEXT("HoldCanceled"))
}

void UERInteractableComponent::InteractHoldCompleted()
{
	InteractInstigator = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("HoldCompleted"))
}

bool UERInteractableComponent::DoesUseCustomInteractArea() const
{
	return bUseCustomInteractArea;
}

void UERInteractableComponent::SetCanInteract(const bool bNewCanInteract)
{
	bCanInteract = bNewCanInteract;
}

bool UERInteractableComponent::GetCanInteract() const
{
	return bCanInteract;
}

EERInteractType UERInteractableComponent::GetInteractType() const
{
	return InteractType;
}

void UERInteractableComponent::SetInteractCategory(const EERInteractCategory InInteractCategory)
{
	UVALID_LOG_DEBUG(InteractWidget)

	InteractWidget->SetInteractCategory(InInteractCategory);
}

void UERInteractableComponent::OnRegister()
{
	Super::OnRegister();

	AActor* Owner{GetOwner()};
	if (!Cast<AERInteractableActorBase>(Owner) && !Cast<AERInteractablePawnBase>(Owner) && !Cast<AERInteractableCharacterBase>(Owner))
	{
		UE_LOG(LogInteractableComponent, Error, TEXT("%s owned by %s must register a component owned by a InteractableActorBase, InteractablePawnBase or InteractableCharacterBase"), *GetName(), *GetNameSafe(GetOwner()))
	}
}

void UERInteractableComponent::InitializeInteractWidget()
{
	UVALID_LOG_DEBUG(InteractWidgetClass)

	InteractWidget = CreateWidget<UERInteractIconWidget>(GetWorld(), InteractWidgetClass);
	InteractWidgetComp = NewObject<UWidgetComponent>(this, TEXT("InteractWidgetComp"));

	UVALID_LOG_DEBUG(InteractWidget)
	UVALID_LOG_DEBUG(InteractWidgetComp)

	InteractWidget->Init(InteractCategory,
	                     InteractType,
	                     MinimalIconOpacity,
	                     IconSize,
	                     MinimalProgressCircleOpacity,
	                     ProgressCircleSize);
	InteractWidget->SetVisibility(ESlateVisibility::Hidden);
	InteractWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidgetComp->SetDrawAtDesiredSize(true);
	InteractWidgetComp->SetWidget(InteractWidget);
	InteractWidgetComp->RegisterComponent();
}

void UERInteractableComponent::UpdateWidgetAttachment() const
{
	UVALID_LOG_DEBUG(InteractWidgetComp)

	USceneComponent* WidgetAttachComp{IERInteractInterface::Execute_GetWidgetAttachmentComponent(GetOwner())};
	if (!WidgetAttachComp)
	{
		WidgetAttachComp = GetOwner()->GetRootComponent();
	}
	InteractWidgetComp->AttachToComponent(WidgetAttachComp, FAttachmentTransformRules::KeepRelativeTransform);
}

void UERInteractableComponent::UpdateOutline(const bool bHighlight)
{
	for (UMeshComponent* OutlineMeshComponent : OutlineMeshComps)
	{
		if (OutlineMeshComponent)
		{
			OutlineMeshComponent->SetRenderCustomDepth(bHighlight);
			OutlineMeshComponent->SetCustomDepthStencilValue(bHighlight ? 1 : 0);
		}
	}
}
