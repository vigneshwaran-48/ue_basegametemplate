// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerFeatures/WeaponWheelComponent.h"

#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventorySystemComponent.h"
#include "PlayerFeatures/WeaponWheelWidget.h"

UWeaponWheelComponent::UWeaponWheelComponent() {
  PrimaryComponentTick.bCanEverTick = true;
  bIsWheelActive = false;
  CurrentSelectionIndex = 0;
  LastNavigationTime = 0.0f;
}

void UWeaponWheelComponent::BeginPlay() {
  Super::BeginPlay();

  AActor* OwnerActor = GetOwner();

  if (OwnerActor) {
    InventorySystemComponent =
        OwnerActor->GetComponentByClass<UInventorySystemComponent>();

    if (InventorySystemComponent) {
      UE_LOG(LogTemp, Log,
             TEXT("WeaponWheelComponent successfully found "
                  "InventorySystemComponent."));
    } else {
      UE_LOG(LogTemp, Error,
             TEXT("WeaponWheelComponent could not find "
                  "InventorySystemComponent on its owner (%s)."),
             *OwnerActor->GetName());
    }
  }
}

// Called every frame
void UWeaponWheelComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void UWeaponWheelComponent::ToggleWheel(bool bNewActive,
                                        EWheelActivationSource Source) {
  if (bNewActive == bIsWheelActive) {
    return;
  }

  bIsWheelActive = bNewActive;

  APawn* OwnerPawn = Cast<APawn>(GetOwner());
  if (!OwnerPawn) {
    return;
  }

  APlayerController* PlayerController =
      Cast<APlayerController>(OwnerPawn->GetController());
  if (!PlayerController) {
    return;
  }

  if (bIsWheelActive) {
    // Set the activation source upon opening
    ActivationSource = Source;

    // 1. Show Widget
    if (WeaponWheelWidgetClass) {
      WeaponWheelWidgetInstance = CreateWidget<UWeaponWheelWidget>(
          PlayerController, WeaponWheelWidgetClass);
      if (WeaponWheelWidgetInstance) {
        WeaponWheelWidgetInstance->AddToViewport();
        if (InventorySystemComponent) {
          TArray<FInventoryItem> Weapons =
              InventorySystemComponent->GetWeapons();
          WeaponWheelWidgetInstance->LoadWeaponItems(
              InventorySystemComponent->GetWeapons());
        }
      }
    }

    // 2. Set Input Mode to UI Only (or Game and UI)
    PlayerController->SetInputMode(FInputModeGameAndUI());
    // PlayerController->bShowMouseCursor = true;

    // 3. Reset selection to the first slot when opening
    CurrentSelectionIndex = 0;
    LastNavigationTime = GetWorld()->GetTimeSeconds();  // Reset cooldown timer

    // 4. Start Auto-Hide Timer ONLY if activated by mouse scroll
    if (ActivationSource == EWheelActivationSource::MouseScroll) {
      GetWorld()->GetTimerManager().SetTimer(
          AutoHideTimerHandle, this, &UWeaponWheelComponent::AutoHideWheel,
          AutoHideDelay, false);
    }
  } else {
    // 1. Clear Auto-Hide Timer (Crucial: prevents the timer from firing after
    // closing)
    GetWorld()->GetTimerManager().ClearTimer(AutoHideTimerHandle);

    // 2. Hide Widget
    if (WeaponWheelWidgetInstance) {
      WeaponWheelWidgetInstance->RemoveFromParent();
      WeaponWheelWidgetInstance = nullptr;
    }

    // 3. Set Input Mode back to Game Only
    PlayerController->SetInputMode(FInputModeGameOnly());
    PlayerController->bShowMouseCursor = false;

    // Reset source
    ActivationSource = EWheelActivationSource::None;
  }
}

void UWeaponWheelComponent::NavigateWheel(float InputValue) {
  if (!bIsWheelActive) {
    return;
  }

  // --- MOUSE SCROLL TIMER MANAGEMENT ---
  // If the wheel was opened by a scroll, any further scroll should reset the
  // auto-hide timer. This keeps the wheel open as long as the player is
  // actively scrolling.
  if (ActivationSource == EWheelActivationSource::MouseScroll) {
    // Reset the timer back to the full AutoHideDelay duration on every scroll
    // input.
    GetWorld()->GetTimerManager().SetTimer(
        AutoHideTimerHandle, this, &UWeaponWheelComponent::AutoHideWheel,
        AutoHideDelay, false);
  }

  // --- Gamepad Stick Logic ---
  if (FMath::Abs(InputValue) < 1.0f) {
    int32 Direction = ProcessStickInput(InputValue);
    if (Direction != 0) {
      ChangeSelectionIndex(Direction);
    }
  }
  // --- Mouse Wheel Logic ---
  else if (InputValue != 0.0f) {
    ChangeSelectionIndex(FMath::Sign(InputValue));
  }

  UE_LOG(LogTemp, Log, TEXT("Wheel Selection: %d"), CurrentSelectionIndex);
}

void UWeaponWheelComponent::SelectWeapon() {
  if (!bIsWheelActive) {
    return;
  }

  // 1. Finalize the selection
  UE_LOG(LogTemp, Warning, TEXT("Weapon selected: Slot %d"),
         CurrentSelectionIndex);

  // 2. Hide the wheel (handled by the Character on button release)
  ToggleWheel(false);

  // TODO: Add logic to actually equip the weapon corresponding to
  InventorySystemComponent->EquipWeapon(CurrentSelectionIndex);
}

void UWeaponWheelComponent::AutoHideWheel() {
  // This function is called when the timer runs out (i.e., when no new input
  // has occurred for AutoHideDelay seconds after opening or the last
  // navigation).
  SelectWeapon();      // Finalize selection
  ToggleWheel(false);  // Close the UI
}

// --- Helper Functions ---

void UWeaponWheelComponent::ChangeSelectionIndex(int32 Direction) {
  // Ensure the direction is -1 or 1
  Direction = FMath::Sign(Direction);

  // Calculate the new index
  int32 NewIndex = CurrentSelectionIndex + Direction;

  // Handle wrap-around logic (circular selection)
  if (NewIndex < 0) {
    NewIndex = TotalWheelSlots - 1;
  } else if (NewIndex >= TotalWheelSlots) {
    NewIndex = 0;
  }

  CurrentSelectionIndex = NewIndex;

  WeaponWheelWidgetInstance->SelectSlot(CurrentSelectionIndex);
}

int32 UWeaponWheelComponent::ProcessStickInput(float InputValue) {
  const float CurrentTime = GetWorld()->GetTimeSeconds();

  // Check cooldown
  if (CurrentTime < LastNavigationTime + StickNavigationCooldown) {
    return 0;  // Cooldown not yet finished
  }

  // Check threshold
  if (InputValue > StickNavigationThreshold) {
    LastNavigationTime = CurrentTime;
    return 1;  // Scroll forward
  } else if (InputValue < -StickNavigationThreshold) {
    LastNavigationTime = CurrentTime;
    return -1;  // Scroll backward
  }

  return 0;  // No scroll triggered
}
