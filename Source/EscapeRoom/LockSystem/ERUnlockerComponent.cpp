// Fill out your copyright notice in the Description page of Project Settings.


#include "ERUnlockerComponent.h"


UERUnlockerComponent::UERUnlockerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UERUnlockerComponent::UnlockItems()
{
	OnUnlockItemsDelegate.Broadcast();
}

void UERUnlockerComponent::LockItems()
{
	OnLockItemsDelegate.Broadcast();
}
