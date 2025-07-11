// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRegionActor.h"

// Sets default values
AMyRegionActor::AMyRegionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyRegionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyRegionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

