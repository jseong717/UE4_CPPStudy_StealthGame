// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "FPSCharacter.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	TriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("LunchTrigger"));
	TriggerComp->SetBoxExtent(FVector(75.0f, 75.0f, 50.0f));
	RootComponent = TriggerComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(TriggerComp);

	LaunchStrength = 1500.0f;
	LaunchPitchAngle = 35.0f;
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OnTriggerBeginOverlap);
}

void AFPSLaunchPad::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	ACharacter* OtherChar = Cast<ACharacter>(OtherActor);
	if (OtherChar)
	{
		OtherChar->LaunchCharacter(LaunchVelocity, true, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActiveLaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActiveLaunchPadEffect, GetActorLocation());
	}
}