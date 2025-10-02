// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractableDoor.h"

void AInteractableDoor::Interact_Implementation(AActor* InteractingActor) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
		  -1,  // Key (-1 = always add new, otherwise replaces existing with
			   // same key)
		  5.0f,                    // Time to display (seconds)
		  FColor::Green,           // Text color
		  TEXT("Interacted with the door")  // The message
		);
	}
}
