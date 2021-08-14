// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	// Print object's name
	FString ObjectName = Owner->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Object name is: %s"), *ObjectName);

	// Print object's location
	FVector ObjectPosition = Owner->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Object location is: %s"), *ObjectPosition.ToString());
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

