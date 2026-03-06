// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;


	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr)
	{
		return;
	}

    check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
    const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnOverlapEnd)
	{
        ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
        ApplyEffectToTarget(TargetActor, InstantGameplayEffect);
	}
	if (DurationEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
        ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
    }
}

void AAuraEffectActor::OnOverlapEnd(AActor* TargetActor)
{
	if (InstantEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnOverlapEnd)
	{
        ApplyEffectToTarget(TargetActor, InstantGameplayEffect);
	}
	if (DurationEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnOverlapEnd)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
    }
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnOverlapEnd)
	{
        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC))
		{
            return;
		}

        TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		for (const auto& HandlePair : ActiveEffectHandles)
		{
			if (HandlePair.Value == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
            }
		}

		for (const auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.Remove(Handle);
        }
	}
}
