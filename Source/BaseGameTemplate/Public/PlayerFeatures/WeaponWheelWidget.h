// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "InventorySystem/InventoryItem.h"
#include "WeaponWheelWidget.generated.h"

/**
 *
 */
UCLASS()
class BASEGAMETEMPLATE_API UWeaponWheelWidget : public UUserWidget {
  GENERATED_BODY()

 public:
  /** * Reference to the Weapon Wheel Component that owns this UI.
   * This is set immediately after the widget is created in the C++
   * component.
   */
  UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon Wheel")
  TObjectPtr<class UWeaponWheelComponent> OwningWeaponWheelComponent;

  UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Weapon Wheel")
  TArray<class UImage*> Slots;

  UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Weapon Wheel")
  TArray<class UImage*> SlotIcons;

  // --- C++ to Blueprint Communication ---

  /**
   * Called by the C++ component whenever the selection index changes.
   * The Blueprint should implement this to visually update the
   * highlighted slot.
   * * @param NewIndex The index (0 to N-1) of the item now selected.
   */
  UFUNCTION(BlueprintCallable, Category = "Weapon Wheel")
  void SelectSlot(int32 NewIndex);

  /**
   * Called by the C++ component when the wheel is closed (after
   * selection). The Blueprint can use this for any closing animations or
   * final cleanup.
   */
  UFUNCTION(BlueprintCallable, Category = "Weapon Wheel")
  void OnWheelClosed();

  UFUNCTION(BlueprintCallable, Category = "Weapon Wheel")
  void LoadWeaponItems(const TArray<struct FInventoryItem>& Items);

 private:
  class UImage* CurrentSelectedSlot;

 protected:
  virtual void NativeConstruct() override;
};
