// Created by Spring2022_Capstone team


#include "RecoilComponent.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URecoilComponent::URecoilComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void URecoilComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningParentWeapon = Cast<AWeaponBase>(GetOwner()); 
	
	OwnersPlayerController = GetWorld()->GetFirstPlayerController();
	
	bOriginalAimRotSet = false;
	
}

void URecoilComponent::RecoilStart()
{
	
	GetWorld()->GetTimerManager().SetTimer(TimeSinceLastShotTimerHandle, this, &URecoilComponent::RecoilStop, TimeBeforeRecovery); // Set timer to check if player has stopped firing.
	
	PlayerDeltaRot = FRotator::ZeroRotator;
	RecoilDeltaRot = FRotator::ZeroRotator;
	Del = FRotator::ZeroRotator;
	if(bOriginalAimRotSet == false)
	{
		RecoilStartRot = OwnersPlayerController->GetControlRotation();
		bOriginalAimRotSet = true;
	}
	
	bIsFiring = true;
	
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &URecoilComponent::RecoilTimerFunction, RecoverySpeed, false);
	
	bRecoil = true;
	bRecoilRecovery = false;
	
}

void URecoilComponent::RecoilStop()
{
	bIsFiring = false;
}

void URecoilComponent::RecoveryStart()
{
	if(OwnersPlayerController->GetControlRotation().Pitch > RecoilStartRot.Pitch)
	{
		bRecoilRecovery = true;
		GetWorld()->GetTimerManager().SetTimer(RecoveryTimerHandle, this, &URecoilComponent::RecoveryTimerFunction, 0.5, false); // Note - Leave 0.5 Issues with moving and shooting when not.
	}
	else
	{
		bOriginalAimRotSet = false;
		PlayerDeltaRot = FRotator::ZeroRotator;
		RecoilDeltaRot = FRotator::ZeroRotator;
	}
	
}

void URecoilComponent::RecoveryTimerFunction()
{
	bRecoilRecovery = false;
	
	// This is the last called method in Recoil execution. After recoil finishes we want to have to OriginalAimRot reset on next fire.
	bOriginalAimRotSet = false; // Reset aim point after recoil recovery has finished
	
}

void URecoilComponent::RecoilTimerFunction()
{
	bRecoil = false;
	GetWorld()->GetTimerManager().PauseTimer(FireTimerHandle);
}

// Called every frame
void URecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bRecoil)
	{

		Del.Roll = 0;
		Del.Pitch = VerticalKickAmount; 

		PlayerDeltaRot = OwnersPlayerController->GetControlRotation() - RecoilStartRot - RecoilDeltaRot;
		OwnersPlayerController->SetControlRotation(RecoilStartRot + PlayerDeltaRot + Del);
		RecoilDeltaRot = Del;
		
		if(!bIsFiring)
		{
			GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
			bRecoil = false;
			RecoveryStart();
		}
		
	}
	else if(bRecoilRecovery)
	{
		// Reset Recoil
		FRotator TmpRot = OwnersPlayerController->GetControlRotation(); 

		if(TmpRot.Pitch >= RecoilStartRot.Pitch)						
		{

			// We want to reset the vertical recoil, but maintain horizontal change.
			FRotator AimResetRotation = FRotator(RecoilStartRot.Pitch + PlayerDeltaRot.Pitch, OwnersPlayerController->GetControlRotation().Yaw, OwnersPlayerController->GetControlRotation().Roll);

			OwnersPlayerController->SetControlRotation(UKismetMathLibrary::RInterpTo(TmpRot, AimResetRotation, DeltaTime, RecoverySpeed));
			
		}
		else
		{
			RecoveryTimerHandle.Invalidate();
		}
	}
}
