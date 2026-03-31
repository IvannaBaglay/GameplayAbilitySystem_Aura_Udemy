// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Math/RotationMatrix.h"
#include "DrawDebugHelpers.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    CursorTrace();
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
    GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Pressed: %s"), *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("Released: %s"), *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Blue, FString::Printf(TEXT("Held: %s"), *InputTag.ToString()));
}
