// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnToggleWeaponHold);

UCLASS()
class BASEGAMETEMPLATE_API ABaseCharacter : public ACharacter {
  GENERATED_BODY()

 public:
  // Sets default values for this character's properties
  ABaseCharacter();

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction System")
  class UInteractionComponent* InteractionComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System")
  class UInventorySystemComponent* InventorySystemComponent;

  /** The dedicated component for handling the weapon wheel logic. */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponWheel")
  class UWeaponWheelComponent* WeaponWheelComp;

  UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
  class UInputAction* InteractionAction;

  UPROPERTY(EditDefaultsOnly, Category = "Weapon Wheel")
  class UInputAction* ToggleWeaponWheel;

  /** Input Action for scrolling (discrete or continuous axis). Mapped to Mouse
   * Wheel and Right Stick. */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Wheel")
  UInputAction* NavigateWheelAction;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon System")
  UInputAction* ToggleWeaponHold;

  UFUNCTION()
  void HandleInteract(const FInputActionValue& Value);

  /** Called when the Weapon Wheel Toggle button (L1/Left Shoulder) is pressed.
   */
  void OnToggleWheelPressed();

  /** Called when the Weapon Wheel Toggle button (L1/Left Shoulder) is released.
   */
  void OnToggleWheelReleased();

  /**
   * Called on Mouse Wheel Scroll (discrete) or Gamepad Right Stick movement
   * (continuous). Navigates the wheel selection.
   */
  void HandleWheelNavigation(const FInputActionValue& Value);

  void HandleToggleWeaponHold();

  // --- Gamepad State ---

  /** Stores the last input from the Right Stick (Horizontal or Vertical). */
  float LastRightStickInput;

 public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(
      class UInputComponent* PlayerInputComponent) override;

  UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
  void OnPrimaryItemInteract(FInventoryItem Interactable);

  UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
  void OnPrimaryItemPutback(FInventoryItem InventoryItem);

  UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
  void OnPrimaryItemEquip(FInventoryItem InventoryItem);

  FOnToggleWeaponHold OnToggleWeaponHold;
};
