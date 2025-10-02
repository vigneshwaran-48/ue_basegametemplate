// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InteractionSystem/InteractionComponent.h"
#include "InventorySystem/InventorySystemComponent.h"
#include "PlayerFeatures/WeaponWheelComponent.h"

ABaseCharacter::ABaseCharacter() {
  PrimaryActorTick.bCanEverTick = true;

  InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(
      TEXT("InteractionComponent"));
  InventorySystemComponent = CreateDefaultSubobject<UInventorySystemComponent>(
      TEXT("InventorySystemComponent"));

  // Create and initialize the Weapon Wheel Component
  WeaponWheelComp = CreateDefaultSubobject<UWeaponWheelComponent>(
      TEXT("WeaponWheelComponent"));

  LastRightStickInput = 0.0f;
}

void ABaseCharacter::BeginPlay() { Super::BeginPlay(); }

void ABaseCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  // --- Gamepad Navigation Logic in Tick ---
  // Continuously feed the right stick input to the component for navigation
  // processing
  if (WeaponWheelComp && WeaponWheelComp->bIsWheelActive &&
      FMath::Abs(LastRightStickInput) > KINDA_SMALL_NUMBER) {
    // Use the absolute value of the stick input if you want to allow both X
    // and Y axis to scroll For simplicity, we assume one axis is mapped to
    // the input function in SetupPlayerInputComponent.
    WeaponWheelComp->NavigateWheel(LastRightStickInput);
  }
}

void ABaseCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  if (UEnhancedInputComponent* EnhancedInput =
          Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    if (InteractionAction) {
      EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Started, this,
                                &ABaseCharacter::HandleInteract);
    }

    if (ToggleWeaponWheel) {
      EnhancedInput->BindAction(ToggleWeaponWheel, ETriggerEvent::Started, this,
                                &ABaseCharacter::OnToggleWheelPressed);
      EnhancedInput->BindAction(ToggleWeaponWheel, ETriggerEvent::Completed,
                                this, &ABaseCharacter::OnToggleWheelReleased);
    }

    if (NavigateWheelAction) {
      EnhancedInput->BindAction(NavigateWheelAction, ETriggerEvent::Triggered,
                                this, &ABaseCharacter::HandleWheelNavigation);
    }

    if (ToggleWeaponHold) {
      EnhancedInput->BindAction(ToggleWeaponHold, ETriggerEvent::Started, this,
                                &ABaseCharacter::HandleToggleWeaponHold);
    }
  }
}

// --- INPUT HANDLER FUNCTIONS ---

void ABaseCharacter::HandleInteract(const FInputActionValue& Value) {
  if (InteractionComponent) {
    InteractionComponent->TryInteract();
  }
}

void ABaseCharacter::OnToggleWheelPressed() {
  if (WeaponWheelComp) {
    const EWheelActivationSource Source = EWheelActivationSource::L1Button;

    // Open the wheel when L1/Toggle is pressed
    WeaponWheelComp->ToggleWheel(true, Source);
  }
}

void ABaseCharacter::OnToggleWheelReleased() {
  if (WeaponWheelComp) {
    // This action is always tied to the L1 Button logic (hold to open, release
    // to select/close)
    const EWheelActivationSource Source = EWheelActivationSource::L1Button;

    // Select weapon and close the wheel when L1/Toggle is released
    WeaponWheelComp->SelectWeapon();
    WeaponWheelComp->ToggleWheel(false, Source);
  }
}

void ABaseCharacter::HandleWheelNavigation(const FInputActionValue& Value) {
  const float ScrollValue = Value.Get<float>();

  if (WeaponWheelComp && ScrollValue != 0.0f) {
    // If the input is from the Right Stick, store the continuous value for Tick
    // processing. If the input is from the Mouse Wheel, the scroll value will
    // be discrete (+1.0 or -1.0).

    if (FMath::Abs(ScrollValue) <
        1.0f)  // Heuristic check: continuous input (stick) is typically < 1.0
    {
      LastRightStickInput = ScrollValue;
    } else  // Discrete input (mouse wheel) is typically exactly 1.0 or -1.0
    {
      // --- Mouse Wheel Logic ---
      const EWheelActivationSource Source = EWheelActivationSource::MouseScroll;

      // --- Mouse Wheel Logic ---
      // If the wheel is not active, open it immediately upon scrolling.
      if (!WeaponWheelComp->bIsWheelActive) {
        WeaponWheelComp->ToggleWheel(true, Source);
      }

      // Pass the discrete scroll value directly for navigation
      WeaponWheelComp->NavigateWheel(ScrollValue);
    }
  }
}

void ABaseCharacter::HandleToggleWeaponHold() {
  // Need to handle actual toggling of holding state in future. Currently only
  // putting back the weapon.
  if (InteractionComponent) {
    InteractionComponent->PutBackHoldingPrimaryItem();
  }
}
