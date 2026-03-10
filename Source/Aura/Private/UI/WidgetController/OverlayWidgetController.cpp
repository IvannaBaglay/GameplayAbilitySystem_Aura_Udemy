// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
    const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

    OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
        .AddUObject(this, &UOverlayWidgetController::HealthChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
        .AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
        .AddUObject(this, &UOverlayWidgetController::ManaChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
        .AddUObject(this, &UOverlayWidgetController::MaxManaChanged); // what is AddObjectUI


    Cast<UAuraAbilitySystemComponentBase>(AbilitySystemComponent)->OnEffectAssetTagChanged.AddLambda([this](const FGameplayTagContainer& AssetTags)
        {
            for (const FGameplayTag& Tag : AssetTags)
            {
                // Example, Tag = Message.HealthPotion
                // 


                //"Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
                const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
                if (Tag.MatchesTag(MessageTag))
                {
                    FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
                    MessageWidgetRowDelegate.Broadcast(*Row);
                }

                //const FString TagString = FString::Printf(TEXT("Tag Applied: %s"), *Tag.ToString()); // Or Tag.GetTagName()
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TagString);
            }
        }
    );
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
    OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
    OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
    OnManaChanged.Broadcast(Data.NewValue);

}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
    OnMaxManaChanged.Broadcast(Data.NewValue);
}

//void UOverlayWidgetController::MessageDelegate(const FUIWidgetRow& Data) const
//{
//    const FString TagString = FString::Printf(TEXT("Tag Applied: %s"), Data.Message);
//    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TagString);
//}
