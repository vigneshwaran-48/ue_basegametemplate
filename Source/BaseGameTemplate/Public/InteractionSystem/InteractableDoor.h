// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystem/BaseInteractable.h"
#include "InteractableDoor.generated.h"

/**
 * 
 */
UCLASS()
class BASEGAMETEMPLATE_API AInteractableDoor : public ABaseInteractable
{
	GENERATED_BODY()
	

  protected:

	virtual void Interact_Implementation(AActor* InteractingActor) override;
};
