// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BASEGAMETEMPLATE_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

  public:

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetIcon(UTexture2D* NewIcon);

  protected:

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UTexture2D* Icon;

	virtual void NativeConstruct() override;
};
