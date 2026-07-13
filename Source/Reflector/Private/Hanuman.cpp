#include "Hanuman.h"
#include "Components/StaticMeshComponent.h"

AHanuman::AHanuman()
{
    // Tick is needed to drive the rotation; set RotationSpeed = 0 to skip it at runtime
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // Disable all collisions
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Mesh->SetGenerateOverlapEvents(false);
}

void AHanuman::BeginPlay()
{
    Super::BeginPlay();

    // Auto destroy after a few seconds
    SetLifeSpan(LifeTime);
}

void AHanuman::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (RotationSpeed != 0.0f)
    {
        AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
    }
}