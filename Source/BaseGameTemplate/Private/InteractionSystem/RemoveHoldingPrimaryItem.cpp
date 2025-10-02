// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/RemoveHoldingPrimaryItem.h"

#include "InteractionSystem/InteractionComponent.h"
#include "InteractionSystem/InteractionSystemHelpers.h"

void URemoveHoldingPrimaryItem::Notify(USkeletalMeshComponent* MeshComp,
                                       UAnimSequenceBase* Animation) {
  UInteractionComponent* interactionComponent =
      InteractionSystemHelpers::CheckAndGetInteractionComponent(MeshComp);
  if (interactionComponent) {
    interactionComponent->RemoveHoldingPrimaryItem();
  } else {
    UE_LOG(
        LogTemp, Warning,
        TEXT(
            "URemoveHoldingPrimaryItem::Notify: InteractionComponent is null"));
  }
}
