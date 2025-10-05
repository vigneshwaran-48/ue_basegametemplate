// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/InteractionWidget.h"

#include "Components/Image.h"

void UInteractionWidget::NativeConstruct() {
  Super::NativeConstruct();

  if (IconImage && Icon) {
    IconImage->SetBrushFromTexture(Icon);
  }
}

void UInteractionWidget::SetIcon(UTexture2D* NewIcon) {
  if (IconImage && NewIcon) {
    IconImage->SetBrushFromTexture(NewIcon);
  }
}
