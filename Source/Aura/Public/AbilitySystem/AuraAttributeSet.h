// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
    FGameplayAttributeData Health;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAuraAttributeSet, Health);
    GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);
    GAMEPLAYATTRIBUTE_VALUE_SETTER(Health);
    GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAuraAttributeSet, MaxHealth);
    GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHealth);
    GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxHealth);
    GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attribute")
    FGameplayAttributeData Mana;
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAuraAttributeSet, Mana);
    GAMEPLAYATTRIBUTE_VALUE_GETTER(Mana);
    GAMEPLAYATTRIBUTE_VALUE_SETTER(Mana);
    GAMEPLAYATTRIBUTE_VALUE_INITTER(Mana);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxMana,  Category = "Vital Attribute")
    FGameplayAttributeData MaxMana;
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAuraAttributeSet, MaxMana);
    GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxMana);
    GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxMana);
    GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxMana);

	UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
};
