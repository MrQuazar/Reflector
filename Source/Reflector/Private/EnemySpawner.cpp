#include "EnemySpawner.h"
#include "EnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AEnemySpawner::AEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    ScheduleNextSpawn();
}

void AEnemySpawner::ScheduleNextSpawn()
{
    float RandomDelay = SpawnInterval + FMath::FRandRange(-SpawnIntervalVariance, SpawnIntervalVariance);
    RandomDelay = FMath::Max(0.5f, RandomDelay); // prevent negative or too small

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AEnemySpawner::SpawnEnemy,
        RandomDelay,
        false
    );
}

void AEnemySpawner::SpawnEnemy()
{
    if (!EnemyClass || SpawnPoints.Num() == 0)
    {
        ScheduleNextSpawn();
        return;
    }

    if (CurrentAliveEnemies >= MaxAliveEnemies)
    {
        ScheduleNextSpawn();
        return;
    }

    int32 Index = FMath::RandRange(0, SpawnPoints.Num() - 1);

    FVector SpawnLocation = SpawnPoints[Index]->GetActorLocation();
    FRotator SpawnRotation = SpawnPoints[Index]->GetActorRotation();

    AEnemyAI* Enemy = GetWorld()->SpawnActor<AEnemyAI>(EnemyClass, SpawnLocation, SpawnRotation);

    if (Enemy)
    {
        CurrentAliveEnemies++;

        // When enemy dies reduce count
        Enemy->OnDestroyed.AddDynamic(this, &AEnemySpawner::OnEnemyDestroyed);
    }

    ScheduleNextSpawn();
}

void AEnemySpawner::OnEnemyDestroyed(AActor* DestroyedActor)
{
    CurrentAliveEnemies--;
}