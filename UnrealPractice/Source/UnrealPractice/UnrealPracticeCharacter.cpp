// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealPracticeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UnrealPractice.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "DiceJokboSubSystem.h"

AUnrealPracticeCharacter::AUnrealPracticeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AUnrealPracticeCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnrealPracticeCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AUnrealPracticeCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnrealPracticeCharacter::Look);
	}
	else
	{
		UE_LOG(LogUnrealPractice, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &AUnrealPracticeCharacter::ToggleDiceUI);
	}
}

void AUnrealPracticeCharacter::Move(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AUnrealPracticeCharacter::Look(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AUnrealPracticeCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AUnrealPracticeCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AUnrealPracticeCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AUnrealPracticeCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

// ui 토글
void AUnrealPracticeCharacter::ToggleDiceUI()
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("K키 입력 감지됨!"));

    if (DiceWidgetClass == nullptr) 
    {
        if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("위젯 클래스가 없음 (None)"));
        return;
    }
	if (CurrentDiceWidget)
	{
		CurrentDiceWidget->RemoveFromParent();
		CurrentDiceWidget = nullptr;

		return;
	}

	if (DiceWidgetClass)
	{
		// UI 생성
		CurrentDiceWidget = CreateWidget<UUserWidget>(GetWorld(), DiceWidgetClass);
		if (CurrentDiceWidget)
		{
			CurrentDiceWidget->AddToViewport(); // 화면에 부착
		}
	}
}

void AUnrealPracticeCharacter::PerformAttack(const TArray<int32>& DiceResults)
{
    //서브시스템 가져오기
    UGameInstance* GI = GetGameInstance();
    UDiceJokboSubsystem* JokboSystem = GI ? GI->GetSubsystem<UDiceJokboSubsystem>() : nullptr;

    if (JokboSystem)
    {
        // 가능한 족보 긁어오기
        TArray<FAchievedJokbo> Options = JokboSystem->GetAchievableJokbos(DiceResults);

        // 바로 공격하지 말고 UI표시
        ShowJokboSelection(Options);
    }
}

void AUnrealPracticeCharacter::ProcessFinalAttack(FAchievedJokbo SelectedJokbo)
{

	FString LogMsg = FString::Printf(TEXT("공격! [%s] 데미지: %d"), *SelectedJokbo.Description, SelectedJokbo.FinalDamage);
    if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, LogMsg);
	
	// 실제 데미지 처리
	OnExecuteAttack(SelectedJokbo.FinalDamage, SelectedJokbo.Description);

	// 기존 UI 끄기
	ToggleDiceUI();
	// TODO: 공격 로직이나 애니메이션
}
