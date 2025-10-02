// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInteractionState : uint8 {
  None UMETA(DisplayName = "None"),
  Picking UMETA(DisplayName = "Picking"),
  Holding UMETA(DisplayName = "Holding"),
  Reading UMETA(DisplayName = "Reading"),
};

UENUM(BlueprintType)
enum class EPlayerHandState : uint8 {
  /** Hand is completely free. */
  Free UMETA(DisplayName = "Free"),

  /** Hand holds a permanent utility item (Weapon, Torch, Flashlight). */
  HoldingPrimaryItem UMETA(DisplayName = "Holding Utility"),

  /** Hand holds a temporary throwable/holdable item (Brick, Bottle). */
  HoldingTemporaryHoldable UMETA(DisplayName = "Holding Holdable"),

  PickingItem UMETA(DisplayName = "Picking Item"),

  PuttingBackPrimaryItem UMETA(DisplayName = "Putting Back Primary Item"),
};
