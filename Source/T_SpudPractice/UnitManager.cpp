// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitManager.h"
#include "MySaveGame.h"
#include "MyRegionActor.h"
#include "MyUnitActor.h"
#include "CharacterObj.h"
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
    if (!MyGI->bIsLoadingFromSave) {
        for (int32 i = 0; i < 2; ++i) {
            UCharacterObj* CharacterObj = NewObject<UCharacterObj>(this, UCharacterObj::StaticClass());
            CharacterObj->CharacterName = TEXT("FootSoldier");
            CharacterObj->Level = 1;
            CharacterObj->UnitManager = this;
            FreeCharacters_Arr.Add(CharacterObj);
        }
    SpawnRegions();
    }
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

    {

        //SaveGameRef->
        FSerializedActorData Entry;

        //this->LastTransform = Unit->GetActorTransform();

        Entry.ActorClass = this->GetClass();
        Entry.ActorPath = this->GetPathName();

        FMemoryWriter Writer(Entry.Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Writer, false);
        Ar.ArIsSaveGame = true;
        this->Serialize(Ar);

        SaveGameRef->SavedUnitManager = Entry;
    }

    for (AMyUnitActor* Unit : All_UnitActors)
    {
        if (!IsValid(Unit)) continue;

        Unit->CharacterObj;
        FSerializedActorData EntryCharacter;

        EntryCharacter.ActorClass = Unit->CharacterObj->GetClass();
        EntryCharacter.ActorPath = Unit->CharacterObj->GetPathName();

        FMemoryWriter Writer2(EntryCharacter.Data, true);
        FObjectAndNameAsStringProxyArchive Ar2(Writer2, false);
        Ar2.ArIsSaveGame = true;
        Unit->CharacterObj->Serialize(Ar2);

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
        SaveGameRef->SerializedCharacters.Add(EntryCharacter);



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


    for (UCharacterObj* FreeCharacter : FreeCharacters_Arr)
    {
        if (!IsValid(FreeCharacter)) continue;

        FSerializedActorData Entry;

        Entry.ActorClass = FreeCharacter->GetClass();
        Entry.ActorPath = FreeCharacter->GetPathName();

        FMemoryWriter Writer(Entry.Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Writer, false);
        Ar.ArIsSaveGame = true;
        FreeCharacter->Serialize(Ar);

        SaveGameRef->SerializedFreeCharacters.Add(Entry);
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

    {
        FSerializedActorData& LoadedData = SaveGameRef->SavedUnitManager;

        FString IDName;// old actor path (old reference)

        LoadedData.ActorPath.Split(".", nullptr, &IDName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
        this->Rename(*IDName);
    }


    // Spawn Units
    for (int32 i = 0; i < SaveGameRef->SerializedUnitActors.Num(); ++i)
    {
        FSerializedActorData& LoadedData = SaveGameRef->SerializedUnitActors[i];
        AActor* CurrentActor = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, LoadedData.ActorClass, FTransform(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        CurrentActor->FinishSpawning(FTransform());

        FString IDName;// old actor path (old reference)

        LoadedData.ActorPath.Split(".", nullptr, &IDName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
        CurrentActor->Rename(*IDName);
        All_UnitActors.Add(Cast<AMyUnitActor>(CurrentActor));

        FSerializedActorData& LoadedDataCharacter = SaveGameRef->SerializedCharacters[i];

        UCharacterObj* NewObj = NewObject<UCharacterObj>(CurrentActor, LoadedDataCharacter.ActorClass);
        if (!NewObj)
            return;
        FString IDNameCharacter;
        if (LoadedDataCharacter.ActorPath.Split(TEXT("."), nullptr, &IDNameCharacter, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
        {
            NewObj->Rename(*IDNameCharacter);
        }
        //Cast<AMyUnitActor>(CurrentActor)->CharacterObj = NewObj;
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

    for (const FSerializedActorData& LoadedData : SaveGameRef->SerializedFreeCharacters)
    {
        UCharacterObj* NewObj = NewObject<UCharacterObj>(this, LoadedData.ActorClass);

        FString IDName;// old actor path (old reference)

        LoadedData.ActorPath.Split(".", nullptr, &IDName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
        NewObj->Rename(*IDName);
        FreeCharacters_Arr.Add(NewObj);

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

    for (int32 i = 0; i < SaveGameRef->SerializedCharacters.Num(); ++i)
    {
        FMemoryReader Reader(SaveGameRef->SerializedCharacters[i].Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Reader, true);
        Ar.ArIsSaveGame = true;

        All_UnitActors[i]->CharacterObj->Serialize(Ar);
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



    for (int32 i = 0; i < FreeCharacters_Arr.Num(); ++i)
    {
        FSerializedActorData& LoadedData = SaveGameRef->SerializedFreeCharacters[i];
        FMemoryReader Reader(LoadedData.Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Reader, true);
        Ar.ArIsSaveGame = true;

        FreeCharacters_Arr[i]->Serialize(Ar);

    }

    {
        // === PASS 2 - DESERIALIZE ===
        FMemoryReader Reader(SaveGameRef->SavedUnitManager.Data, true);
        FObjectAndNameAsStringProxyArchive Ar(Reader, true);
        Ar.ArIsSaveGame = true;

        this->Serialize(Ar);
    }


    int32 finalBreakpoint = 0;

    /////////////////

    /////////////////



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
