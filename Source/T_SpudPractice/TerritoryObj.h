// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TerritoryObj.generated.h"

/**
 * 
 */

class UCharacterObj;

UCLASS()
class T_SPUDPRACTICE_API UTerritoryObj : public UObject
{
	GENERATED_BODY()

		TArray<UCharacterObj*> CharactersInTerritory;
	
};
