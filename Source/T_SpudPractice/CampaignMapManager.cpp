// Fill out your copyright notice in the Description page of Project Settings.


#include "CampaignMapManager.h"

// Sets default values
ACampaignMapManager::ACampaignMapManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACampaignMapManager::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < 4; ++i) {


	}
	
}

// Called every frame
void ACampaignMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

