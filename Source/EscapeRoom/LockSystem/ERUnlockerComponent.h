// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ERUnlockerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnUnlockObjectsSignature)
DECLARE_MULTICAST_DELEGATE(FOnLockObjectsSignature)

/**
 * A component that can unlock and lock objects that have LockComponent.
 */
UCLASS(ClassGroup=(Lock), meta=(BlueprintSpawnableComponent), DisplayName="Unlocker Component")
class ESCAPEROOM_API UERUnlockerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UERUnlockerComponent();

	UFUNCTION(BlueprintCallable, Category="ER|Unlocker")
	void UnlockObjects();
	UFUNCTION(BlueprintCallable, Category="ER|Unlocker")
	void LockObjects();

	FOnUnlockObjectsSignature OnUnlockObjectsDelegate;
	FOnLockObjectsSignature OnLockObjectsDelegate;
};
