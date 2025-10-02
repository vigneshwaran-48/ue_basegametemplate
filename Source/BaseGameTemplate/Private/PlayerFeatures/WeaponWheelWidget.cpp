
#include "PlayerFeatures/WeaponWheelWidget.h"

#include "Components/Image.h"
#include "InventorySystem/InventoryItem.h"

void UWeaponWheelWidget::NativeConstruct() { Super::NativeConstruct(); }

void UWeaponWheelWidget::LoadWeaponItems(const TArray<FInventoryItem>& Items) {
  UE_LOG(LogTemp, Log, TEXT("Load weapon items start"));
  for (int i = 0; i < Items.Num(); i++) {
    FInventoryItem Item = Items[i];
    SlotIcons[i]->SetBrushFromTexture(Item.Icon);
    SlotIcons[i]->SetRenderOpacity(1);
  }
}

void UWeaponWheelWidget::SelectSlot(int32 NewIndex) {
  if (CurrentSelectedSlot) {
    CurrentSelectedSlot->SetRenderOpacity(0.3f);
  }

  CurrentSelectedSlot = Slots[NewIndex];
  CurrentSelectedSlot->SetRenderOpacity(0.7f);
}

void UWeaponWheelWidget::OnWheelClosed() {
  if (CurrentSelectedSlot) {
    CurrentSelectedSlot->SetRenderOpacity(0.3f);
    CurrentSelectedSlot = nullptr;
  }
}