// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "InteractionState.h"
#include "InteractionStateNotify.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMETEMPLATE_API UInteractionStateNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
         EInteractionState InteractionState = EInteractionState::None;

        virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
