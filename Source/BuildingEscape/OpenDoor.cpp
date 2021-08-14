// Copyright Blasthal 2021


#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	InitialOpenYaw = InitialYaw;
	InitialCloseYaw = InitialYaw;
	TargetYaw = InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (PressurePlate)
	{
		if (TotalMassOfActors() > MassToOpenDoors)
		{
			OpenDoor();
			DoorLastOpened = GetWorld()->GetTimeSeconds();
		}
		else
		{
			float ElapsedTimeFromOpened = GetWorld()->GetTimeSeconds() - DoorLastOpened;
			if (DoorCloseDelay < ElapsedTimeFromOpened)
			{
				CloseDoor();
			}
		}
	}


	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	// 時間経過
	ElapsedTime += DeltaTime;
	// UE_LOG(LogTemp, Warning, TEXT("ElapsedTime is: %f"), ElapsedTime);
	
	// 回転値補間計算
	float Alpha = 0.0f;
	switch (DoorState)
	{
		case EDoorState::Opening:
		{
			Alpha = ElapsedTime * DoorOpenSpeed;
			Alpha = FMath::Min(1.0f, Alpha);
			CurrentYaw = FMath::Lerp(InitialOpenYaw, TargetYaw, Alpha);
			break;
		}
		case EDoorState::Closing:
		{
			Alpha = ElapsedTime * DoorCloseSpeed;
			Alpha = FMath::Min(1.0f, Alpha);
			CurrentYaw = FMath::Lerp(InitialCloseYaw, TargetYaw, Alpha);
			break;
		}
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("Alpha is: %f"), Alpha);

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = CurrentYaw;

	// 回転値反映
	GetOwner()->SetActorRotation(CurrentRotation);
}


// Open
void UOpenDoor::OpenDoor()
{
	if (DoorState == EDoorState::Opening)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Call %s"), __FUNCTIONW__);

	DoorState = EDoorState::Opening;

	InitialOpenYaw = CurrentYaw;
	TargetYaw = InitialYaw + OpenAngle;
	ElapsedTime = 0.0f;


	// Play sound
	if (AudioComponent)
	{
		AudioComponent->Play();
	}
}


// Close
void UOpenDoor::CloseDoor()
{
	if (DoorState == EDoorState::Closing)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Call %s"), __FUNCTIONW__);

	DoorState = EDoorState::Closing;

	InitialCloseYaw = CurrentYaw;
	TargetYaw = InitialYaw;
	ElapsedTime = 0.0f;

	
	// Play sound
	if (AudioComponent)
	{
		AudioComponent->Play();
	}
}


float UOpenDoor::TotalMassOfActors() const
{
	if (!PressurePlate)
	{
		return 0.0f;
	}


	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		UPrimitiveComponent* PrimitiveComponent = Actor->FindComponentByClass<UPrimitiveComponent>();
		if (PrimitiveComponent)
		{
			TotalMass += PrimitiveComponent->GetMass();
		}
	}


	DrawDebugString(
		GetWorld(),
		GetOwner()->GetActorLocation(),
		// TEXT("TotalMass"),
		FString::Format(TEXT("TotalMass: {0}"), { TotalMass }),
		nullptr,
		FColor::White,
		0.0f,
		false,
		1.0f
	);


	return TotalMass;
}


void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the UOpenDoor component on it. but no PressurePlate set!"),
			*GetOwner()->GetName()
		);
	}
}


void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Audio component!"), *GetOwner()->GetName());
	}
}
