// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widget/AuraUserWidget.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
    if (OverlayWidgetController == nullptr)
    {
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerParams(Params);
        OverlayWidgetController->BindCallbacksToDependencies();

        return OverlayWidgetController;
    }
    return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& Params)
{
    if (AttributeMenuWidgetController == nullptr)
    {
        AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
        AttributeMenuWidgetController->SetWidgetControllerParams(Params);
        AttributeMenuWidgetController->BindCallbacksToDependencies();

        return AttributeMenuWidgetController;
    }
    return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* playerController, APlayerState* playerState, UAbilitySystemComponent* abilitySystemComponent, UAttributeSet* attributeSet)
{
    checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is uninitialized, please fill out BP_AuraHUD"));
    checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is uninitialized, please fill out BP_AuraHUD "));

    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget = Cast<UAuraUserWidget>(Widget);

    const FWidgetControllerParams Params(playerController, playerState, abilitySystemComponent, attributeSet);

    UOverlayWidgetController* WidgetController = GetOverlayWidgetController(Params);
    OverlayWidget->SetWidgetController(WidgetController);
    WidgetController->BroadcastInitialValues();

    Widget->AddToViewport();
}
