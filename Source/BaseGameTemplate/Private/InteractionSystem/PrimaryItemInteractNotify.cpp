// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/PrimaryItemInteractNotify.h"

#include "InteractionSystem/InteractionComponent.h"
#include "InteractionSystem/InteractionSystemHelpers.h"

void UPrimaryItemInteractNotify::Notify(USkeletalMeshComponent* MeshComp,
                                        UAnimSequenceBase* Animation) {
  UInteractionComponent* interactionComponent =
      InteractionSystemHelpers::CheckAndGetInteractionComponent(MeshComp);
  if (interactionComponent) {
    interactionComponent->PrimaryItemInteractNotify();
  } else {
    UE_LOG(LogTemp, Warning,
           TEXT("UInteractNotify::Notify: InteractionComponent is null"));
  }
}
