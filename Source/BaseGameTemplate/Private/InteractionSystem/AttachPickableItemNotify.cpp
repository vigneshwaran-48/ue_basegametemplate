// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/AttachPickableItemNotify.h"

#include "InteractionSystem/InteractionComponent.h"
#include "InteractionSystem/InteractionSystemHelpers.h"

void UAttachPickableItemNotify::Notify(USkeletalMeshComponent* MeshComp,
                                       UAnimSequenceBase* Animation) {
  UInteractionComponent* interactionComponent =
      InteractionSystemHelpers::CheckAndGetInteractionComponent(MeshComp);
  if (interactionComponent) {
    interactionComponent->AttachPickableItem();
  } else {
    UE_LOG(
        LogTemp, Warning,
        TEXT(
            "UAttachPickableItemNotify::Notify: InteractionComponent is null"));
  }
}
