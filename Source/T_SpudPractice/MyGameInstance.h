// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyPersistence.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSaveCompleted, UMySaveGame*, SavedGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameLoadCompleted, UMySaveGame*, SavedGame);


class UMySaveGame;
class UAsyncActionHandleSaveGame;

/**
 * 
 */
UCLASS(Blueprintable)
class T_SPUDPRACTICE_API UMyGameInstance : public UGameInstance, public IMyPersistence
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
		UMySaveGame* SaveGameRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SaveGameSlotName = TEXT("MySaveSlot");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 UserIndex = 0;

	UFUNCTION(BlueprintCallable)
		void SaveGame(bool bAsync);

	UFUNCTION(BlueprintCallable, Category = "Save System")
		virtual void RequestSave(bool bAsync) override;

	UFUNCTION(BlueprintCallable, Category = "Save System")
		virtual void RequestLoad(UObject* Requester) override;

	UFUNCTION(BlueprintCallable)
		void CreateSaveGame();

	UFUNCTION(BlueprintCallable)
		void LoadGame(bool bAsync);

	UPROPERTY()
		bool bIsLoadingFromSave = false;



private:
	UFUNCTION()
		void OnAsyncSaveCompleted(USaveGame* SavedGame, bool bSuccess);	
	
	UFUNCTION()
		void OnAsyncLoadCompleted(USaveGame* SavedGame, bool bSuccess);


	UPROPERTY(BlueprintAssignable, Category = "Save System")
		FOnGameSaveCompleted OnGameSaveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Save System")
		FOnGameLoadCompleted OnGameLoadCompleted;

	UPROPERTY()
		UAsyncActionHandleSaveGame* CurrentAsyncSaveHandle;




	
};
