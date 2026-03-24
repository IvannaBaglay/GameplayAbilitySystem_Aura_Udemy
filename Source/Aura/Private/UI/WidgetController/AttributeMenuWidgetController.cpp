// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

    check(AttributeInformation);

    for (auto& Pair : AS->TagsToAttributes)
    {
        FAuraAttributeInfo Info =  AttributeInformation->FindAttributeInfoForTag(Pair.Key);
        FGameplayAttribute Attribute = Pair.Value();
        Info.AttributeValue = Attribute.GetNumericValue(AS);
        AttributeInfoDelegate.Broadcast(Info);
    }
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
