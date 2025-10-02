// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "RemoveHoldingPrimaryItem.generated.h"

/**
 *
 */
UCLASS()
class BASEGAMETEMPLATE_API URemoveHoldingPrimaryItem : public UAnimNotify {
  GENERATED_BODY()

 public:
  virtual void Notify(USkeletalMeshComponent* MeshComp,
                      UAnimSequenceBase* Animation) override;
};
