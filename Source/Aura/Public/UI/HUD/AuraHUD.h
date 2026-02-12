// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
class UOverlayWidgetController;

class APlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;

struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
    TObjectPtr<UAuraUserWidget> OverlayWidget;

    UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params );

    void InitOverlay(APlayerController* playerController, APlayerState* playerState, UAbilitySystemComponent* abilitySystemComponent, UAttributeSet* attributeSet);
protected:

private:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
