#include "ParrySpotlight.h"
#include "Components/SpotLightComponent.h"

AParrySpotlight::AParrySpotlight()
{
    PrimaryActorTick.bCanEverTick = false;

    Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
    RootComponent = Light;

    Light->Intensity = 50000.f;
    Light->bUseInverseSquaredFalloff = false;
}

void AParrySpotlight::BeginPlay()
{
    Super::BeginPlay();

    SetLifeSpan(LifeTime);
}