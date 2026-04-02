#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Health, float, Damage);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REFLECTOR_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Replicated, Category = "Health")
    float MaxHealth = 100.f;

    UPROPERTY(ReplicatedUsing = OnRep_Health)
    float CurrentHealth;

    UFUNCTION()
    void OnRep_Health();

    UFUNCTION()
    void HandleDeath();

public:
    UFUNCTION(BlueprintCallable)
    void ApplyDamage(float Damage, AActor* DamageCauser);

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;

    float GetHealth() const { return CurrentHealth; }

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};