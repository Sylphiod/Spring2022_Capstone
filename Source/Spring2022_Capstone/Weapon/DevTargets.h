// Created by Spring2022_Capstone team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DevTargets.generated.h"

UCLASS()
class SPRING2022_CAPSTONE_API ADevTargets : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADevTargets();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TargetStaticMesh;

	UPROPERTY(EditAnywhere)
	UMaterial* DefaultMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* HitMaterial;

	void ToggleMaterial();
	
	
	//UPROPERTY(EditAnywhere)
	//USkeletalMeshComponent* TargetSkeletalMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
