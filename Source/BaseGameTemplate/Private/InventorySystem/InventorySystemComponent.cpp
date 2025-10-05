// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/InventorySystemComponent.h"

#include "InteractionSystem/InteractionComponent.h"
#include "InventorySystem/InventoryItem.h"

// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent() {
  // Set this component to be initialized when the game starts, and to be ticked
  // every frame.  You can turn these features off to improve performance if you
  // don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  // ...
}

// Called when the game starts
void UInventorySystemComponent::BeginPlay() {
  Super::BeginPlay();

  AActor* OwnerActor = GetOwner();

  if (OwnerActor) {
    InteractionComponent =
        OwnerActor->GetComponentByClass<UInteractionComponent>();
    if (InteractionComponent) {
      UE_LOG(LogTemp, Log,
             TEXT("InventoryComponent successfully found "
                  "InteractionComponent."));
    } else {
      UE_LOG(LogTemp, Error,
             TEXT("InventoryComponent could not find "
                  "InteractionComponent on its owner (%s)."),
             *OwnerActor->GetName());
    }
  }
}

// Called every frame
void UInventorySystemComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

bool UInventorySystemComponent::AddItem(FInventoryItem Item, int32 Quantity) {
  for (FInventoryItem& Slot : InventorySlots) {
    // For now, let's just add it if the item ID matches and it's stackable.
    // A more complex system would handle partial stacks.
    if (Slot.ID == Item.ID) {
      if (Slot.CurrentStackSize + Quantity > Slot.MaxStackSize) {
        return false;
      }
      Slot.CurrentStackSize += Quantity;
      OnInventoryUpdated.Broadcast();
      return true;
    }
  }

  if (InventorySlots.Num() < MaxInventorySlots)  // Example max slots
  {
    FInventoryItem NewItem = Item;
    NewItem.CurrentStackSize = Quantity;
    InventorySlots.Add(NewItem);
    OnInventoryUpdated.Broadcast();
    return true;
  }

  return false;  // Inventory is full or something went wrong
}

bool UInventorySystemComponent::IsSpaceAvailableForItem(FInventoryItem Item,
                                                        int32 Quantity) {
  int32 TotalAvailableSpace = 0;
  bool itemSlotExists = false;
  for (const FInventoryItem& Slot : InventorySlots) {
    if (Slot.ID == Item.ID) {
      TotalAvailableSpace += (Slot.MaxStackSize - Slot.CurrentStackSize);
      itemSlotExists = true;
    }
  }
  if (itemSlotExists) {
    return TotalAvailableSpace >= Quantity;
  }
  int32 EmptySlots = MaxInventorySlots - InventorySlots.Num();
  return EmptySlots > 0;
}

TArray<FInventoryItem> UInventorySystemComponent::GetWeapons() const {
  TArray<FInventoryItem> Weapons;
  for (const FInventoryItem& Item : InventorySlots) {
    if (Item.Type == EItemType::Weapon) {
      Weapons.Add(Item);
    }
  }
  return Weapons;
}

void UInventorySystemComponent::EquipWeapon(int weaponSlotIndex) {
  TArray<FInventoryItem> weapons = GetWeapons();
  if (weaponSlotIndex < 0 || weaponSlotIndex >= weapons.Num()) {
    UE_LOG(LogTemp, Warning, TEXT("Invalid weapon slot index"));
    return;
  }
  FInventoryItem& weaponToEquip = weapons[weaponSlotIndex];
  InteractionComponent->EquipPrimaryItem(weaponToEquip);
}
