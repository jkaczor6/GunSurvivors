// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivorsGameMode.h"

AGunSurvivorsGameMode::AGunSurvivorsGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGunSurvivorsGameMode::AddScore(int ScoreToAdd)
{
	Score += ScoreToAdd;
	ScoreChangeDelegate.Broadcast(Score);
}

void AGunSurvivorsGameMode::OnRestartGameTimerTimeout()
{
	GetWorldTimerManager().SetTimer(RestartGameTimer, this, &AGunSurvivorsGameMode::RestartGame, 1.0f, false, TimeBeforeRestart);
}

void AGunSurvivorsGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MAP_Main"));
}