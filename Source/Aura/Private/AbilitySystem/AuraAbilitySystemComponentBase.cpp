// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponentBase.h"

void UAuraAbilitySystemComponentBase::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponentBase::EffectApplied);
}

void UAuraAbilitySystemComponentBase::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);
    for (const FGameplayTag& Tag: TagContainer)
    {
        // TODO: Broadcast tags to the Widget Controller
        const FString TagString = FString::Printf(TEXT("Tag Applied: %s"), *Tag.ToString()); // Or Tag.GetTagName()
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TagString);
    }

}
