// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
    AAuraEnemy();
    virtual void BeginPlay() override;
public:

    /* Enemy Interface*/
    virtual void HightlightActor() override;
    virtual void UnHightlightActor() override;

    /*Combat Interface*/
    virtual int32 GetPlayerLevel() override;

    UPROPERTY(BlueprintReadOnly)
    bool bHightlighted = false;
    protected:
    virtual void InitAbilityActorInfo() override;

    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Character Class Defaults")
    int32 Level = 1;
private:
};
