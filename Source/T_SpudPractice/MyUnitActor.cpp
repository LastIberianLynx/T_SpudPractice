// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUnitActor.h"
#include "MySaveGame.h"
#include "MyGameInstance.h"
#include "CharacterObj.h"
#include "UnitManager.h"

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
		if (MyGI->bIsLoadingFromSave) {
			MyGI->RequestLoad(this);
			return;
		}
	}



	if (!UnitManager) {
		UnitManager = Cast<AUnitManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUnitManager::StaticClass()));
	}

	if (!CharacterObj)
	{
		CharacterObj = NewObject<UCharacterObj>(this, UCharacterObj::StaticClass());
		CharacterObj->CharacterName = TEXT("Knight");
		CharacterObj->Level = 1;
		CharacterObj->Unit = this;
		CharacterObj->UnitManager = UnitManager;
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
	
	return;
	return;
	return;

	SaveGameRef->SavedHP = HP;
	//CharacterObj->Getname
	CharacterObj->Rename(*CharacterObj->GetName(), SaveGameRef);
	SaveGameRef->Character = CharacterObj;


}

void AMyUnitActor::LoadData_Implementation(UMySaveGame* SaveGameRef)
{
	if (!IsValid(SaveGameRef))
		return;

		return;
		return;
		return;
	
		HP = SaveGameRef->SavedHP;
	//CharacterObj = SaveGameRef->Character;
	if (!IsValid(SaveGameRef->Character))
		return;
	//FString Yo = "Yo";
	SaveGameRef->Character->Rename(*SaveGameRef->Character->GetName(), this);
	CharacterObj = Cast<UCharacterObj>(SaveGameRef->Character);

}