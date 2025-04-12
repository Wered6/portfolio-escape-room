// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ERLockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnlockSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLockSignature);

UCLASS(ClassGroup=(Lock), meta=(BlueprintSpawnableComponent))
class ESCAPEROOM_API UERLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UERLockComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Unlock();
	void Lock();

	UFUNCTION(BlueprintGetter, Category="ER|Lock")
	FORCEINLINE bool GetIsLocked() const
	{
		return bIsLocked;
	}

	UPROPERTY(BlueprintAssignable, Category="ER|Lock")
	FOnUnlockSignature OnUnlockDelegate;
	UPROPERTY(BlueprintAssignable, Category="ER|Lock")
	FOnLockSignature OnLockDelegate;

private:
	void PopulateUnlockersFromTags();
	void BindUnlockers();

	/**
	 * Array of actors capable of unlocking the lock component.
	 */
	UPROPERTY(EditInstanceOnly, Category="ER|Lock")
	TArray<AActor*> Unlockers;

	/**
	 * List of tags used to identify actors that can unlock the lock component.
	 * Actors with matching tags will be added to the unlockers list.
	 */
	UPROPERTY(EditAnywhere, Category="ER|Lock")
	TArray<FName> UnlockersTags;

	/**
	 * Specifies whether the lock component is currently locked.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetIsLocked, Category="ER|Lock")
	bool bIsLocked{true};
};
