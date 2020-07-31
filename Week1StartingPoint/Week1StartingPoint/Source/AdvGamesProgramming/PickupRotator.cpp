// Jonathan Moallem 2020


#include "PickupRotator.h"
#include "GameFramework/Actor.h"


UPickupRotator::UPickupRotator()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPickupRotator::BeginPlay()
{
	Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorLocation().ToString())
}


void UPickupRotator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TickRotateObject(DeltaTime);
}


void UPickupRotator::TickRotateObject(const float& DeltaTime)
{
    FRotator CurrentActorRotation = GetOwner()->GetActorRotation();
    CurrentActorRotation.Yaw += DeltaTime * RotSpeed;
    GetOwner()->SetActorRotation(CurrentActorRotation);
}