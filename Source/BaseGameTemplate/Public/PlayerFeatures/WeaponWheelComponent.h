// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "WeaponWheelComponent.generated.h"

/**
 * Tracks how the weapon wheel was opened, dictating closure behavior.
 * - L1Button: Requires L1 release (or explicit action) to close/select.
 * - MouseScroll: Starts an auto-hide timer that is CANCELLED upon the first
 * navigation.
 */
UENUM(BlueprintType)
enum class EWheelActivationSource : uint8 {
  None UMETA(DisplayName = "None"),
  L1Button UMETA(DisplayName = "L1 Button (Gamepad)"),
  MouseScroll UMETA(DisplayName = "Mouse Scroll (Auto-Hide)")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASEGAMETEMPLATE_API UWeaponWheelComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  UWeaponWheelComponent();

  /** True if the weapon wheel is currently open and active. */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Wheel")
  bool bIsWheelActive;

  /** The index of the currently selected weapon (0 to N-1). */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Wheel")
  int32 CurrentSelectionIndex;

  /** The total number of slots in the weapon wheel. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Wheel")
  int32 TotalWheelSlots = 8;

  // --- Gamepad Navigation Properties ---

  /** The required stick magnitude to trigger a navigation event. */
  UPROPERTY(EditAnywhere, Category = "Gamepad Navigation")
  float StickNavigationThreshold = 0.5f;

  /** Cooldown time between stick navigation events (to prevent instant
   * scrolling). */
  UPROPERTY(EditAnywhere, Category = "Gamepad Navigation")
  float StickNavigationCooldown = 0.25f;

  // --- Auto-Hide for Mouse Input ---

  /** How long (in seconds) the wheel remains open after the last scroll input
   * before auto-hiding. */
  UPROPERTY(EditDefaultsOnly, Category = "Weapon Wheel")
  float AutoHideDelay = 1.5f;

 protected:
  // Called when the game starts
  virtual void BeginPlay() override;

  /**
   * Helper function to change the index, handling wrap-around logic (0 to
   * TotalWheelSlots-1).
   * @param Direction - 1 for forward, -1 for backward.
   */
  void ChangeSelectionIndex(int32 Direction);

  /**
   * Checks if the stick input magnitude exceeds the threshold and the
   * cooldown has passed.
   * @param InputValue - The continuous float value from the stick axis.
   * @return The scroll direction (1 for forward, -1 for backward, 0 for
   * none).
   */
  int32 ProcessStickInput(float InputValue);

  /** Closes the wheel and executes the SelectWeapon logic. Bound to the
   * auto-hide timer. */
  UFUNCTION()
  void AutoHideWheel();

  /** Handle for the auto-hide timer. */
  FTimerHandle AutoHideTimerHandle;

  class UInventorySystemComponent* InventorySystemComponent;

 public:
  // Called every frame
  virtual void TickComponent(
      float DeltaTime, ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;

  // --- Public Functions for Character Input ---

  /**
   * Toggles the visibility and active state of the weapon wheel.
   * Called when the L1/Left Shoulder button is pressed/released.
   * @param bNewActive - True to show the wheel, False to hide it.
   */
  UFUNCTION(BlueprintCallable, Category = "Weapon Wheel")
  void ToggleWheel(bool bNewActive, EWheelActivationSource Source =
                                        EWheelActivationSource::None);
  /**
   * Navigates the wheel selection index.
   * Used for both Mouse Wheel and Gamepad Right Stick movement.
   * @param InputValue - Typically +1/-1 for mouse scroll, or continuous
   * float for stick.
   */
  UFUNCTION(BlueprintCallable, Category = "Weapon Wheel")
  void NavigateWheel(float InputValue);

  /**
   * Finalizes the selection and equips the weapon in the
   * CurrentSelectionIndex slot. Called when the L1/Toggle button is
   * released.
   */
  UFUNCTION(BlueprintCallable, Category = "Weapon Wheel")
  void SelectWeapon();

 private:
  /** Tracks the time since the last stick navigation event. */
  float LastNavigationTime;

  EWheelActivationSource ActivationSource = EWheelActivationSource::None;

  // --- Widget Handling ---

  /** The UMG widget class to spawn when the wheel is activated. */
  UPROPERTY(EditDefaultsOnly, Category = "Weapon Wheel|UI")
  TSubclassOf<class UWeaponWheelWidget> WeaponWheelWidgetClass;

  /** The spawned instance of the weapon wheel widget. */
  UPROPERTY()
  class UWeaponWheelWidget* WeaponWheelWidgetInstance;

  /** Attempts to load the widget class reference from the Asset path
   * (Blueprint). */
  void LoadWeaponWheelWidgetClass();
};
