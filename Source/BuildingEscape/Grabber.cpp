// Copyright Blasthal 2021


#include "Grabber.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}


void UGrabber::FindPhysicsHandle()
{
	// Checking for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics handle component found on %s."), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle component not found on %s. Add physics handle component."), *GetOwner()->GetName());
	}
}


void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
}


void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = HitResult.GetActor();

	// if (HitResult.bBlockingHit)
	if (ActorHit)
	{
		if (PhysicsHandle)
		{
			UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
			PhysicsHandle->GrabComponentAtLocation(
				ComponentToGrab,
				NAME_None,
				GetPlayersReach()
			);
		}
	}
}


FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	
	// Ray cast
	const FVector LineTraceStart = PlayerViewPointRotation.Vector() * StartDistance + PlayerViewPointLocation;
	const FVector LineTraceEnd = PlayerViewPointRotation.Vector() * Reach + LineTraceStart;

	return LineTraceEnd;
}


void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released."));

	if (!PhysicsHandle) { return; }

	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Ray cast
	FHitResult HitResult;
	const FCollisionQueryParams QueryParams = { FName(TEXT("")), false, GetOwner() };

	const bool bIsBlocking = GetWorld()->LineTraceSingleByObjectType(
		/* OutHit = */ OUT HitResult,
		/* Start = */ GetPlayersWorldPos(),
		/* End = */ GetPlayersReach(),
		/* ObjectQueryParams = */ FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		/* Params = */ QueryParams
	);

	// Debug
	// if (bIsBlocking)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s.\n%s"),
	// 		HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("None"),
	// 		*HitResult.ToString()
	// 	);
	// 	GEngine->AddOnScreenDebugMessage(
	// 		INDEX_NONE,
	// 		2.0f,
	// 		FColor::Yellow,
	// 		FString::Format(TEXT("Line trace has hit: {0}.\n{1}"), { HitResult.GetActor() ? HitResult.GetActor()->GetName() : TEXT("None"), HitResult.ToString() }),
	// 		true,
	// 		FVector2D::UnitVector
	// 	);
	// }

	// // Draw debug line
	// DrawDebugLine(
	// 	GetWorld(),
	// 	/* LineTraceStart = */ GetPlayersWorldPos(),
	// 	/* LineEnTraced = */ GetPlayersReach(),
	// 	/* Color = */ bIsBlocking ? FColor::Red : FColor::Yellow,
	// 	/* LineTracebPersistentLines = */ false,
	// 	/* LifeTime = */ 0.1f,
	// 	/* DepthPriotity = */ 0,
	// 	/* Tichkness = */ 1.0f
	// );


	return HitResult;
}


FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	const FVector LineTraceStart = PlayerViewPointRotation.Vector() * StartDistance + PlayerViewPointLocation;

	return LineTraceStart;
}

