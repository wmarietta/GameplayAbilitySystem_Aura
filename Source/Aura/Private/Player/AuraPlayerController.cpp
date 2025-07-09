// Copyright William Marietta


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

}

void AAuraPlayerController::Tick(float DeltaTime)
{
	CursorTrace();
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


	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);



	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);


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
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);

	if (!CursorHitResult.bBlockingHit) 
	{
		return;
	}


	LastActor = CurrentActor;
	CurrentActor = CursorHitResult.GetActor();
	
	
	/**
	*	Line cursor trace has multiple scenarios:
	*	A. LastActor is null && CurrentActor is null
	*		- Do nothing
	*	
	*	B. LastActor is null && CurrentActor is valid
	*		- Highlight CurrentActor
	*	
	*	C. LastActor is valid && CurrentActor is null
	*		- Unhighlight LastActor
	* 
	*	D. LastActor is valid && CurrentActor is valid and LastActor != CurrentActor 
	*		- Unhighlight LastActor
	*	    - Highlight CurrentActor
	* 
	*	E. LastActor is valid && CurrentActor is valid and LastActor == CurrentActor
	*		- Do nothing
	*/


	if (LastActor == nullptr && CurrentActor != nullptr) 
	{
		//Case B
		CurrentActor->HighlightActor();
	}

	else if (LastActor != nullptr && CurrentActor == nullptr) 
	{
		//Case C
		LastActor->UnHighlightActor();
	}

	else if (LastActor != nullptr && CurrentActor != nullptr && LastActor != CurrentActor) 
	{
		//Case D
		LastActor->UnHighlightActor();
		CurrentActor->HighlightActor();
	}

}
