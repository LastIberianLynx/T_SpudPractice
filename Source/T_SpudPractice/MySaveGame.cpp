// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"
#include "SaveableInterface.h"

void UMySaveGame::SaveAllData(TArray<UObject*> SaveableObjects) {


		for (UObject* Object : SaveableObjects)
		{
			if (!Object) continue;

			if (Object->GetClass()->ImplementsInterface(USaveableInterface::StaticClass()))
			{
				ISaveableInterface::Execute_SaveData(Object, this);
			}
		}
}

void UMySaveGame::LoadDataForRequester(UObject* Requester) {


		if (Requester->GetClass()->ImplementsInterface(USaveableInterface::StaticClass()))
		{
			ISaveableInterface::Execute_LoadData(Requester, this);
		}


}