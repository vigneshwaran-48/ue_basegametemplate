// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionData.generated.h"

USTRUCT(BlueprintType)
struct FInteractionData {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
  TObjectPtr<class UAnimMontage> PickupMontage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
  TObjectPtr<class UAnimMontage> PutbackMontage;

  FInteractionData() : PickupMontage(nullptr), PutbackMontage(nullptr) {}

  FInteractionData(TObjectPtr<class UAnimMontage> InPickupMontage)
      : PickupMontage(InPickupMontage), PutbackMontage(nullptr) {}

  FInteractionData(TObjectPtr<class UAnimMontage> InPickupMontage,
                   TObjectPtr<class UAnimMontage> InPutbackMontage)
      : PickupMontage(InPickupMontage), PutbackMontage(InPutbackMontage) {}
};
