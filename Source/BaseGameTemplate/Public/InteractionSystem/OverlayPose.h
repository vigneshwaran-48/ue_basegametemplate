// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EOverlayPose : uint8 {
  ThrowableBottle UMETA(DisplayName = "Throwable Bottle"),
  ThrowableBrick UMETA(DisplayName = "Throwable Brick"),
  Pistol1H UMETA(DisplayName = "Pistol 1 Hand"),
  Pistol2H UMETA(DisplayName = "Pistol 2 Hand"),
  Rifle UMETA(DisplayName = "Rifle"),
  Box UMETA(DisplayName = "Box"),
  Bow UMETA(DisplayName = "Bow"),
  Binoculars UMETA(DisplayName = "Binoculars"),
};
