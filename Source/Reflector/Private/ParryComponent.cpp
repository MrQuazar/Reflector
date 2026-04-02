#include "ParryComponent.h"
#include "TimerManager.h"

UParryComponent::UParryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UParryComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UParryComponent::StartParry()
{
    if (bIsParrying) return;

    bIsParrying = true;

    GetWorld()->GetTimerManager().SetTimer(
        ParryTimer,
        this,
        &UParryComponent::EndParry,
        ParryWindow,
        false
    );
}

void UParryComponent::EndParry()
{
    bIsParrying = false;
}