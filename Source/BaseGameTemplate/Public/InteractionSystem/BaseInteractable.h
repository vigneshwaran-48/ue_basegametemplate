// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractionData.h"
#include "BaseInteractable.generated.h"

UCLASS()
class BASEGAMETEMPLATE_API ABaseInteractable : public AActor, public IInteractableInterface {
	GENERATED_BODY()
	
public:	
	ABaseInteractable();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction System")
	USceneComponent* SceneRoot;

	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction System")
	 UStaticMeshComponent* StaticMesh;

	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction System")
	 class UBoxComponent* InteractionBox;

	 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System")
	 FInteractionData InteractionData;

	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction System")
	 class UWidgetComponent* InteractionWidget;

	 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System")
	 class UTexture2D* InteractionIcon;

	 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System")
	 EInteractionType InteractionType;

	 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory System")
	 FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory System")
	class UDataTable* ItemDataTable;

	virtual void Interact_Implementation(AActor* InteractingActor) override;

    virtual FText GetInteractionText_Implementation() const override;

    virtual EInteractionType GetInteractionType_Implementation() const override;

    virtual void Highlight_Implementation(bool highlight) override;

    virtual class UStaticMesh* GetMesh_Implementation() const override;

    virtual EInteractionInputType GetInteractionInputType_Implementation() const override;

	virtual bool CanInteract_Implementation(AActor* InteractingActor) const override;

	virtual FInteractionData GetInteractionData_Implementation() const override;

	virtual class UTexture2D* GetIcon_Implementation() const override;

	virtual FName GetItemId_Implementation() const override;

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
