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

	PopulateKeyItemsFromTags();
	BindKeys();
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

void UERLockComponent::PopulateKeyItemsFromTags()
{
	if (KeyTags.IsEmpty())
	{
		return;
	}

	for (const FName Tag : KeyTags)
	{
		if (Tag.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s|%s: Tag is empty"), *GetOwner()->GetName(), *FString(__FUNCTION__))
			return;
		}

		TArray<AActor*> TaggedActors;
		UGameplayStatics::GetAllActorsWithTag(this, Tag, TaggedActors);

		KeyItems.Append(TaggedActors);
	}
}

void UERLockComponent::BindKeys()
{
	for (const AActor* Item : KeyItems)
	{
		if (!Item)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s|%s: Item is nullptr."), *GetOwner()->GetName(), *FString(__FUNCTION__))
			return;
		}

		UERUnlockerComponent* KeyComponent{Item->FindComponentByClass<UERUnlockerComponent>()};
		if (KeyComponent)
		{
			KeyComponent->OnUnlockItemsDelegate.AddUObject(this, &UERLockComponent::Unlock);
			KeyComponent->OnLockItemsDelegate.AddUObject(this, &UERLockComponent::Lock);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: does not have ERKeyComponent"), *Item->GetName())
			return;
		}
	}
}
