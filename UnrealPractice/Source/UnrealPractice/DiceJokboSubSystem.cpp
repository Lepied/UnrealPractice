#include "DiceJokboSubsystem.h"

void UDiceJokboSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);

    // 야찌 (5개 동일)
    AllJokbos.Add(FJokboDefinition(
        TEXT("야찌"), 150, 100,
        [this](const TArray<int32> &Dice) -> bool
        {
            int32 Counts[MAX_DICE_VALUE + 1];
            CountDiceFrequency(Dice, Counts);

            // 배열 순회하며 5개 이상인 숫자가 있는지 확인
            for (int32 i = 1; i <= MAX_DICE_VALUE; ++i)
            {
                if (Counts[i] >= 5)
                    return true;
            }
            return false;
        }));

    // 포카드 (4개 동일)
    AllJokbos.Add(FJokboDefinition(
        TEXT("포카드"), 80, 50,
        [this](const TArray<int32> &Dice) -> bool
        {
            int32 Counts[MAX_DICE_VALUE + 1];
            CountDiceFrequency(Dice, Counts);

            for (int32 i = 1; i <= MAX_DICE_VALUE; ++i)
            {
                if (Counts[i] >= 4)
                    return true;
            }
            return false;
        }));

    // 풀 하우스 (3개 + 2개)
    AllJokbos.Add(FJokboDefinition(
        TEXT("풀 하우스"), 70, 40,
        [this](const TArray<int32> &Dice) -> bool
        {
            int32 Counts[MAX_DICE_VALUE + 1];
            CountDiceFrequency(Dice, Counts);

            bool bThree = false;
            bool bTwo = false;

            for (int32 i = 1; i <= MAX_DICE_VALUE; ++i)
            {
                if (Counts[i] == 5)
                    return true; // 야찌도 풀하우스로 쳐주기
                if (Counts[i] == 3)
                    bThree = true;
                else if (Counts[i] == 2)
                    bTwo = true;
            }
            return bThree && bTwo;
        }));

    // 라지 스트레이트 //Sort랑 Set 쓰는거에서 빈도수 배열 순회로 변경해서 최적화
    AllJokbos.Add(FJokboDefinition(
        TEXT("라지 스트레이트"), 70, 40,
        [this](const TArray<int32> &Dice) -> bool
        {
            int32 Counts[MAX_DICE_VALUE + 1];
            CountDiceFrequency(Dice, Counts);

            int32 ConsecutiveCount = 0;
            for (int32 i = 1; i <= MAX_DICE_VALUE; ++i)
            {
                if (Counts[i] > 0) // 눈금이 존재함
                {
                    ConsecutiveCount++;
                    if (ConsecutiveCount >= 5)
                        return true;
                }
                else
                {
                    ConsecutiveCount = 0;
                }
            }
            return false;
        }));

    AllJokbos.Add(FJokboDefinition(
        TEXT("스몰 스트레이트"), 50, 25,
        [this](const TArray<int32> &Dice) -> bool
        {
            int32 Counts[MAX_DICE_VALUE + 1];
            CountDiceFrequency(Dice, Counts);

            int32 ConsecutiveCount = 0;
            for (int32 i = 1; i <= MAX_DICE_VALUE; ++i)
            {
                if (Counts[i] > 0)
                {
                    ConsecutiveCount++;
                    if (ConsecutiveCount >= 4)
                        return true;
                }
                else
                {
                    ConsecutiveCount = 0;
                }
            }
            return false;
        }));

    // 모두 짝수
    AllJokbos.Add(FJokboDefinition(
        TEXT("모두 짝수"), 30, 15,
        [](const TArray<int32> &Dice) -> bool
        {
            for (int32 Val : Dice)
            {
                if (Val % 2 != 0)
                    return false;
            }
            return true;
        }));

    // 모두 홀수
    AllJokbos.Add(FJokboDefinition(
        TEXT("모두 홀수"), 30, 15,
        [](const TArray<int32> &Dice) -> bool
        {
            for (int32 Val : Dice)
            {
                if (Val % 2 == 0)
                    return false;
            }
            return true;
        }));

    // 트리플
    AllJokbos.Add(FJokboDefinition(
        TEXT("트리플"), 40, 20,
        [this](const TArray<int32> &Dice) -> bool
        {
            int32 Counts[MAX_DICE_VALUE + 1];
            CountDiceFrequency(Dice, Counts);

            for (int32 i = 1; i <= MAX_DICE_VALUE; ++i)
            {
                if (Counts[i] >= 3)
                    return true;
            }
            return false;
        }));

    // 투 페어
    AllJokbos.Add(FJokboDefinition(
        TEXT("투 페어"), 25, 10,
        [this](const TArray<int32> &Dice) -> bool
        {
            int32 Counts[MAX_DICE_VALUE + 1];
            CountDiceFrequency(Dice, Counts);

            int32 PairCount = 0;
            for (int32 i = 1; i <= MAX_DICE_VALUE; ++i)
            {
                if (Counts[i] >= 2)
                    PairCount++;
            }
            return PairCount >= 2;
        }));

    // 총합 (가변 데미지)
    AllJokbos.Add(FJokboDefinition(
        TEXT("총합"),
        // 데미지 계산 람다
        [](const TArray<int32> &Dice) -> int32
        {
            int32 Sum = 0;
            for (int32 Val : Dice)
                Sum += Val;
            return Sum;
        },
        // 골드 계산 람다
        [](const TArray<int32> &Dice) -> int32
        {
            int32 Sum = 0;
            for (int32 Val : Dice)
                Sum += Val;
            return Sum;
        },
        // 조건 (항상 참)
        [](const TArray<int32> &Dice) -> bool
        {
            return true;
        }));
}

// 최적화 함수
void UDiceJokboSubsystem::CountDiceFrequency(const TArray<int32> &DiceValues, int32 (&OutCounts)[MAX_DICE_VALUE + 1])
{
    // 메모리 초기화
    FMemory::Memzero(OutCounts, sizeof(int32) * (MAX_DICE_VALUE + 1));

    for (int32 Val : DiceValues)
    {
        if (Val >= 1 && Val <= MAX_DICE_VALUE)
        {
            OutCounts[Val]++;
        }
    }
}


// 달성 가능한 족보 리스트 반환
TArray<FAchievedJokbo> UDiceJokboSubsystem::GetAchievableJokbos(const TArray<int32> &DiceValues)
{
    TArray<FAchievedJokbo> Result;

    // 유물도 구현하려면 여기서 체크해서 넘기기

    for (const FJokboDefinition &Def : AllJokbos)
    {
        // 조건 체크
        if (Def.CheckLogic && Def.CheckLogic(DiceValues))
        {
            FAchievedJokbo Achieved;
            Achieved.Description = Def.Description;

            // 가변/고정 데미지 계산
            if (Def.DamageCalcLogic)
            {
                Achieved.FinalDamage = Def.DamageCalcLogic(DiceValues);
                Achieved.FinalGold = Def.GoldCalcLogic(DiceValues);
            }
            else
            {
                Achieved.FinalDamage = Def.BaseDamage;
                Achieved.FinalGold = Def.BaseGold;
            }

            Result.Add(Achieved);
        }
    }

    // 데미지 높은 순으로 정렬
    Result.Sort([](const FAchievedJokbo &A, const FAchievedJokbo &B)
                { return A.FinalDamage > B.FinalDamage; });

    return Result;
}


//과거 방식이랑 새로 배열로 바꾼거랑 성능 비교해보기
void UDiceJokboSubsystem::RunBenchmark()
{
    //일단 주사위 다섯개로 테스트
    TArray<int32> TestDice = {1, 2, 3, 4, 5}; 
    int32 LoopCount = 1000000; //반복회수

    //힙 할당 발생하는 기존 방식
    double StartTimeOld = FPlatformTime::Seconds();

    for (int32 i = 0; i < LoopCount; ++i)
    {
        
        TMap<int32, int32> OldCounts;
        for (int32 Val : TestDice)
        {
            OldCounts.FindOrAdd(Val)++;
        }
        
        // 컴파일러가 아무것도 안한다고 꺼버리지 못하게 하는 코드
        int32 Dummy = OldCounts.Num(); 
    }

    double TimeOld = (FPlatformTime::Seconds() - StartTimeOld) * 1000.0f;


    //배열이랑 스택메모리로 하는 새 방식
    double StartTimeNew = FPlatformTime::Seconds();

    for (int32 i = 0; i < LoopCount; ++i)
    {
        int32 NewCounts[MAX_DICE_VALUE + 1];
        CountDiceFrequency(TestDice, NewCounts);

        // 더미 코드
        int32 Dummy = NewCounts[1];
    }

    double TimeNew = (FPlatformTime::Seconds() - StartTimeNew) * 1000.0f;


    //결과 출력하게하기
    UE_LOG(LogTemp, Error, TEXT("성능 측정 결과 (%d회 반복)"), LoopCount);
    UE_LOG(LogTemp, Warning, TEXT("1. 기존 방식: %.4f ms"), TimeOld);
    UE_LOG(LogTemp, Warning, TEXT("2. 개선 방식: %.4f ms"), TimeNew);
    
    if (TimeNew > 0.0f)
    {
        float SpeedUp = TimeOld / TimeNew;
        UE_LOG(LogTemp, Error, TEXT(" %.2f배 더 개선됨"), SpeedUp);
    }
}