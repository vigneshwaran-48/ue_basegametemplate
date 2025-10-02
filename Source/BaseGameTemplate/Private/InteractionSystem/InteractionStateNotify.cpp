// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractionStateNotify.h"
#include "InteractionSystem/InteractionComponent.h"
#include "InteractionSystem/InteractionSystemHelpers.h"

void UInteractionStateNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	UInteractionComponent* interactionComponent = InteractionSystemHelpers::CheckAndGetInteractionComponent(MeshComp);
	if (interactionComponent) {
		interactionComponent->SetInteractionState(InteractionState);
	}
}
