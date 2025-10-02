// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionData.h"
#include "InventorySystem/InventoryItem.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class BASEGAMETEMPLATE_API IInteractableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* InteractingActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FText GetInteractionText() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    EInteractionType GetInteractionType() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Highlight(bool highlight);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    class UStaticMesh* GetMesh() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    EInteractionInputType GetInteractionInputType() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AActor* InteractingActor) const; // TODO: Need to change the definition for getting the interactor interface as input

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FInteractionData GetInteractionData() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	class UTexture2D* GetIcon() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FName GetItemId() const;
};
