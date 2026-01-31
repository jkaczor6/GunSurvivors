// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivorsGameMode.h"

void AGunSurvivorsGameMode::AddScore(int ScoreToAdd)
{
	Score += ScoreToAdd;
	ScoreChangeDelegate.Broadcast(Score);
}
