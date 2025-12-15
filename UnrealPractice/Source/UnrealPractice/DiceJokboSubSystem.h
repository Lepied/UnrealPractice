#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DiceJokboSubsystem.generated.h"

// 달성된 족보 정보 UI용
USTRUCT(BlueprintType)
struct FAchievedJokbo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Jokbo")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Jokbo")
    int32 FinalDamage;

    UPROPERTY(BlueprintReadOnly, Category = "Jokbo")
    int32 FinalGold;
};

//족보 판별 로직 정의 람다함수 저장용으로 쓰기
struct FJokboDefinition
{
    FString Description;
    int32 BaseDamage;
    int32 BaseGold;

    // 조건 검사 함수 (리스트 -> bool)
    TFunction<bool(const TArray<int32>&)> CheckLogic;

    //가변 데미지 계산 함수
    TFunction<int32(const TArray<int32>&)> DamageCalcLogic;
    TFunction<int32(const TArray<int32>&)> GoldCalcLogic;

    // 생성자
    FJokboDefinition(FString Desc, int32 Dmg, int32 Gold, TFunction<bool(const TArray<int32>&)> Check)
        : Description(Desc), BaseDamage(Dmg), BaseGold(Gold), CheckLogic(Check), DamageCalcLogic(nullptr), GoldCalcLogic(nullptr) {}

    FJokboDefinition(FString Desc, TFunction<int32(const TArray<int32>&)> DmgCalc, TFunction<int32(const TArray<int32>&)> GoldCalc, TFunction<bool(const TArray<int32>&)> Check)
        : Description(Desc), BaseDamage(0), BaseGold(0), CheckLogic(Check), DamageCalcLogic(DmgCalc), GoldCalcLogic(GoldCalc) {}
};

//했던거에서 족보DB역할. 싱글톤역할해줌
UCLASS()
class UNREALPRACTICE_API UDiceJokboSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // 주어진 주사위 값들로 달성 가능한 족보 리스트 반환
    UFUNCTION(BlueprintCallable, Category = "Jokbo")
    TArray<FAchievedJokbo> GetAchievableJokbos(const TArray<int32>& DiceValues);

private:
    // 족보 정의 리스트
    TArray<FJokboDefinition> AllJokbos;

    // 내부 헬퍼 함수 숫자 빈도수세기
    TMap<int32, int32> GetDiceCounts(const TArray<int32>& DiceValues);
};