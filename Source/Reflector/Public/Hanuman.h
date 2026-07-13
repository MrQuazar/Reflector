#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Hanuman.generated.h"

UCLASS()
class REFLECTOR_API AHanuman : public AActor
{
    GENERATED_BODY()

public:
    AHanuman();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    float LifeTime = 3.0f; // how long it stays

    // Degrees per second; set to 0 in the editor to disable rotation
    UPROPERTY(EditAnywhere)
    float RotationSpeed = 90.0f;
};