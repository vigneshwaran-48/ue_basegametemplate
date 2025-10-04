// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InteractionSystem/InteractionData.h"
#include "InteractionSystem/OverlayPose.h"
#include "InventoryItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 {
  Consumable UMETA(DisplayName = "Consumable"),
  Weapon UMETA(DisplayName = "Weapon"),
  Healing UMETA(DisplayName = "Healing"),
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  FName ID;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  FText Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  FText Description;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  int32 MaxStackSize;

  UPROPERTY(BlueprintReadWrite, Category = "Item")
  int32 CurrentStackSize;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  class UStaticMesh* ItemMesh;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  class UTexture2D* Icon;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  EItemType Type;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  FInteractionData InteractionData;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
  EOverlayPose OverlayPose;
};
