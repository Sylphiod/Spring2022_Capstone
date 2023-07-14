// Created by Spring2022_Capstone team


#include "Enemies/SniperEnemy.h"

// Sets default values
ASniperEnemy::ASniperEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASniperEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASniperEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASniperEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

