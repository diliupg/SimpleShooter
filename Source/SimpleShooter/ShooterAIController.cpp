// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"


void AShooterAIController::BeginPlay( )
{
	Super::BeginPlay( );

	PlayerPawn = UGameplayStatics::GetPlayerPawn( GetWorld( ), 0 );

	SetFocus( PlayerPawn );
	
}

void AShooterAIController::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	if ( LineOfSightTo(PlayerPawn))
	{
		MoveToActor( PlayerPawn, AcceptanceRadius );
	}
	else
	{
		ClearFocus( EAIFocusPriority::Gameplay );
		StopMovement( );
	}

}

