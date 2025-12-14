#include "DiceJokboSubsystem.h"

void UDiceJokboSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    //야찌 (5개 동일)
    AllJokbos.Add(FJokboDefinition(
        TEXT("야찌 (5개)"), 150, 100,
        [this](const TArray<int32>& Dice) -> bool {
            TMap<int32, int32> Counts = GetDiceCounts(Dice);
            for (auto& Elem : Counts) {
                if (Elem.Value >= 5) return true;
            }
            return false;
        }
    ));

    //포카드 (4개 동일)
    AllJokbos.Add(FJokboDefinition(
        TEXT("포카드 (4개)"), 80, 50,
        [this](const TArray<int32>& Dice) -> bool {
            TMap<int32, int32> Counts = GetDiceCounts(Dice);
            for (auto& Elem : Counts) {
                if (Elem.Value >= 4) return true;
            }
            return false;
        }
    ));

    //풀 하우스 (3개 + 2개)
    AllJokbos.Add(FJokboDefinition(
        TEXT("풀 하우스"), 70, 40,
        [this](const TArray<int32>& Dice) -> bool {
            TMap<int32, int32> Counts = GetDiceCounts(Dice);
            bool bThree = false;
            bool bTwo = false;
            for (auto& Elem : Counts) {
                if (Elem.Value == 3) bThree = true;
                else if (Elem.Value == 2) bTwo = true;
                // 야찌(5개)인 경우도 풀하우스로 쳐줄지 로직에 따라 결정하기, 지금은 정확히 3,2만해주기
                if(Elem.Value == 5) return true; 
            }
            return bThree && bTwo;
        }
    ));

    //스트레이트
    AllJokbos.Add(FJokboDefinition(
        TEXT("라지 스트레이트 (5연속)"), 70, 40,
        [](const TArray<int32>& Dice) -> bool {
            // 중복 제거 및 정렬
            TArray<int32> Sorted = Dice;
            Sorted.Sort();
            // SEt이용해 중복제거
            TSet<int32> UniqueSet(Sorted);
            if (UniqueSet.Num() < 5) return false;

            TArray<int32> UniqueArr = UniqueSet.Array();
            UniqueArr.Sort();

            // 1,2,3,4,5 또는 2,3,4,5,6 확인
            bool bSeq1 = true; // 1~5
            bool bSeq2 = true; // 2~6
            
            //패턴 매칭
            // 연속성 체크 루프를쓰는게 좋을까
            int32 ConsecutiveCount = 0;
            for(int32 i = 0; i < UniqueArr.Num() - 1; ++i)
            {
                if(UniqueArr[i] + 1 == UniqueArr[i+1]) ConsecutiveCount++;
                else ConsecutiveCount = 0; // 끊김

                if(ConsecutiveCount >= 4) return true; // 5개 연속이란 뜻
            }
            return false;
        }
    ));

    AllJokbos.Add(FJokboDefinition(
        TEXT("스몰 스트레이트 (4연속)"), 50, 25,
        [](const TArray<int32>& Dice) -> bool {
            TArray<int32> Sorted = Dice;
            Sorted.Sort();
            // 중복 제거 (TSet 이용)
            TSet<int32> UniqueSet(Sorted);
            TArray<int32> UniqueArr = UniqueSet.Array();
            UniqueArr.Sort();

            if (UniqueArr.Num() < 4) return false;

            // 연속성 체크
            int32 Consec = 0;
            for(int32 i = 0; i < UniqueArr.Num() - 1; ++i) {
                if(UniqueArr[i] + 1 == UniqueArr[i+1]) {
                    Consec++;
                    if(Consec >= 3) return true; // 간격이 3개면 숫자 4개가 연속임
                } else {
                    Consec = 0;
                }
            }
            return false;
        }
    ));
        //모두 짝수
    AllJokbos.Add(FJokboDefinition(
        TEXT("모두 짝수"), 30, 15,
        [](const TArray<int32>& Dice) -> bool {
            for (int32 Val : Dice) {
                if (Val % 2 != 0) return false; // 홀수 발견되면 안됨
            }
            return true;
        }
    ));

    // 모두 홀수
    AllJokbos.Add(FJokboDefinition(
        TEXT("모두 홀수"), 30, 15,
        [](const TArray<int32>& Dice) -> bool {
            for (int32 Val : Dice) {
                if (Val % 2 == 0) return false; // 짝수 발견되면안됨
            }
            return true;
        }
    ));

    //트리플 
    AllJokbos.Add(FJokboDefinition(
        TEXT("트리플 (3개)"), 40, 20,
        [this](const TArray<int32>& Dice) -> bool {
            TMap<int32, int32> Counts = GetDiceCounts(Dice);
            for (auto& Elem : Counts) {
                if (Elem.Value >= 3) return true;
            }
            return false;
        }
    ));

    //투 페어
    AllJokbos.Add(FJokboDefinition(
        TEXT("투 페어"), 25, 10,
        [this](const TArray<int32>& Dice) -> bool {
            TMap<int32, int32> Counts = GetDiceCounts(Dice);
            int32 PairCount = 0;
            for (auto& Elem : Counts) {
                if (Elem.Value >= 2) PairCount++;
            }
            return PairCount >= 2;
        }
    ));

    // 총합 (가변 데미지)
    AllJokbos.Add(FJokboDefinition(
        TEXT("총합"),
        // 데미지 계산 람다
        [](const TArray<int32>& Dice) -> int32 {
            int32 Sum = 0;
            for (int32 Val : Dice) Sum += Val;
            return Sum;
        },
        // 골드 계산 람다
        [](const TArray<int32>& Dice) -> int32 {
            int32 Sum = 0;
            for (int32 Val : Dice) Sum += Val;
            return Sum;
        },
        // 조건 (항상 참)
        [](const TArray<int32>& Dice) -> bool {
            return true;
        }
    ));
}

//숫자별 개수 세기
TMap<int32, int32> UDiceJokboSubsystem::GetDiceCounts(const TArray<int32>& DiceValues)
{
    TMap<int32, int32> Counts;
    for (int32 Val : DiceValues)
    {
        if (Counts.Contains(Val))
        {
            Counts[Val]++;
        }
        else
        {
            Counts.Add(Val, 1);
        }
    }
    return Counts;
}

// 달성 가능한 족보 리스트 반환
TArray<FAchievedJokbo> UDiceJokboSubsystem::GetAchievableJokbos(const TArray<int32>& DiceValues)
{
    TArray<FAchievedJokbo> Result;

    // 유물도 구현하려면 여기서 체크해서 넘기기

    for (const FJokboDefinition& Def : AllJokbos)
    {
        //조건 체크
        if (Def.CheckLogic && Def.CheckLogic(DiceValues))
        {
            FAchievedJokbo Achieved;
            Achieved.Description = Def.Description;

            //가변/고정 데미지 계산
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
    Result.Sort([](const FAchievedJokbo& A, const FAchievedJokbo& B) {
        return A.FinalDamage > B.FinalDamage;
    });

    return Result;
}