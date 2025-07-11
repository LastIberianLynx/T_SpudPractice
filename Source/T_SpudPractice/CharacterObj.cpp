// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterObj.h"
#include "MySaveGame.h"
#include "MyGameInstance.h"

void UCharacterObj::SaveData_Implementation(UMySaveGame* SaveGameRef)
{
	if (!IsValid(SaveGameRef))
		return;
	//SaveGameRef->SavedHP = HP;


}

void UCharacterObj::LoadData_Implementation(UMySaveGame* SaveGameRef)
{
	if (!IsValid(SaveGameRef))
		return;
	//HP = SaveGameRef->SavedHP;
}