// Created by Spring2022_Capstone team


#include "Enemies/RangedEnemyHit.h"

// Sets default values
ARangedEnemyHit::ARangedEnemyHit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARangedEnemyHit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangedEnemyHit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

