#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "ParrySpotlight.generated.h"

UCLASS()
class REFLECTOR_API AParrySpotlight : public AActor
{
    GENERATED_BODY()

public:
    AParrySpotlight();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere)
    USpotLightComponent* Light;

    float LifeTime = 0.3f; // quick flash
};