// Copyright William Marietta


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

	

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));



}

void AAuraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CursorTrace();

	if(bIsAutoRunning) 
	{
		AutoRun();
		
	}

	

}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextClass) 
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);

	}
}


void AAuraPlayerController::AutoRun()
{

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector DirectionOnSpline = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(DirectionOnSpline, 1.0f);

		const float DistanceToDestination = FVector::Distance(LocationOnSpline, CachedDestination);
		if (DistanceToDestination < AutoRunAcceptanceRadius)
		{
			bIsAutoRunning = false;
		}
	}
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{ 
		Subsystem->AddMappingContext(AuraContext, 0); 
	}

	

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	

}


void AAuraPlayerController::SetupInputComponent() 
{
	Super::SetupInputComponent();


	//UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	UAuraInputComponent* AuraInputComponent = Cast<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

}

void AAuraPlayerController::ShiftPressed()
{
	bIsShiftPressed = true;
}

void AAuraPlayerController::ShiftReleased()
{
	bIsShiftPressed = false;
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	


}

void AAuraPlayerController::CursorTrace()
{
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);

	if (!CursorHitResult.bBlockingHit) 
	{
		return;
	}


	LastActor = CurrentActor;
	CurrentActor = CursorHitResult.GetActor();
	

	if (LastActor == nullptr && CurrentActor != nullptr) 
	{
		CurrentActor->HighlightActor();
	}

	else if (LastActor != nullptr && CurrentActor == nullptr) 
	{
		LastActor->UnHighlightActor();
	}

	else if (LastActor != nullptr && CurrentActor != nullptr && LastActor != CurrentActor) 
	{
		LastActor->UnHighlightActor();
		CurrentActor->HighlightActor();
	}

}



void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB)) 
	{
		bIsTargeting = (CurrentActor) ? true : false;
		bIsAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetAuraAbilitySystemComponent() == nullptr)
	{
		return;
	}

	// navigation path following
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB) && !bIsTargeting && !bIsShiftPressed)
	{
		
		APawn* ControlledPawn = GetPawn<APawn>();
		if (FollowTime <= ShortPressedThreshold && ControlledPawn) 
		{
			
			
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), ControlledPawn->GetActorLocation(), CachedDestination)) 
			{
				Spline->ClearSplinePoints();
				for(FVector& Point : NavPath->PathPoints) 
				{
					Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
					
				}

				if (NavPath) 
				{
					if(NavPath->PathPoints.Num() > 0)
					{
						CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					}
					
				}

			}

			bIsAutoRunning = true;

		}

		FollowTime = 0.0f;
		bIsTargeting = false;


		return;
	}



	GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

	if(GetAuraAbilitySystemComponent() == nullptr)
	{
		return;
	}

	//should the character move towards the cursor location
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB) && !bIsTargeting && !bIsShiftPressed)
	{
		
		FollowTime += GetWorld()->GetDeltaSeconds();

		if(CursorHitResult.bBlockingHit)
		{
			CachedDestination = CursorHitResult.Location;
		}

		if (APawn* ControlledPawn = GetPawn()) 
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection, 1.0f);
		}

		return;
	}

	GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);

}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}

	return AuraAbilitySystemComponent;
}

