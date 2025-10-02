#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8 {
  /** Item that goes straight to the Inventory component (ammo, junk, medkit).
   */
  InventoryItem UMETA(DisplayName = "Inventory Item"),

  /** Utility item that equips permanently to the Left Hand (Torch, Flashlight,
     Small Shield). */
  LeftHandUtility UMETA(DisplayName = "Left Hand Utility"),

  // Primary Item that will be attached to the right hand
  PrimaryItem UMETA(DisplayName = "Primary Item"),

  // Temporary Holdable objects that will be holded in the right hand
  TemporaryHoldable UMETA(DisplayName = "Temporary Holdable"),

  /** An environmental object for action (Door, Valve, Climb Point). */
  ComplexInteraction UMETA(DisplayName = "Complex Interaction"),
};
