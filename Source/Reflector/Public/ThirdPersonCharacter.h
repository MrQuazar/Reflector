#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "ThirdPersonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UParryComponent;
class UHealthComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class REFLECTOR_API AThirdPersonCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AThirdPersonCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // INPUT
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartSprint();
    void StopSprint();
    void StartParry();

    UFUNCTION(Server, Reliable)
    void ServerSetSprint(bool bSprint);

    // Animation helpers
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Animation")
    float Speed;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Animation")
    bool bIsInAir;

    UPROPERTY(Replicated, BlueprintReadOnly, Replicated, Category = "Movement")
    bool bIsSprinting = false;

    //COMBAT
    UPROPERTY(VisibleAnywhere, Category = "Combat")
    class UParryComponent* ParryComponent;
    UPROPERTY(VisibleAnywhere, Category = "Combat")
    class UHealthComponent* HealthComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    UAnimMontage* ParryMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float ParryMontageSpeed = 3.0f;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

    // COMPONENTS
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UCameraComponent* FollowCamera;

    // INPUT ACTIONS
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* ParryAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* DefaultMappingContext;


    // MOVEMENT
    UPROPERTY(EditAnywhere, Category = "Movement")
    float WalkSpeed = 500.f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintSpeed = 900.f;
};