// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DiceJokboSubsystem.h"
#include "UnrealPracticeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AUnrealPracticeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	AUnrealPracticeCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
    //UI 위젯을 저장해주는 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dice UI")
    TSubclassOf<class UUserWidget> DiceWidgetClass;

    //현재 화면에 떠 있는 UI 위젯 변수
    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Dice UI")
    class UUserWidget* CurrentDiceWidget;

    //K키를 눌렀을 때 실행할 액션
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* RollAction;

    //주사위 결과보고 공격시작하기
    UFUNCTION(BlueprintCallable, Category = "Dice Combat")
    void PerformAttack(const TArray<int32>& DiceResults);

	// [신규] 유저가 리스트에서 버튼을 눌렀을 때 호출할 "진짜 공격" 함수
    UFUNCTION(BlueprintCallable, Category = "Dice Combat")
    void ProcessFinalAttack(FAchievedJokbo SelectedJokbo);

protected:
	//UI 토글
    void ToggleDiceUI();

protected:
    //족보 선택 UI 표시
    UFUNCTION(BlueprintImplementableEvent, Category = "Dice Combat")
    void ShowJokboSelection(const TArray<FAchievedJokbo>& Options);

};

