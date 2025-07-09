// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUnitActor.h"
#include "MySaveGame.h"
#include "MyGameInstance.h"

// Sets default values
AMyUnitActor::AMyUnitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyUnitActor::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstance* MyGI = Cast<UMyGameInstance>(GetGameInstance());
	if (MyGI)
	{
		MyGI->RequestLoad(this);
	}
	
}

// Called every frame
void AMyUnitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyUnitActor::SaveData_Implementation(UMySaveGame* SaveGameRef)
{
	if (!IsValid(SaveGameRef))
		return;
	SaveGameRef->SavedHP = HP;


}

void AMyUnitActor::LoadData_Implementation(UMySaveGame* SaveGameRef)
{
	if (!IsValid(SaveGameRef))
		return;
	HP = SaveGameRef->SavedHP;
}