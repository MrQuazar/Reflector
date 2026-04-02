#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAI.generated.h"

class UHealthComponent;

UCLASS()
class REFLECTOR_API AEnemyAI : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyAI();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:

    // Components
    UPROPERTY(VisibleAnywhere)
    UHealthComponent* HealthComponent;

    // Settings
    UPROPERTY(EditAnywhere, Category = "AI")
    float ChaseSpeed = 400.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DamageAmount = 100.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DamageDistance = 100.f;

    // Cached Player
    APawn* PlayerPawn;

    void ChasePlayer();
    void TryDamagePlayer();
};