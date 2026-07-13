#include "ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ParryComponent.h"
#include "HealthComponent.h"
#include "Hanuman.h"
#include "ParrySpotlight.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"

AThirdPersonCharacter::AThirdPersonCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    // Capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Disable controller rotation
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Camera Boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->bUsePawnControlRotation = true;

    // Follow Camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom);
    FollowCamera->bUsePawnControlRotation = false;

    SetReplicates(true);
    SetReplicateMovement(true);

    ParryComponent = CreateDefaultSubobject<UParryComponent>(TEXT("ParryComponent"));
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AThirdPersonCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (HealthComponent)
    {
        HealthComponent->OnParrySuccess.AddDynamic(this, &AThirdPersonCharacter::SpawnHanuman);
    }

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
}

void AThirdPersonCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Speed (horizontal only)
    FVector Velocity = GetVelocity();
    Velocity.Z = 0;
    Speed = Velocity.Size();

    // Air state
    bIsInAir = GetCharacterMovement()->IsFalling();
}

void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Look);

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);

        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AThirdPersonCharacter::StartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AThirdPersonCharacter::StopSprint);

        EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &AThirdPersonCharacter::StartParry);
    }
}

void AThirdPersonCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(Forward, MovementVector.Y);
        AddMovementInput(Right, MovementVector.X);
    }
}

void AThirdPersonCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxis = Value.Get<FVector2D>();

    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}

void AThirdPersonCharacter::StartSprint()
{
    ServerSetSprint(true);
}

void AThirdPersonCharacter::StopSprint()
{
    ServerSetSprint(false);
}

void AThirdPersonCharacter::ServerSetSprint_Implementation(bool bSprint)
{
    bIsSprinting = bSprint;
    GetCharacterMovement()->MaxWalkSpeed = bSprint ? SprintSpeed : WalkSpeed;
}

void AThirdPersonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AThirdPersonCharacter, bIsSprinting);
}

void AThirdPersonCharacter::SpawnHanuman()
{
    if (!HanumanClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnHanuman: HanumanClass is not set on %s - assign it in the Character Blueprint's Combat category."), *GetName());
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("SpawnHanuman: called, spawning %s"), *HanumanClass->GetName());

    FVector Forward = GetActorForwardVector();

    // Spawn slightly behind + above ground
    FVector SpawnLocation = GetActorLocation() - (Forward * 100.f) + FVector(0, 0, 20.f);

    FRotator SpawnRotation = GetActorRotation();

    FActorSpawnParameters Params;
    Params.Owner = this;

    AHanuman* Hanuman = GetWorld()->SpawnActor<AHanuman>(
        HanumanClass,
        SpawnLocation,
        SpawnRotation,
        Params
    );

    if (Hanuman)
    {
        Hanuman->AttachToComponent(
            GetMesh(),
            FAttachmentTransformRules::KeepWorldTransform
        );
        UE_LOG(LogTemp, Warning, TEXT("SpawnHanuman: spawned and attached %s"), *Hanuman->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnHanuman: SpawnActor failed for class %s"), *HanumanClass->GetName());
    }
}

void AThirdPersonCharacter::StartParry()
{
    if (ParryComponent)
    {
        ParryComponent->StartParry();

        if (ParryMontage)
        {
            PlayAnimMontage(ParryMontage, ParryMontageSpeed);
        }

        // Spawn spotlight
        if (SpotlightClass)
        {
            FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 150.f);

            GetWorld()->SpawnActor<AActor>(
                SpotlightClass,
                SpawnLocation,
                FRotator(-90.f, 0.f, 0.f) // pointing down
            );
        }
    }
}