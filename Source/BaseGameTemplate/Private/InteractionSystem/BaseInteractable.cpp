// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/BaseInteractable.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DataTable.h"
#include "InteractionSystem/InteractionData.h"
#include "InteractionSystem/InteractionInputType.h"
#include "InteractionSystem/InteractionType.h"
#include "InventorySystem/InventoryItem.h"
#include "InventorySystem/InventorySystemComponent.h"

ABaseInteractable::ABaseInteractable() {
  PrimaryActorTick.bCanEverTick = true;

  SceneRoot =
      AActor::CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
  RootComponent = SceneRoot;

  StaticMesh =
      AActor::CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
  StaticMesh->SetupAttachment(SceneRoot);

  InteractionBox =
      AActor::CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
  InteractionBox->SetupAttachment(StaticMesh);

  InteractionBox->InitBoxExtent(FVector(50.f, 50.f, 50.f));

  InteractionWidget = AActor::CreateDefaultSubobject<UWidgetComponent>(
      TEXT("InteractionWidget"));
  InteractionWidget->SetupAttachment(RootComponent);
  InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts or when spawned
void ABaseInteractable::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ABaseInteractable::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// TODO: Use the CanInteract to check whether inventory is full or not before
// calling this.
void ABaseInteractable::Interact_Implementation(AActor* InteractingActor) {
  UActorComponent* InventorySystemActor = InteractingActor->GetComponentByClass(
      UInventorySystemComponent::StaticClass());
  UInventorySystemComponent* InventorySystem;
  if (InventorySystemActor) {
    InventorySystem = Cast<UInventorySystemComponent>(InventorySystemActor);
  } else {
    return;
  }
  if (InventorySystem) {
    FInventoryItem* Item = ItemDataTable->FindRow<FInventoryItem>(
        ItemID, TEXT("ItemID not found in DataTable"));
    if (!Item) {
      UE_LOG(LogTemp, Warning, TEXT("Item not found in the datatable"));
      return;
    }
    InventorySystem->AddItem(*Item, 1);
  }
}

FText ABaseInteractable::GetInteractionText_Implementation() const {
  return FText::FromString("Base Interactable");
}

EInteractionType ABaseInteractable::GetInteractionType_Implementation() const {
  return InteractionType;
}

void ABaseInteractable::Highlight_Implementation(bool highlight) {
  StaticMesh->SetRenderCustomDepth(highlight);
}

UStaticMesh* ABaseInteractable::GetMesh_Implementation() const {
  return StaticMesh->GetStaticMesh();
}

EInteractionInputType
ABaseInteractable::GetInteractionInputType_Implementation() const {
  return EInteractionInputType::Press;
}

bool ABaseInteractable::CanInteract_Implementation(
    AActor* InteractingActor) const {
  UActorComponent* InventorySystemActor = InteractingActor->GetComponentByClass(
      UInventorySystemComponent::StaticClass());
  UInventorySystemComponent* InventorySystem;
  if (InventorySystemActor) {
    InventorySystem = Cast<UInventorySystemComponent>(InventorySystemActor);
  } else {
    return false;
  }
  if (!ItemDataTable) {
    UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is empty"));
    return false;
  }
  FInventoryItem* Item = ItemDataTable->FindRow<FInventoryItem>(
      ItemID, TEXT("ItemID not found in DataTable"));
  if (InventorySystem->IsSpaceAvailableForItem(*Item, 1)) {
    return true;
  }
  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(
        -1,    // Key (-1 = always add new, otherwise replaces existing with
               // same key)
        5.0f,  // Time to display (seconds)
        FColor::Red,                             // Text color
        TEXT("Inventory Full in can interact!")  // The message
    );
  }
  return false;
}

FInteractionData ABaseInteractable::GetInteractionData_Implementation() const {
  return InteractionData;
}

UTexture2D* ABaseInteractable::GetIcon_Implementation() const {
  return InteractionIcon;
}

FName ABaseInteractable::GetItemId_Implementation() const { return ItemID; }
