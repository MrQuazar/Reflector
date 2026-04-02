#include "HealthComponent.h"
#include "ParryComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
    SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
}

void UHealthComponent::OnRep_Health()
{
    OnHealthChanged.Broadcast(CurrentHealth, 0);
}

void UHealthComponent::ApplyDamage(float Damage, AActor* DamageCauser)
{
    if (Damage <= 0.f) return;

    // Check for parry
    if (AActor* Owner = GetOwner())
    {
        UParryComponent* Parry = Owner->FindComponentByClass<UParryComponent>();
        if (Parry && Parry->IsParrying())
        {
            // Reflect damage
            if (DamageCauser)
            {
                if (UHealthComponent* EnemyHealth = DamageCauser->FindComponentByClass<UHealthComponent>())
                {
                    EnemyHealth->ApplyDamage(Damage, Owner);
                }
            }

            UE_LOG(LogTemp, Warning, TEXT("Parry Successful!"));
            return;
        }
    }


    if (CurrentHealth <= 0.f)
    {
        HandleDeath();
    }

    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, Damage);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(UHealthComponent, CurrentHealth);
    DOREPLIFETIME(UHealthComponent, MaxHealth);
}

void UHealthComponent::HandleDeath()
{
    AActor* Owner = GetOwner();
    if (Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s Died"), *Owner->GetName());
        Owner->Destroy();
    }
}