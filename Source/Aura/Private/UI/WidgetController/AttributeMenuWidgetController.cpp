// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
    check(AttributeInformation);

    for (auto& [Tag, Attribute] : AS->TagsToAttributes)
    {
        BroadcastAttributeInfo(Tag, Attribute());
    }
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
    check(AttributeInformation);

    for (auto& [Tag, Attribute] : AS->TagsToAttributes)
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute()).AddLambda
        (
            [this, Tag, Attribute](const FOnAttributeChangeData& Data)
            {
                BroadcastAttributeInfo(Tag, Attribute());
            }

        );
    }
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const 
{
    FAuraAttributeInfo Info = AttributeInformation->FindAttributeInfoForTag(AttributeTag);
    Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
    AttributeInfoDelegate.Broadcast(Info);
}
