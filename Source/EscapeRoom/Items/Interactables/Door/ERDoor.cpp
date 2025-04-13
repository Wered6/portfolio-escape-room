// Fill out your copyright notice in the Description page of Project Settings.


#include "ERDoor.h"
#include "EscapeRoom/InteractionSystem/ERInteractableComponent.h"
#include "EscapeRoom/LockSystem/ERLockComponent.h"


AERDoor::AERDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	FrameMesh->SetCollisionProfileName(TEXT("BlockAll"));
	SetRootComponent(FrameMesh);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetCollisionProfileName(TEXT("BlockAll"));
	DoorMesh->SetupAttachment(FrameMesh);

	HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMesh"));
	HandleMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HandleMesh->SetupAttachment(DoorMesh);

	LockComponent = CreateDefaultSubobject<UERLockComponent>(TEXT("LockComponent"));

	InteractableComponent->AddOutlineMeshComponent(HandleMesh);
}

void AERDoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InteractableComponent->InteractCategory = LockComponent->GetIsLocked() ? EERInteractCategory::Unlock : EERInteractCategory::Open;
}

void AERDoor::BeginPlay()
{
	Super::BeginPlay();

	LockComponent->OnUnlockDelegate.AddDynamic(this, &AERDoor::OnUnlockHandle);
}

void AERDoor::OnUnlockHandle()
{
	InteractableComponent->SetInteractCategory(EERInteractCategory::Open);
}
