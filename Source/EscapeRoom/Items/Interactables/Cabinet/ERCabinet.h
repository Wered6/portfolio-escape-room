// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ERCabinet.generated.h"

UCLASS()
class ESCAPEROOM_API AERCabinet : public AActor
{
	GENERATED_BODY()

public:
	AERCabinet();

protected:
	virtual void BeginPlay() override;

private:
	void AttachDoorsAndDrawers();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(EditInstanceOnly, Category="ER|Cabinet")
	TObjectPtr<AActor> DoorLeft;
	UPROPERTY(EditInstanceOnly, Category="ER|Cabinet")
	TObjectPtr<AActor> DoorRight;
	UPROPERTY(EditInstanceOnly, Category="ER|Cabinet")
	TObjectPtr<AActor> DrawerFirst;
	UPROPERTY(EditInstanceOnly, Category="ER|Cabinet")
	TObjectPtr<AActor> DrawerSecond;
	UPROPERTY(EditInstanceOnly, Category="ER|Cabinet")
	TObjectPtr<AActor> DrawerThird;
};
