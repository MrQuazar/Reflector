#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REFLECTOR_API UParryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UParryComponent();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Parry")
    float ParryWindow = 0.15f;

    bool bIsParrying = false;

    FTimerHandle ParryTimer;

    void EndParry();

public:
    void StartParry();

    bool IsParrying() const { return bIsParrying; }
};