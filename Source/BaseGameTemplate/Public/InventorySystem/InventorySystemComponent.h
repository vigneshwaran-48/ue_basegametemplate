// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "InventorySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASEGAMETEMPLATE_API UInventorySystemComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  UInventorySystemComponent();

  UPROPERTY(BlueprintAssignable, Category = "Inventory")
  FOnInventoryUpdated OnInventoryUpdated;

  // Array to hold the items. Use FInventoryItem as the data structure.
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
  TArray<struct FInventoryItem> InventorySlots;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
  int32 MaxInventorySlots = 10;

  /**
   * @brief Adds an item to the inventory.
   * @param Item The item to add.
   * @param Quantity The amount of the item to add.
   * @return True if the item was added successfully.
   */
  UFUNCTION(BlueprintCallable, Category = "Inventory")
  bool AddItem(FInventoryItem Item, int32 Quantity);

  UFUNCTION(BlueprintCallable, Category = "Inventory")
  bool IsSpaceAvailableForItem(FInventoryItem Item, int32 Quantity);

 protected:
  // Called when the game starts
  virtual void BeginPlay() override;

 public:
  // Called every frame
  virtual void TickComponent(
      float DeltaTime, ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;

  TArray<struct FInventoryItem> GetWeapons() const;
};
