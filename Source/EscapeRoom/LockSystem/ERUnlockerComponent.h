// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ERUnlockerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnUnlockItemsSignature)
DECLARE_MULTICAST_DELEGATE(FOnLockItemsSignature)

UCLASS(ClassGroup=(LockKey))
class ESCAPEROOM_API UERUnlockerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UERUnlockerComponent();

	void UnlockItems();
	void LockItems();

	FOnUnlockItemsSignature OnUnlockItemsDelegate;
	FOnLockItemsSignature OnLockItemsDelegate;
};
