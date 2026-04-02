#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyAI;
class USceneComponent;

UCLASS()
class REFLECTOR_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

protected:
    virtual void BeginPlay() override;

    void SpawnEnemy();
    void ScheduleNextSpawn();

    FTimerHandle SpawnTimerHandle;

    UFUNCTION()
    void OnEnemyDestroyed(AActor* DestroyedActor);

public:
    // Enemy to spawn
    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<AEnemyAI> EnemyClass;

    // Spawn points (assign in editor)
    UPROPERTY(EditAnywhere, Category = "Spawner")
    TArray<AActor*> SpawnPoints;

    // Base interval
    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 3.0f;

    // Random variance (+/-)
    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnIntervalVariance = 1.5f;

    // Max enemies alive
    UPROPERTY(EditAnywhere, Category = "Spawner")
    int32 MaxAliveEnemies = 10;

private:
    int32 CurrentAliveEnemies = 0;
};