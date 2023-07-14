// Created by Spring2022_Capstone team

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Engine.h"
#include "RangedEnemy.generated.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


/**
 *
 */
UCLASS()
class SPRING2022_CAPSTONE_API ARangedEnemy : public ABaseEnemy
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
		
	

public:
	// Sets default values for this character's properties
	ARangedEnemy();

protected: 
	virtual void Attack() override;

private:

	void Raycast();

	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
	int MagazineSize;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
	int Ammo;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (AllowPrivateAccess = true))
	float ReloadTime;
	float RangeDistance

		UPROPERTY(EditAnywhere, meta = (AllowedPrivateAccess = True)
		TSubclassOf<AActor> ActorToSpawn;
	void Reload();
};
