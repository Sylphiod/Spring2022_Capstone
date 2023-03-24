// Created by Spring2022_Capstone team

#include "PlayerCharacter.h"

#include <string>

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Weapon/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


APlayerCharacter::APlayerCharacter()
{
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(RootComponent);
    Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
    Camera->bUsePawnControlRotation = true;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
        EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Completed, this, &APlayerCharacter::SwitchWeapon);
    }
}


void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(CharacterMappingContext, 0);
        }
    }
}

void APlayerCharacter::Move(const FInputActionValue &Value)
{
    const FVector2D DirectionalValue = Value.Get<FVector2D>();
    if (GetController() && (DirectionalValue.X != 0.f || DirectionalValue.Y != 0.f))
    {
        AddMovementInput(GetActorForwardVector(), DirectionalValue.Y * Speed * UGameplayStatics::GetWorldDeltaSeconds(this));
        AddMovementInput(GetActorRightVector(), DirectionalValue.X * Speed * UGameplayStatics::GetWorldDeltaSeconds(this));
    }
}

void APlayerCharacter::Look(const FInputActionValue &Value)
{
    const FVector2D LookAxisValue = Value.Get<FVector2D>();
    if (GetController())
    {
        AddControllerYawInput(LookAxisValue.X * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this));
        AddControllerPitchInput(LookAxisValue.Y * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this));
    }
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
    ActiveWeapon->Shoot();
}

void APlayerCharacter::SwitchWeapon(const FInputActionValue& Value)
{
    ActiveWeapon = (ActiveWeapon == Weapon1) ? Weapon2 : Weapon1;
}
    
void APlayerCharacter::SetWeapon1(AWeaponBase* Weapon)
{
    Weapon1 = Weapon;
    ActiveWeapon = Weapon1;
}

void APlayerCharacter::SetWeapon2(AWeaponBase* Weapon)
{
    Weapon2 = Weapon;
    ActiveWeapon = Weapon2;
}

AWeaponBase* APlayerCharacter::GetWeapon1() const
{
    return Weapon1;
}

AWeaponBase* APlayerCharacter::GetWeapon2() const
{
    return Weapon2;
}


