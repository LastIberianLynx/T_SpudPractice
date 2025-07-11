#include "MyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "MyDefaultPawn.h" // Replace with your pawn's header
#include "UnitManager.h"
#include "MyRegionActor.h"
#include "MyUnitActor.h"

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind the "SpawnUnit" action to the F key
    InputComponent->BindAction("SpawnUnit", IE_Pressed, this, &AMyPlayerController::OnSpawnUnit);

    UnitManager = Cast<AUnitManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AUnitManager::StaticClass())
        );
}

void AMyPlayerController::OnSpawnUnit()
{
    if (APawn* ControlledPawn = GetPawn())
    {
        // Cast to your specific pawn class
        if (AMyDefaultPawn* DefaultPawn = Cast<AMyDefaultPawn>(ControlledPawn))
        {

            FVector HitLocation = FVector::ZeroVector;
            FHitResult HitResult;

            // Perform a trace under the mouse cursor against the world static objects (e.g., terrain)
            bool bHit = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
            if (bHit)
            {


                HitLocation = HitResult.Location;
                FTransform SpawnTransform;

                AMyRegionActor* Region = UnitManager->GetClosestRegion(HitLocation);

                SpawnTransform.SetLocation(Region->GetActorLocation());
                SpawnTransform.SetRotation(FQuat::Identity);
                SpawnTransform.SetScale3D(FVector(1.f));

                float DefaultHP = 100.f; // Example HP


                AMyUnitActor* Unit = DefaultPawn->SpawnUnit(SpawnTransform, DefaultHP);
                Unit->CurrentRegion = Region;
                Region->CurrentUnit = Unit;
            }
        }
    }
}
