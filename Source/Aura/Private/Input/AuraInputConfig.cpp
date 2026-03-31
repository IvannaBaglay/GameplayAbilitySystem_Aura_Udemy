// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"
#include "EnhancedInput/Public/InputAction.h"

const UInputAction* UAuraInputConfig::GetInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    const FAuraInputAction* Action = AbilityInputActions.FindByPredicate([&InputTag](const FAuraInputAction& action) {
        return action.InputTag == InputTag;
        });

    if (bLogNotFound && !Action)
    {
        UE_LOG(LogTemp, Error, TEXT("can't find AbilityInputAction for InputTag [%s], on InputConfig [%s] "), *InputTag.ToString(), *GetNameSafe(this));
    }

    return Action ? Action->InputAction : nullptr;
}
