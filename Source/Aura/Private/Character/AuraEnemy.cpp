// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

AAuraEnemy::AAuraEnemy()
{
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");

}

void AAuraEnemy::BeginPlay()
{
    Super::BeginPlay();
    if (AbilitySystemComponent != nullptr)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
    else
    {
        UE_LOG(LogMyGame, Error, TEXT("[AAuraEnemy]: InitAbilityActorInfo failed"));
    }
}

void AAuraEnemy::HightlightActor()
{
    bHightlighted = true;
    GetMesh()->SetRenderCustomDepth(bHightlighted);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

    Weapon->SetRenderCustomDepth(bHightlighted);
    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHightlightActor()
{
    bHightlighted = false;
    GetMesh()->SetRenderCustomDepth(bHightlighted);
    //GetMesh()->SetCustomDepthStencilValue(0);

    Weapon->SetRenderCustomDepth(bHightlighted);
    //Weapon->SetCustomDepthStencilValue(0);
}
