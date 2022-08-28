// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
// #include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>( TEXT( "Root" ) );
	SetRootComponent( Root );
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "Mesh" ) );
	Mesh->SetupAttachment( Root );

}

// Called when the game starts or when spawned
void AGun::BeginPlay( )
{
	Super::BeginPlay( );

}

// Called every frame
void AGun::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGun::PullTrigger( )
{
	UGameplayStatics::SpawnEmitterAttached( MuzzleFlash, Mesh, TEXT( "MuzzleFlashSocket" ) );

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace( Hit, ShotDirection );

	if ( bSuccess )
	{
		//DrawDebugPoint( GetWorld( ), Hit.Location, 20, FColor::Red, true );
		
		UGameplayStatics::SpawnEmitterAtLocation( GetWorld( ), ImpactEffect, Hit.Location, ShotDirection.Rotation( ) );
		
		AActor* HitActor = Hit.GetActor( );
		if ( HitActor != nullptr )
		{
			FPointDamageEvent DamageEvent( Damage, Hit, ShotDirection, nullptr );
			AController *OwnerController = GetOwnerController( );
			HitActor->TakeDamage( Damage, DamageEvent, OwnerController, this );
		}
	}

}

bool AGun::GunTrace( FHitResult& Hit, FVector& ShotDirection )
{
	AController *OwnerController = GetOwnerController( );

	if ( OwnerController == nullptr ) return false;

	FVector StartLocation;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint( StartLocation, Rotation );
	ShotDirection = -Rotation.Vector( );

	FVector End = StartLocation + Rotation.Vector( ) * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor( this );
	 
	return GetWorld( )->LineTraceSingleByChannel( Hit, StartLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params );
}

AController* AGun::GetOwnerController( ) const
{
	APawn *OwnerPawn = Cast<APawn>( GetOwner( ) );
	if ( OwnerPawn == nullptr ) return nullptr;
	
	return OwnerPawn->GetController( );
}

