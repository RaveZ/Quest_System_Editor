// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting_QuestEditor.h"

UQuestEditorSettings::UQuestEditorSettings()
{
	AutoLayoutStrategy = EAutoLayoutType::Tree;

	bFirstPassOnly = false;

	bRandomInit = false;

	OptimalDistance = 100.f;

	MaxIteration = 50;

	InitTemperature = 10.f;

	CoolDownRate = 10.f;
}

UQuestEditorSettings::~UQuestEditorSettings()
{
}
