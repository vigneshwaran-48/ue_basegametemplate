// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractionMontageEndNotify.h"
#include "InteractionSystem/InteractionComponent.h"
#include "InteractionSystem/InteractionSystemHelpers.h"

void UInteractionMontageEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	UInteractionComponent* interactionComponent = InteractionSystemHelpers::CheckAndGetInteractionComponent(MeshComp);
	if (interactionComponent) {
	  interactionComponent->OnInteractionMontageEnded();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("UInteractionMontageEndNotify::Notify: InteractionComponent is null"));
	}
}
