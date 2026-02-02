// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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
