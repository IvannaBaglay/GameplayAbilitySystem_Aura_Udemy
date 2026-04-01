// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Math/RotationMatrix.h"
#include "DrawDebugHelpers.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true;
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    CursorTrace();

    AutoRun();
}

void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay();

    check(AuraContext);

    //UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()); // ?? what is better

    if (Subsystem)
    {
        Subsystem->AddMappingContext(AuraContext, 0);
    }


    // Mouse setup
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);

    SetInputMode(InputModeData);
    //
}

void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

    AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
    AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector Rightirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControllerPawn = GetPawn<APawn>())
    {
        ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControllerPawn->AddMovementInput(Rightirection, InputAxisVector.X);
    }
}

void AAuraPlayerController::CursorTrace()
{
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    if (!CursorHit.bBlockingHit)
        return;

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    /*
    * Line trace from cursor. Several scenarios:
    * A. LastActor is valid, ThisActor is valid
    * - Do nothing
    * B. LastActor is valid, ThisActor is invalid
    * - UnHightLight LastActor
    * C. LastActor is invalid, ThisActor is valid
    * - HightLight ThisActor
    * D. LastActor is valid and ThisActor is valid, but different
    * - LastActor UnHightLight
    * - ThisActor HightLight
    * E. LastActor is valid, ThisActor is valid but the same
    * - Do nothing
    */

    if (LastActor == ThisActor)
    {
        // case A and E
        return;
    }

    if (LastActor != nullptr)
    {
        if (ThisActor != nullptr)
        {
            // case D
            LastActor->UnHightlightActor();
            ThisActor->HightlightActor();
        }
        else
        {
            // case B
            LastActor->UnHightlightActor();
        }
    }
    else
    {
        if (ThisActor != nullptr)
        {
            // case C
            ThisActor->HightlightActor();
        }
    }
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        bTargeting = ThisActor ? true : false;
        bAutoRunning = false;
    }
    //GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Pressed: %s"), *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        if (GetAuraAbilitySystemComponentBase())
        {
            GetAuraAbilitySystemComponentBase()->AbilityInputTagReleased(InputTag);
        }
        return;
    }

    if (bTargeting)
    {
        if (GetAuraAbilitySystemComponentBase())
        {
            GetAuraAbilitySystemComponentBase()->AbilityInputTagReleased(InputTag);
        }
    }
    else
    {
        APawn* ControlledPawn = GetPawn();
        if (FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                SplineComponent->ClearSplinePoints();
                if (!NavPath->PathPoints.IsEmpty())
                {
                    for (const FVector& PointLocation : NavPath->PathPoints)
                    {
                        SplineComponent->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
                        DrawDebugSphere(GetWorld(), PointLocation, 8.f, 8, FColor::Green, false, 5.f);
                    }
                    CachedDestination = NavPath->PathPoints.Last();
                    bAutoRunning = true;
                }
               
            }
        }
        FollowTime = 0.f;
        bTargeting = false;
    }
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

    if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        if (GetAuraAbilitySystemComponentBase())
        {
            GetAuraAbilitySystemComponentBase()->AbilityInputTagHeld(InputTag);
        }
        return;
    }

    if (bTargeting)
    {
        if (GetAuraAbilitySystemComponentBase())
        {
            GetAuraAbilitySystemComponentBase()->AbilityInputTagHeld(InputTag);
        }
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();
        FHitResult CursorHit;
        if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
        {
            CachedDestination = CursorHit.ImpactPoint;
        }

        if (APawn* ControlledPawn = GetPawn())
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection);
        }
    }
}

UAuraAbilitySystemComponentBase* AAuraPlayerController::GetAuraAbilitySystemComponentBase()
{
    if (AuraAbilitySystemComponentBase == nullptr)
    {
        AuraAbilitySystemComponentBase = Cast<UAuraAbilitySystemComponentBase>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
    }
    return AuraAbilitySystemComponentBase;
}

void AAuraPlayerController::AutoRun()
{
    if (!bAutoRunning)
        return;

    APawn* ControlledPawn = GetPawn();

    if (ControlledPawn)
    {
        const FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
        const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
        if (DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bAutoRunning = false;
        }
    }
}
