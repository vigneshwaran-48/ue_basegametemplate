// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.h"

class BASEGAMETEMPLATE_API InteractionSystemHelpers {

	public:

		static UInteractionComponent* CheckAndGetInteractionComponent(USkeletalMeshComponent* MeshComp){
			if (!MeshComp) {
				UE_LOG(LogTemp, Warning, TEXT("InteractionSystemHelpers::CheckAndGetInteractionComponent: MeshComp is null"));
				return nullptr;
			}
			AActor* owner = MeshComp->GetOwner();
			if (!owner) {
				UE_LOG(LogTemp, Warning, TEXT("InteractionSystemHelpers::CheckAndGetInteractionComponent: Owner is null"));
				return nullptr;
			}
			UInteractionComponent* interactionComponent = owner->FindComponentByClass<UInteractionComponent>();
			if (!interactionComponent) {
				UE_LOG(LogTemp, Warning, TEXT("InteractionSystemHelpers::CheckAndGetInteractionComponent: InteractionComponent is null"));
				return nullptr;
			}
			return interactionComponent;
		}

		static void LogAndDisplayDebugMessage(const FString& Message,
                                                      FColor DisplayColor,
                                                      float Duration) {
                  UE_LOG(LogTemp, Warning, TEXT("DEBUG: %s"), *Message);

                  if (GEngine) {
                    GEngine->AddOnScreenDebugMessage(
                        -1,            // Key
                        Duration,      // Time to display
                        DisplayColor,  // Text color
                        Message  // The message (FString is acceptable here)
                    );
                  }
		}
};
