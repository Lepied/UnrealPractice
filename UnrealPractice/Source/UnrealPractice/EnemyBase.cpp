#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"

AEnemyBase::AEnemyBase()
{
    // 기본 설정 (캡슐 콜리전같은것들)
    PrimaryActorTick.bCanEverTick = true;
    
    //초기값
    MaxHP = 100.0f;
    CurrentHP = MaxHP;
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    //  데이터 테이블에서 가져오기
    if (EnemyDataTable && !RowName.IsNone())
    {
        static const FString ContextString(TEXT("Enemy Stat Context"));
        // 테이블에서 RowName에 해당하는 줄을 찾아서 FEnemyStat 형태로
        FEnemyStat* Stat = EnemyDataTable->FindRow<FEnemyStat>(RowName, ContextString, true);

        if (Stat)
        {
            // 덮어씌우기
            MaxHP = Stat->MaxHP;
            CurrentHP = MaxHP;
            
            // 이름이든 메쉬든 공격력이든 여기서 다 바꾸기
        }
    }
}

// 데미지 로직
float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    // 체력 깎기
    CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.0f, MaxHP);
    UE_LOG(LogTemp, Warning, TEXT("적 HP: %f"), CurrentHP);

    if (CurrentHP <= 0.0f)
    {
        Die();
    }

    return ActualDamage;
}

void AEnemyBase::Die()
{
    // 사망 애니메이션 재생, 콜리전 끄기 등등등
    UE_LOG(LogTemp, Warning, TEXT("적사망"));
    
    // 래그돌(시체) 효과
    GetMesh()->SetSimulatePhysics(true);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    // 일정 시간 후 삭제
    SetLifeSpan(5.0f);
}