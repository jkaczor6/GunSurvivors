// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivorsGameMode.h"

void AGunSurvivorsGameMode::AddScore(int ScoreToAdd)
{
	Score += ScoreToAdd;
	UE_LOG(LogTemp, Display, TEXT("Score: %d"), Score)
}
