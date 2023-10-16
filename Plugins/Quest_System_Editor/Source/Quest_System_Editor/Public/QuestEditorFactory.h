// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraph.h"
#include "Factories/Factory.h"
#include "QuestEditorFactory.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_SYSTEM_EDITOR_API UQuestEditorFactory : public UFactory
{
	GENERATED_BODY()
public:
	UQuestEditorFactory();
	virtual ~UQuestEditorFactory();

	UPROPERTY(EditAnywhere, Category = DataAsset)	
		TSubclassOf<UQuestSystemGraph> QuestSystemGraphClass;

	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
