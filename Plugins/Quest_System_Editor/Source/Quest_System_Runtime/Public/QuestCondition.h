// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestCondition.generated.h"

class UQuestSystemGraph;

UCLASS(Blueprintable, EditInlineNew, HideDropdown)
class QUEST_SYSTEM_RUNTIME_API UQuestCondition : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "QuestCondition")
		UQuestSystemGraph* QuestGraph;

	UPROPERTY(BlueprintReadOnly, Category = "QuestCondition")
		UQuestSystem* QuestSystem;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuestCondition")
		bool IsConditionMet() const;

		virtual bool IsConditionMet_Implementation() const;


	UFUNCTION(BlueprintCallable, Category = "QuestCondition")
		AController* GetOwningController() const;

	UFUNCTION(BlueprintCallable, Category = "QuestCondition")
		APawn* GetOwningPawn() const;

	UFUNCTION(BlueprintCallable, Category = "QuestCondition")
		UQuestComponent* GetQuestComponent() const;

	UFUNCTION(BlueprintCallable, Category = "QuestCondition")
		UQuestSystemGraph* GetOwningQuestGraph() const;

	UFUNCTION(BlueprintCallable, Category = "QuestCondition")
		UQuestSystem* GetOwningQuestSystem() const;
	
};
