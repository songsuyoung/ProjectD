// Fill out your copyright notice in the Description page of Project Settings.


#include "PDGameMode.h"
#include "Game/PDGameState.h"
#include "Character/PDCharacterPlayer.h"
#include "Player/PDPlayerController.h"
#include "Player/PDPlayerState.h"

APDGameMode::APDGameMode()
{
	static ConstructorHelpers::FClassFinder<APDGameState> GameStateClassRef(TEXT("/Game/ProjectD/Blueprint/BPPD_GameState.BPPD_GameState_C"));

	if (GameStateClassRef.Class)
	{
		GameStateClass = GameStateClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APDPlayerController> PlayerControllerClassRef(TEXT("/Game/ProjectD/Blueprint/Player/BPPD_PlayerController.BPPD_PlayerController_C"));

	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APDPlayerState> PlayerStateClassRef(TEXT("/Game/ProjectD/Blueprint/Player/BPPD_PlayerState.BPPD_PlayerState_C"));

	if (PlayerStateClassRef.Class)
	{
		PlayerStateClass = PlayerStateClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APDCharacterPlayer> PawnClassRef(TEXT("/Game/ProjectD/Blueprint/Player/BPPD_Player.BPPD_Player_C"));

	if (PawnClassRef.Class)
	{
		DefaultPawnClass = PawnClassRef.Class;
	}
}
