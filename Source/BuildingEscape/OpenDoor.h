// Copyright Blasthal 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.generated.h"


	// ★
	UENUM()
	enum class EDoorState : uint8
	{
		// Closed,
		Opening,
		// Opened,
		Closing,
	};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()


private:

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Open
	void OpenDoor();

	// Close
	void CloseDoor();

	float TotalMassOfActors() const;

	void FindPressurePlate();

	void FindAudioComponent();

private:
	UPROPERTY(VisibleAnywhere)
	EDoorState DoorState = EDoorState::Closing;

	UPROPERTY(VisibleAnywhere)
	float InitialYaw = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float InitialOpenYaw = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float InitialCloseYaw = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float CurrentYaw = 0.0f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.0f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.0f;

	float TargetYaw = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float ElapsedTime = 0.0f;

	float DoorLastOpened = 0.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.0f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent = nullptr;
};
