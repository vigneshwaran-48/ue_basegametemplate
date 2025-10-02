// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/RemovePickableItem.h"
#include "InteractionSystem/InteractionComponent.h"
#include "InteractionSystem/InteractionSystemHelpers.h"

void URemovePickableItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	UInteractionComponent* interactionComponent = InteractionSystemHelpers::CheckAndGetInteractionComponent(MeshComp);
	if (interactionComponent) {
		interactionComponent->RemovePickableItem();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("URemovePickableItem::Notify: InteractionComponent is null"));
	}
}
