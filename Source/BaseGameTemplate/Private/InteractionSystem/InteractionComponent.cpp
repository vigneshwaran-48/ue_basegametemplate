// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/InteractionComponent.h"

#include "BaseCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "InteractionSystem/InteractableInterface.h"
#include "InteractionSystem/InteractionData.h"
#include "InteractionSystem/InteractionState.h"
#include "InteractionSystem/InteractionSystemHelpers.h"
#include "InteractionSystem/InteractionType.h"
#include "InteractionSystem/InteractionWidget.h"
#include "Kismet/KismetSystemLibrary.h"

UInteractionComponent::UInteractionComponent() {
  PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay() {
  Super::BeginPlay();

  UActorComponent* interactionComp =
      GetOwner()->GetComponentByClass(UBoxComponent::StaticClass());
  if (interactionComp == nullptr) {
    UE_LOG(LogTemp, Warning,
           TEXT("InteractionCollision not present in the character"));
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
      UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit,
                                     true);
    }
    return;
  }
  UBoxComponent* interactionCollision = Cast<UBoxComponent>(interactionComp);
  interactionCollision->OnComponentBeginOverlap.AddDynamic(
      this, &UInteractionComponent::OnBoxBeginOverlap);
  interactionCollision->OnComponentEndOverlap.AddDynamic(
      this, &UInteractionComponent::OnBoxEndOverlap);
}

void UInteractionComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  AActor* bestInteractable = FindBestInteractable();

  if (bestInteractable != CurrentBestInteractable) {
    RemoveInteractionWidgetFromInteractable(CurrentBestInteractable);
    AddInteractionWidgetToInteractable(bestInteractable);
    CurrentBestInteractable = bestInteractable;
  }
}

AActor* UInteractionComponent::FindBestInteractable() {
  ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
  if (!ownerCharacter) {
    return nullptr;
  }
  FVector actorLocation = ownerCharacter->GetActorLocation();
  float capsuleHalfHeight =
      ownerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() *
      0.9f;

  FVector start = actorLocation + FVector(0, 0, capsuleHalfHeight);

  UCameraComponent* camera =
      GetOwner()->FindComponentByClass<UCameraComponent>();
  if (!camera) {
    UE_LOG(LogTemp, Log, TEXT("Camera Not found"));
    return nullptr;
  }

  FVector cameraDistance = camera->GetForwardVector() * InteractionDistance;
  FVector end = start + cameraDistance;

  FCollisionShape sphere = FCollisionShape::MakeSphere(InteractionRadius);
  FCollisionQueryParams params;
  params.AddIgnoredActor(GetOwner());

  TArray<FHitResult> hitResults;
  bool bHit = GetWorld()->SweepMultiByChannel(
      hitResults, start, end, FQuat::Identity,
      UEngineTypes::ConvertToCollisionChannel(TraceChannel), sphere, params);

  for (auto& hit : hitResults) {
    if (AActor* hitActor = hit.GetActor()) {
      if (!hitActor->GetClass()->ImplementsInterface(
              UInteractableInterface::StaticClass())) {
        continue;
      }
      return hitActor;
    }
  }
  return nullptr;
}

void UInteractionComponent::OnBoxBeginOverlap(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult) {
  if (OtherActor != nullptr && OtherActor->GetClass()->ImplementsInterface(
                                   UInteractableInterface::StaticClass())) {
    UE_LOG(LogTemp, Log, TEXT("Adding interactable"));
    NearbyInteractables.AddUnique(OtherActor);
    IInteractableInterface::Execute_Highlight(OtherActor, true);
  }
}

void UInteractionComponent::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp,
                                            AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp,
                                            int32 OtherBodyIndex) {
  if (OtherActor != nullptr && OtherActor->GetClass()->ImplementsInterface(
                                   UInteractableInterface::StaticClass())) {
    UE_LOG(LogTemp, Log, TEXT("Removing interactable"));
    NearbyInteractables.Remove(OtherActor);
    IInteractableInterface::Execute_Highlight(OtherActor, false);
  }
}

void UInteractionComponent::TryInteract() {
  if (!IsPlayAvailableForInteraction()) {
    UE_LOG(LogTemp, Log, TEXT("Already Interacting with something"));
    return;
  }
  if (!CurrentBestInteractable) {
    UE_LOG(LogTemp, Log, TEXT("No interactable in range"));
    return;
  }
  if (!CurrentBestInteractable->GetClass()->ImplementsInterface(
          UInteractableInterface::StaticClass())) {
    UE_LOG(LogTemp, Warning,
           TEXT("CurrentBestInteractable does not implement "
                "InteractableInterface"));
    return;
  }
  if (!IInteractableInterface::Execute_CanInteract(CurrentBestInteractable,
                                                   GetOwner())) {
    UE_LOG(LogTemp, Warning, TEXT("Unable to interact with the interactable"));
    return;
  }

  EInteractionType itemInteractionType =
      IInteractableInterface::Execute_GetInteractionType(
          CurrentBestInteractable);

  switch (itemInteractionType) {
    case EInteractionType::InventoryItem:
      HandleInventoryItemPickup(CurrentBestInteractable);
      break;
    case EInteractionType::PrimaryItem:
      HandlePrimaryItemPickup(CurrentBestInteractable);
      break;
    default:
      break;
  }
}

bool UInteractionComponent::IsPlayAvailableForInteraction() const {
  return InteractionState == EInteractionState::None;
}

void UInteractionComponent::AddInteractionWidgetToInteractable(
    AActor* Interactable) {
  if (Interactable == nullptr) {
    return;
  }
  if (!Interactable->GetClass()->ImplementsInterface(
          UInteractableInterface::StaticClass())) {
    return;
  }
  UActorComponent* interactableWidgetComponentActor =
      Interactable->GetComponentByClass(UWidgetComponent::StaticClass());
  if (!interactableWidgetComponentActor) {
    UE_LOG(LogTemp, Warning,
           TEXT("Interactable dosen't have a widget component"));
    return;
  }
  UWidgetComponent* interactableWidgetComponent =
      Cast<UWidgetComponent>(interactableWidgetComponentActor);
  if (!interactableWidgetComponent->GetWidget()) {
    UInteractionWidget* interactionWidget =
        CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
    interactableWidgetComponent->SetWidget(interactionWidget);
    FInventoryItem* item =
        GetItemById(IInteractableInterface::Execute_GetItemId(Interactable));
    if (!item) {
      InteractionSystemHelpers::LogAndDisplayDebugMessage(
          "Item is empty while getting the icon for displaying!", FColor::Red,
          5.0f);
      return;
    }
    UTexture2D* icon = item->Icon;
    if (icon) {
      interactionWidget->SetIcon(icon);
    } else {
      UE_LOG(LogTemp, Warning,
             TEXT("No icon is availble for the interactable to show"));
    }
  }
}

void UInteractionComponent::RemoveInteractionWidgetFromInteractable(
    AActor* Interactable) {
  if (Interactable == nullptr) {
    return;
  }
  if (!Interactable->GetClass()->ImplementsInterface(
          UInteractableInterface::StaticClass())) {
    return;
  }
  UActorComponent* interactableWidgetComponentActor =
      Interactable->GetComponentByClass(UWidgetComponent::StaticClass());
  if (!interactableWidgetComponentActor) {
    UE_LOG(LogTemp, Warning,
           TEXT("Interactable dosen't have a widget component"));
    return;
  }
  UWidgetComponent* interactableWidgetComponent =
      Cast<UWidgetComponent>(interactableWidgetComponentActor);
  interactableWidgetComponent->SetWidget(nullptr);
}

// Public methods

void UInteractionComponent::SetInteractionState(EInteractionState NewState) {
  InteractionState = NewState;
}

void UInteractionComponent::OnInteractionMontageEnded() {
  // Do Interaction Montage Ended Logic
}

void UInteractionComponent::AttachPickableItem() {
  if (LeftInteractingInteractable == nullptr) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("InteractingInteractable is null in AttachPickableItem"),
        FColor::Green, 5.0f);
    return;
  }
  UStaticMesh* interactableMesh =
      IInteractableInterface::Execute_GetMesh(LeftInteractingInteractable);

  if (!interactableMesh) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("Interactable mesh is null in AttachPickableItem"), FColor::Green,
        5.0f);
    return;
  }

  TArray<UActorComponent*> pickableLocatonMeshActors =
      GetOwner()->GetComponentsByTag(
          UStaticMeshComponent::StaticClass(),
          GetHandSocketName(InteractingInteractionType));
  if (!pickableLocatonMeshActors.IsValidIndex(0)) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("HandSocket not found in the character"), FColor::Green, 5.0f);
    return;
  }
  UStaticMeshComponent* pickableLocationMesh =
      Cast<UStaticMeshComponent>(pickableLocatonMeshActors[0]);

  pickableLocationMesh->SetStaticMesh(interactableMesh);

  switch (IInteractableInterface::Execute_GetInteractionType(
      LeftInteractingInteractable)) {
    case EInteractionType::InventoryItem:
      LeftInteractingInteractable->Destroy();
      LeftInteractingInteractable = nullptr;
      break;
    default:
      break;
  }
}

void UInteractionComponent::RemovePickableItem() {
  TArray<UActorComponent*> pickableLocatonMeshActors =
      GetOwner()->GetComponentsByTag(
          UStaticMeshComponent::StaticClass(),
          GetHandSocketName(InteractingInteractionType));
  if (!pickableLocatonMeshActors.IsValidIndex(0)) {
    UE_LOG(LogTemp, Warning, TEXT("HandSocket not found in the character"));
    return;
  }
  UStaticMeshComponent* pickableLocationMesh =
      Cast<UStaticMeshComponent>(pickableLocatonMeshActors[0]);

  pickableLocationMesh->SetStaticMesh(nullptr);

  switch (InteractingInteractionType) {
    case EInteractionType::InventoryItem:
      LeftHandState = EPlayerHandState::Free;
      break;
    default:
      break;
  }
}

void UInteractionComponent::InteractNotify() {
  if (LeftInteractingInteractable == nullptr) {
    UE_LOG(LogTemp, Warning,
           TEXT("InteractingInteractable is null in InteractNotify"));
    return;
  }
  IInteractableInterface::Execute_Interact(LeftInteractingInteractable,
                                           GetOwner());
}

void UInteractionComponent::PrimaryItemInteractNotify() {
  if (!RightInteractingInteractable) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("Right Hand Interacting Interactable is null"), FColor::Red, 5.0f);
  }
  IInteractableInterface::Execute_Interact(RightInteractingInteractable,
                                           GetOwner());
  ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
  if (!ownerCharacter) {
    return;
  }
  ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(ownerCharacter);
  if (!baseCharacter) {
    return;
  }
  HoldingPrimaryItemId =
      IInteractableInterface::Execute_GetItemId(RightInteractingInteractable);
  baseCharacter->OnPrimaryItemInteract(*GetItemById(HoldingPrimaryItemId));

  RightHandState = EPlayerHandState::HoldingPrimaryItem;
  RightInteractingInteractable->Destroy();
  RightInteractingInteractable = nullptr;
}

void UInteractionComponent::RemoveHoldingPrimaryItem() {
  if (RightHandState != EPlayerHandState::PuttingBackPrimaryItem) {
    return;
  }
  FInventoryItem* HoldingPrimaryItem = GetItemById(HoldingPrimaryItemId);
  if (!HoldingPrimaryItem) {
    UE_LOG(LogTemp, Log, TEXT("HoldingPrimaryItem is not present"));
    return;
  }
  ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(GetOwner());
  if (!baseCharacter) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("Owner is not ABaseCharacter"), FColor::Red, 5.0f);
    return;
  }
  baseCharacter->OnPrimaryItemPutback(*HoldingPrimaryItem);
  RightHandState = EPlayerHandState::Free;

  OnItemPutbackFinished.Broadcast();
}

void UInteractionComponent::PutBackHoldingPrimaryItem() {
  if (RightHandState != EPlayerHandState::HoldingPrimaryItem) {
    UE_LOG(LogTemp, Log, TEXT("RightHandState is not HoldingPrimaryItem"));
    return;
  }
  FInventoryItem* HoldingPrimaryItem = GetItemById(HoldingPrimaryItemId);
  if (!HoldingPrimaryItem) {
    UE_LOG(LogTemp, Log, TEXT("HoldingPrimaryItem is not present"));
    return;
  }
  FInteractionData interactionData = HoldingPrimaryItem->InteractionData;
  RightHandState = EPlayerHandState::PuttingBackPrimaryItem;

  Cast<ACharacter>(GetOwner())
      ->PlayAnimMontage(interactionData.PutbackMontage, 1.0f);
}

void UInteractionComponent::EquipPrimaryItem(FInventoryItem item) {
  if (RightHandState == EPlayerHandState::PickingItem ||
      RightHandState == EPlayerHandState::PuttingBackPrimaryItem) {
    UE_LOG(LogTemp, Warning, TEXT("Right hand is busy"));
    return;
  }
  if (RightHandState == EPlayerHandState::HoldingPrimaryItem &&
      GetItemById(HoldingPrimaryItemId)) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("Putting back item before picking the primary item"),
        FColor::Green, 5.0f);
    auto PutbackFinishedHandler = [this, item]() {
      OnItemPutbackFinished.Remove(PendingPutbackHandle);
      HandleEquipPrimaryItem(item);
      PendingPutbackHandle.Reset();
    };
    PendingPutbackHandle =
        OnItemPutbackFinished.AddLambda(PutbackFinishedHandler);
    PutBackHoldingPrimaryItem();
    return;
  }
  HandleEquipPrimaryItem(item);
}

void UInteractionComponent::EquipPrimaryItemNotify() {
  if (EquippingItem.ID.IsNone()) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("Equipping item is null!"), FColor::Red, 5.0f);
    return;
  }
  ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
  if (!ownerCharacter) {
    return;
  }
  ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(ownerCharacter);
  if (!baseCharacter) {
    return;
  }
  baseCharacter->OnPrimaryItemEquip(EquippingItem);

  RightHandState = EPlayerHandState::HoldingPrimaryItem;
  EquippingItem = FInventoryItem();
}

// Utility Functions

void UInteractionComponent::HandleInventoryItemPickup(AActor* Interactable) {
  if (LeftHandState == EPlayerHandState::PickingItem) {
    UE_LOG(LogTemp, Warning,
           TEXT("Left hand is already picking something, can't pick another "
                "item"));
    return;
  }
  FInteractionData interactionData =
      GetItemById(IInteractableInterface::Execute_GetItemId(Interactable))
          ->InteractionData;
  if (!interactionData.PickupMontage) {
    if (GEngine) {
      GEngine->AddOnScreenDebugMessage(
          -1,    // Key (-1 = always add new, otherwise replaces existing with
                 // same key)
          5.0f,  // Time to display (seconds)
          FColor::Green,  // Text color
          TEXT("Animation montage is not present in the interaction data given "
               "by the interactable")  // The message
      );
    } else {
      UE_LOG(LogTemp, Warning,
             TEXT("Animation montage is not present in the interaction data "
                  "given by the interactable"));
    }
    return;
  }
  Cast<ACharacter>(GetOwner())
      ->PlayAnimMontage(interactionData.PickupMontage, 2.0f);

  LeftHandState = EPlayerHandState::PickingItem;

  LeftInteractingInteractable = Interactable;
  InteractingInteractionType =
      IInteractableInterface::Execute_GetInteractionType(Interactable);
}

FName UInteractionComponent::GetHandSocketName(
    EInteractionType InteractionType) const {
  switch (InteractionType) {
    case EInteractionType::InventoryItem:
      return FName("LeftHandSocket");
    default:
      return FName("None");
  }
}

void UInteractionComponent::HandlePrimaryItemPickup(AActor* Interactable) {
  // Need to handle putting existing holding weapon to bag logic here
  if (RightHandState == EPlayerHandState::PickingItem ||
      RightHandState == EPlayerHandState::PuttingBackPrimaryItem) {
    UE_LOG(LogTemp, Warning, TEXT("Right hand is busy"));
    return;
  }

  if (RightHandState == EPlayerHandState::HoldingPrimaryItem &&
      GetItemById(HoldingPrimaryItemId)) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("Putting back item before picking the primary item"),
        FColor::Green, 5.0f);
    auto PutbackFinishedHandler = [this, Interactable]() {
      OnItemPutbackFinished.Remove(PendingPutbackHandle);
      PickupPrimaryItem(Interactable);
      PendingPutbackHandle.Reset();
    };
    PendingPutbackHandle =
        OnItemPutbackFinished.AddLambda(PutbackFinishedHandler);
    PutBackHoldingPrimaryItem();
    return;
  }
  PickupPrimaryItem(Interactable);
}

void UInteractionComponent::PickupPrimaryItem(AActor* Interactable) {
  FInteractionData interactionData =
      GetItemById(IInteractableInterface::Execute_GetItemId(Interactable))
          ->InteractionData;
  if (!interactionData.PickupMontage) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("Animation montage is not present in interaction data"),
        FColor::Green, 5.0f);
    return;
  }
  RightHandState = EPlayerHandState::PickingItem;
  Cast<ACharacter>(GetOwner())
      ->PlayAnimMontage(interactionData.PickupMontage, 1.0f);
  RightInteractingInteractable = Interactable;
}

FInventoryItem* UInteractionComponent::GetItemById(FName Id) {
  if (!ItemDataTable) {
    UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is empty"));
    return nullptr;
  }
  return ItemDataTable->FindRow<FInventoryItem>(
      Id, TEXT("ItemID not found in DataTable"));
}

void UInteractionComponent::HandleEquipPrimaryItem(FInventoryItem item) {
  FInteractionData interactionData = item.InteractionData;
  if (!interactionData.EquipMontage) {
    InteractionSystemHelpers::LogAndDisplayDebugMessage(
        TEXT("Equip montage is not present in interaction data"), FColor::Green,
        5.0f);
    return;
  }
  RightHandState = EPlayerHandState::PickingItem;
  EquippingItem = item;
  Cast<ACharacter>(GetOwner())
      ->PlayAnimMontage(interactionData.EquipMontage, 1.0f);
}
