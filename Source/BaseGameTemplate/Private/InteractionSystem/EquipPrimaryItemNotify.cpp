// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/EquipPrimaryItemNotify.h"
#include "InteractionSystem/InteractionComponent.h"
#include "InteractionSystem/InteractionSystemHelpers.h"

void UEquipPrimaryItemNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	UInteractionComponent* interactionComponent = InteractionSystemHelpers::CheckAndGetInteractionComponent(MeshComp);
	if (interactionComponent) {
		interactionComponent->EquipPrimaryItemNotify();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("UEquipPrimaryItemNotify::Notify: InteractionComponent is null"));
	}
}
