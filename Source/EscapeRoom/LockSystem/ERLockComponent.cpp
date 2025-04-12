// Fill out your copyright notice in the Description page of Project Settings.


#include "ERLockComponent.h"
#include "ERUnlockerComponent.h"
#include "Kismet/GameplayStatics.h"


UERLockComponent::UERLockComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UERLockComponent::BeginPlay()
{
	Super::BeginPlay();

	PopulateUnlockersFromTags();
	BindUnlockers();
}

void UERLockComponent::Unlock()
{
	if (bIsLocked)
	{
		bIsLocked = false;
		OnUnlockDelegate.Broadcast();
	}
}

void UERLockComponent::Lock()
{
	if (!bIsLocked)
	{
		bIsLocked = true;
		OnLockDelegate.Broadcast();
	}
}

void UERLockComponent::PopulateUnlockersFromTags()
{
	if (UnlockersTags.IsEmpty())
	{
		return;
	}

	for (const FName Tag : UnlockersTags)
	{
		if (Tag.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s|%s: Tag is empty"), *GetOwner()->GetName(), *FString(__FUNCTION__))
			return;
		}

		TArray<AActor*> TaggedActors;
		UGameplayStatics::GetAllActorsWithTag(this, Tag, TaggedActors);

		Unlockers.Append(TaggedActors);
	}
}

void UERLockComponent::BindUnlockers()
{
	for (const AActor* Object : Unlockers)
	{
		if (!Object)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s|%s: Object is nullptr."), *GetOwner()->GetName(), TEXT(__FUNCTION__))
			return;
		}

		UERUnlockerComponent* UnlockerComponent{Object->FindComponentByClass<UERUnlockerComponent>()};
		if (UnlockerComponent)
		{
			UnlockerComponent->OnUnlockObjectsDelegate.AddUObject(this, &UERLockComponent::Unlock);
			UnlockerComponent->OnLockObjectsDelegate.AddUObject(this, &UERLockComponent::Lock);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: does not have UnlockerComponent"), *Object->GetName())
			return;
		}
	}
}
