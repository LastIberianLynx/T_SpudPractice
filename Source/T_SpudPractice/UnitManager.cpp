// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitManager.h"
#include "MySaveGame.h"
#include "MyRegionActor.h"
#include "MyUnitActor.h"
#include "MyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


// Sets default values
AUnitManager::AUnitManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitManager::BeginPlay()
{
	Super::BeginPlay();
    UMyGameInstance* MyGI = Cast<UMyGameInstance>(GetGameInstance());
    if(!MyGI->bIsLoadingFromSave)
    SpawnRegions();
    else
        MyGI->RequestLoad(this);
}

// Called every frame
void AUnitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnitManager::SpawnUnit(FTransform T, float HP) {


}


void AUnitManager::SaveData_Implementation(UMySaveGame* SaveGameRef)
{
	if (!IsValid(SaveGameRef))
		return;

    SaveGameRef->SerializedUnitActors.Empty();

    for (AMyUnitActor* Unit : All_UnitActors)
    {
        if (!IsValid(Unit)) continue;

        FSerializedActorData Entry;
        //Entry.ActorClass = Unit->GetClass();

        Unit->LastTransform = Unit->GetActorTransform();

        Entry.SavedName = Unit->GetPathName();

        Entry.ActorClass = Unit->GetClass();
        Entry.ActorPath = Unit->GetPathName();

        Entry.ReferencePathName1 = Unit->CurrentRegion->GetPathName();

        FMemoryWriter Writer(Entry.Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Writer, false);
        Ar.ArIsSaveGame = true;
        Unit->Serialize(Ar);

        SaveGameRef->SerializedUnitActors.Add(Entry);
    }

    SaveGameRef->SerializedRegions.Empty();


    for (AMyRegionActor* Region : All_RegionActors)
    {
        if (!IsValid(Region)) continue;

        FSerializedActorData Entry;
        //Entry.ActorClass = Unit->GetClass();
        Region->LastTransform = Region->GetActorTransform();

        Entry.SavedName = Region->GetPathName();

        Entry.ActorClass = Region->GetClass();
        Entry.ActorPath = Region->GetPathName();
        Entry.ReferencePathName1 = Region->CurrentUnit->GetPathName();

        FMemoryWriter Writer(Entry.Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Writer, false);
        Ar.ArIsSaveGame = true;
        Region->Serialize(Ar);

        SaveGameRef->SerializedRegions.Add(Entry);
    }
}

void AUnitManager::LoadData_Implementation(UMySaveGame* SaveGameRef)
{
	if (!IsValid(SaveGameRef))
		return;

    All_UnitActors.Empty();
    All_RegionActors.Empty();

    // PASS 1 - SPAWN ONLY

    TMap<FString, AMyUnitActor*> ID_Unit_Map;
    TMap<FString, AMyRegionActor*> ID_Region_Map;
    // === PASS 1 - SPAWN + RENAME ===

    // Spawn Units
    for (const FSerializedActorData& LoadedData : SaveGameRef->SerializedUnitActors)
    {
        AActor* CurrentActor = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, LoadedData.ActorClass, FTransform(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        CurrentActor->FinishSpawning(FTransform());

        FString IDName;// old actor path (old reference)

        LoadedData.ActorPath.Split(".", nullptr, &IDName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
        CurrentActor->Rename(*IDName);
        All_UnitActors.Add(Cast<AMyUnitActor>(CurrentActor));
    }

    // Spawn Regions
    for (const FSerializedActorData& LoadedData : SaveGameRef->SerializedRegions)
    {
        AActor* CurrentActor = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, LoadedData.ActorClass, FTransform(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        CurrentActor->FinishSpawning(FTransform());

        FString IDName;// old actor path (old reference)

        LoadedData.ActorPath.Split(".", nullptr, &IDName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
        CurrentActor->Rename(*IDName);
        All_RegionActors.Add(Cast<AMyRegionActor>(CurrentActor));

    }

    // === PASS 2 - DESERIALIZE ===

    // Deserialize Units
    for (int32 i = 0; i < SaveGameRef->SerializedUnitActors.Num(); ++i)
    {
        FMemoryReader Reader(SaveGameRef->SerializedUnitActors[i].Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Reader, true);
        Ar.ArIsSaveGame = true;

        All_UnitActors[i]->Serialize(Ar);
        All_UnitActors[i]->SetActorTransform(All_UnitActors[i]->LastTransform);
    }

    // Deserialize Regions
    for (int32 i = 0; i < SaveGameRef->SerializedRegions.Num(); ++i)
    {
        FMemoryReader Reader(SaveGameRef->SerializedRegions[i].Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Reader, true);
        Ar.ArIsSaveGame = true;

        All_RegionActors[i]->Serialize(Ar);
        All_RegionActors[i]->SetActorTransform(All_RegionActors[i]->LastTransform);

    }






    //TMap<FString, AMyUnitActor*> ID_Unit_Map;   
    //TMap<FString, AMyRegionActor*> ID_Region_Map;
    //for (const FSerializedActorData& Entry : SaveGameRef->SerializedUnitActors)
    //{
    //    AMyUnitActor* Unit = GetWorld()->SpawnActor<AMyUnitActor>(UnitClass);
    //    //AMyUnitActor* Unit = GetWorld()->SpawnActor<AMyUnitActor>(Entry.ActorClass);
    //    if (!Unit) continue;

    //    FMemoryReader Reader(Entry.Data, true);
    //    FObjectAndNameAsStringProxyArchive Ar(Reader, true);
    //    Ar.ArIsSaveGame = true;
    //    Unit->Serialize(Ar);
    //    ID_Unit_Map.Add(Unit->GetPathName(), Unit);
    //    Unit->SetActorTransform(Unit->LastTransform);
    //    All_UnitActors.Add(Unit);
    //}


    //All_RegionActors.Empty();

    //for (const FSerializedActorData& Entry : SaveGameRef->SerializedRegions)
    //{
    //    //AMyRegionActor* Region = GetWorld()->SpawnActor<AMyRegionActor>(Entry.ActorClass);
    //    AMyRegionActor* Region = GetWorld()->SpawnActor<AMyRegionActor>(RegionClass);
    //    if (!Region) continue;

    //    FMemoryReader Reader(Entry.Data, true);
    //    FObjectAndNameAsStringProxyArchive Ar(Reader, true);
    //    Ar.ArIsSaveGame = true;
    //    Region->Serialize(Ar);

    //    ID_Region_Map.Add(Region->GetPathName(), Region);

    //    Region->SetActorTransform(Region->LastTransform);
    //    All_RegionActors.Add(Region);
    //}

    //for (int32 i = 0; i < SaveGameRef->SerializedUnitActors.Num(); ++i) {
    //    AMyUnitActor* Unit = All_UnitActors[i];
    //    FSerializedActorData& Entry = SaveGameRef->SerializedUnitActors[i];
    //    Unit->CurrentRegion = *ID_Region_Map.Find(Entry.ReferencePathName1);

    //}



    //TMap<FString, AMyRegionActor*> RegionPathMap;

    //for (AMyRegionActor* Region : All_RegionActors)
    //{
    //    FString Path = Region->GetPathName();
    //    RegionPathMap.Add(Path, Region);
    //}


}


void AUnitManager::SpawnRegions() {


        if (!RegionClass) return; // Ensure RegionClass is set

        const int32 NumRows = 2;
        const int32 NumCols = 2;
        const float Spacing = 150.f; // Distance between regions

        FVector Origin = FVector::ZeroVector;

        for (int32 Row = 0; Row < NumRows; ++Row)
        {
            for (int32 Col = 0; Col < NumCols; ++Col)
            {
                FVector SpawnLocation = Origin + FVector(Row * Spacing, Col * Spacing, 0.f);
                FRotator SpawnRotation = FRotator::ZeroRotator;

                AMyRegionActor* Region = GetWorld()->SpawnActor<AMyRegionActor>(RegionClass, SpawnLocation, SpawnRotation);
                Region->LastTransform = Region->GetActorTransform();
                All_RegionActors.Add(Region);
            }
        }

}


AMyRegionActor* AUnitManager::GetClosestRegion(const FVector& Location) const
{
    AMyRegionActor* ClosestRegion = nullptr;
    float MinDistanceSq = FLT_MAX;

    for (AMyRegionActor* Region : All_RegionActors)
    {
        if (!Region) continue;

        float DistSq = FVector::DistSquared(Location, Region->GetActorLocation());
        if (DistSq < MinDistanceSq)
        {
            MinDistanceSq = DistSq;
            ClosestRegion = Region;
        }
    }

    return ClosestRegion;
}
