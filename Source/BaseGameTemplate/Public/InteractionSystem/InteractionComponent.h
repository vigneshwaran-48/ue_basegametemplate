// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "InteractionState.h"
#include "InventorySystem/InventoryItem.h"
#include "InteractionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnItemPutbackFinished);

UCLASS(ClassGroup = (Custom), Blueprintable,
       meta = (BlueprintSpawnableComponent))
class BASEGAMETEMPLATE_API UInteractionComponent : public UActorComponent {
  GENERATED_BODY()
 public:
  // Sets default values for this component's properties
  UInteractionComponent();

  void SetInteractionState(EInteractionState NewState);

  void OnInteractionMontageEnded();

  void AttachPickableItem();

  void RemovePickableItem();

  void InteractNotify();

  void PrimaryItemInteractNotify();

  void RemoveHoldingPrimaryItem();

  UFUNCTION()
  void PutBackHoldingPrimaryItem();

  UFUNCTION()
  void EquipPrimaryItem(FInventoryItem item);

  void EquipPrimaryItemNotify();

  FOnItemPutbackFinished OnItemPutbackFinished;

 protected:
  // Called when the game starts
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
            Category = "Interaction System")
  EInteractionState InteractionState = EInteractionState::None;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
            Category = "Interaction System")
  float InteractionDistance = 250;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
            Category = "Interaction System")
  float InteractionRadius = 30;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
            Category = "Interaction System")
  TEnumAsByte<ETraceTypeQuery> TraceChannel =
      ETraceTypeQuery::TraceTypeQuery1;  // Visibility

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
            Category = "Interaction System")
  TSubclassOf<class UInteractionWidget> InteractionWidgetClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
            Category = "Interaction System")
  class UDataTable* ItemDataTable;

  UFUNCTION()
  AActor* FindBestInteractable();

 private:
  AActor* CurrentBestInteractable;

  TArray<AActor*> NearbyInteractables;

  AActor* LeftInteractingInteractable;

  AActor* RightInteractingInteractable;

  EInteractionType InteractingInteractionType;

  EPlayerHandState RightHandState = EPlayerHandState::Free;

  EPlayerHandState LeftHandState = EPlayerHandState::Free;

  FDelegateHandle PendingPutbackHandle;

  FName HoldingPrimaryItemId;

  FInventoryItem EquippingItem;

  UFUNCTION()
  void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp,
                         AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex, bool bFromSweep,
                         const FHitResult& SweepResult);

  UFUNCTION()
  void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

  UFUNCTION()
  void AddInteractionWidgetToInteractable(AActor* Interactable);

  UFUNCTION()
  void RemoveInteractionWidgetFromInteractable(AActor* Interactable);

  UFUNCTION()
  void HandleInventoryItemPickup(AActor* Interactable);

  UFUNCTION()
  FName GetHandSocketName(EInteractionType InteractionType) const;

  UFUNCTION()
  void HandlePrimaryItemPickup(AActor* Interactable);

  UFUNCTION()
  void PickupPrimaryItem(AActor* Interactable);

  UFUNCTION()
  void HandleEquipPrimaryItem(FInventoryItem item);

  FInventoryItem* GetItemById(FName Id);

 public:
  // Called every frame
  virtual void TickComponent(
      float DeltaTime, ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;

  UFUNCTION()
  void TryInteract();

  UFUNCTION()
  bool IsPlayAvailableForInteraction() const;
};
