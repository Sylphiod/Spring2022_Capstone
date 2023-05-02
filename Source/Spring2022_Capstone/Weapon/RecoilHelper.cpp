// Created by Spring2022_Capstone team


#include "RecoilHelper.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URecoilHelper::URecoilHelper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void URecoilHelper::BeginPlay()
{
	Super::BeginPlay();

	OwningParentWeapon = Cast<AWeaponBase>(GetOwner()); 
	
	OwnersPlayerController = GetWorld()->GetFirstPlayerController();
	
	bOriginalAimRotSet = false;
	
}

void URecoilHelper::RecoilStart()
{
	
	GetWorld()->GetTimerManager().SetTimer(TimeSinceLastShotTimerHandle, this, &URecoilHelper::RecoilStop, TimeBeforeRecovery);
	
	PlayerDeltaRot = FRotator::ZeroRotator;
	RecoilDeltaRot = FRotator::ZeroRotator;
	Del = FRotator::ZeroRotator;
	if(bOriginalAimRotSet == false)
	{
		RecoilStartRot = OwnersPlayerController->GetControlRotation();
		bOriginalAimRotSet = true;
	}
	
	bIsFiring = true;
	
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &URecoilHelper::RecoilTimerFunction, 10.0f, false); // Note: I forgot what the deal with 10.0f is. But changing it causes problems. This might have been RecoverySpeed float before.
	
	bRecoil = true;
	bRecoilRecovery = false;
	
}

void URecoilHelper::RecoilStop()
{
	bIsFiring = false;
}

void URecoilHelper::RecoveryStart()
{
	if(OwnersPlayerController->GetControlRotation().Pitch > RecoilStartRot.Pitch)
	{
		bRecoilRecovery = true;
		GetWorld()->GetTimerManager().SetTimer(RecoveryTimerHandle, this, &URecoilHelper::RecoveryTimerFunction, RecoveryTime, false);
	}

}

void URecoilHelper::RecoveryTimerFunction()
{
	bRecoilRecovery = false;
	
	// This is the last called method in Recoil execution. After recoil finishes we want to have to OriginalAimRot reset on next fire.
	bOriginalAimRotSet = false; // Reset aim point after recoil recovery has finished
}

void URecoilHelper::RecoilTimerFunction()
{
	bRecoil = false;
	GetWorld()->GetTimerManager().PauseTimer(FireTimerHandle);
}

// Called every frame
void URecoilHelper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
			FRotator AimResetRotation = FRotator(RecoilStartRot.Pitch, OwnersPlayerController->GetControlRotation().Yaw, OwnersPlayerController->GetControlRotation().Roll);

			OwnersPlayerController->SetControlRotation(UKismetMathLibrary::RInterpTo(OwnersPlayerController->GetControlRotation(), AimResetRotation, DeltaTime, 10.0f)); // ToDo: I think the InterpSpeed(10) would be something like RecoilRecoveryRate or something along those lines.
		}
		else
		{
			RecoveryTimerHandle.Invalidate();
		}
	}
}
