// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnOverlapEnd,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnOverlapEnd,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "AppliedEffects")
    void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable, Category = "AppliedEffects")
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Category = "AppliedEffects")
    void OnOverlapEnd(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	bool bdestroyOnEffectRemoval = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects|Instant")
	EEffectApplicationPolicy InstantEffectApplycationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects|Instant")
    TSubclassOf<UGameplayEffect> InstantGameplayEffect;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects|Duration")
	EEffectApplicationPolicy DurationEffectApplycationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects|Duration")
	TSubclassOf<UGameplayEffect>  DurationGameplayEffectClass;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects|Infinite")
	EEffectApplicationPolicy InfiniteEffectApplycationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects|Infinite")
    EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnOverlapEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects|Infinite")
	TSubclassOf<UGameplayEffect>  InfiniteGameplayEffectClass;
	
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
    float ActorLevel = 1.f;

private:
};
