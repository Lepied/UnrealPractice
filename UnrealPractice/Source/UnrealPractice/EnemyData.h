#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "EnemyData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyStat : public FTableRowBase // 데이터 테이블용 구조체는 이게 꼭 필요함
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EnemyName; // 적 이름

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHP; // 최대 체력

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackPower; // 공격력

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USkeletalMesh* EnemyMesh; // 나중에 메쉬도 여기서하게하기
    
};