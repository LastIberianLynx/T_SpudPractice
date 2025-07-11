// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDefaultPawn.h"
#include "MyUnitActor.h"
#include "UnitManager.h"

void AMyDefaultPawn::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Default Pawn BeginPlay called."));

    UnitManager = Cast<AUnitManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AUnitManager::StaticClass())
        );


    // Example: Print the player's controller ID
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        UE_LOG(LogTemp, Log, TEXT("PlayerController found: %s"), *PC->GetName());
    }
}

AMyUnitActor* AMyDefaultPawn::SpawnUnit(FTransform T, float UnitHP) {

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.Owner = this;

    FVector SpawnLocation = T.GetLocation(); // Example offset
    FRotator SpawnRotation = FRotator::ZeroRotator;

    AMyUnitActor* SpawnedUnit = GetWorld()->SpawnActor<AMyUnitActor>(UnitClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
    UnitManager->All_UnitActors.Add(SpawnedUnit);
    SpawnedUnit->LastTransform = SpawnedUnit->GetActorTransform();


    if (SpawnedUnit)
    {
        UE_LOG(LogTemp, Log, TEXT("Spawned unit: %s"), *SpawnedUnit->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn unit."));
    }

    return SpawnedUnit;
};
