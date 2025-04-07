// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ERLockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnlockSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLockSignature);

UCLASS(ClassGroup=(LockKey))
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
	void PopulateKeyItemsFromTags();
	void BindKeys();

	UPROPERTY(EditInstanceOnly, Category="ER|Lock")
	TArray<AActor*> KeyItems;

	UPROPERTY(EditAnywhere, Category="ER|Lock")
	TArray<FName> KeyTags;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetIsLocked, Category="ER|Lock")
	bool bIsLocked{true};
};
