// Fill out your copyright notice in the Description page of Project Settings.


#include "ERCabinet.h"
#include "EscapeRoom/Utility/WeredMacros.h"


AERCabinet::AERCabinet()
{
	PrimaryActorTick.bCanEverTick = false;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	BodyMesh->SetCollisionProfileName(TEXT("BlockAll"));
	SetRootComponent(BodyMesh);
}

void AERCabinet::BeginPlay()
{
	Super::BeginPlay();

	AttachDoorsAndDrawers();
}

void AERCabinet::AttachDoorsAndDrawers()
{
	UVALID_LOG_DEBUG(DoorLeft)
	UVALID_LOG_DEBUG(DoorRight)
	UVALID_LOG_DEBUG(DrawerFirst)
	UVALID_LOG_DEBUG(DrawerSecond)
	UVALID_LOG_DEBUG(DrawerThird)

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::KeepWorld, true};
	DoorLeft->AttachToActor(this, AttachmentTransformRules);
	DoorRight->AttachToActor(this, AttachmentTransformRules);
	DrawerFirst->AttachToActor(this, AttachmentTransformRules);
	DrawerSecond->AttachToActor(this, AttachmentTransformRules);
	DrawerThird->AttachToActor(this, AttachmentTransformRules);
}
