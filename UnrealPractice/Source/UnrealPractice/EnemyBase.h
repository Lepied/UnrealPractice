#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyData.h" 
#include "EnemyBase.generated.h"

UCLASS()
class UNREALPRACTICE_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyBase();

protected:
    virtual void BeginPlay() override;

public:
    // 언리얼 표준 피격 함수 (족보 공격 맞을 때 이거 호출됨)
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    // 데이터 테이블용 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stat")
    UDataTable* EnemyDataTable; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stat")
    FName RowName; 

protected:
    // 실제 게임에서 쓸 변수들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
    float CurrentHP;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
    float MaxHP;

    // 죽었을 때
    void Die();
};