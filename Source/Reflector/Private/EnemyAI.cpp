#include "EnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"

AEnemyAI::AEnemyAI()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAIController::StaticClass();
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AEnemyAI::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}

void AEnemyAI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerPawn) return;

    ChasePlayer();
    TryDamagePlayer();
}

void AEnemyAI::ChasePlayer()
{
    AAIController* AIController = Cast<AAIController>(GetController());

    if (AIController)
    {
        // Try NavMesh movement
        AIController->MoveToActor(PlayerPawn, 5.f);
    }
    else
    {
        // Fallback: direct movement
        FVector Direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        AddMovementInput(Direction, 1.0f);
    }
}

void AEnemyAI::TryDamagePlayer()
{
    float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

    if (Distance <= DamageDistance)
    {
        if (UHealthComponent* PlayerHealth = PlayerPawn->FindComponentByClass<UHealthComponent>())
        {
            PlayerHealth->ApplyDamage(DamageAmount, this);
        }
    }
}