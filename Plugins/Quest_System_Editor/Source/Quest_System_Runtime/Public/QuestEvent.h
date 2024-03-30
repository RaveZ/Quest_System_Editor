// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestEvent.generated.h"

class UQuestSystemGraph;

UENUM(BlueprintType)
enum class EEventRunType : uint8
{
	E_Start			UMETA(DisplayName = "START"),
	E_End			UMETA(DisplayName = "END"),
	E_Both			UMETA(DisplayName = "BOTH"),
};

UCLASS(Blueprintable, EditInlineNew, HideDropdown)
class QUEST_SYSTEM_RUNTIME_API UQuestEvent : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "QuestEvent")
		UQuestSystemGraph* QuestGraph;
		
	UPROPERTY(BlueprintReadOnly, Category = "QuestEvent")
		UQuestSystem* QuestSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestEvent")
		EEventRunType EventRunType = EEventRunType::E_Start;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestEvent")
		void BeginEvent();

	UFUNCTION(BlueprintCallable, Category = "QuestEvent")
		AController* GetOwningController() const;

	UFUNCTION(BlueprintCallable, Category = "QuestEvent")
		APawn* GetOwningPawn() const;

	UFUNCTION(BlueprintCallable, Category = "QuestEvent")
		UQuestComponent* GetQuestComponent() const;

	UFUNCTION(BlueprintCallable, Category = "QuestEvent")
		UQuestSystemGraph* GetOwningQuestGraph() const;

	UFUNCTION(BlueprintCallable, Category = "QuestEvent")
		UQuestSystem* GetOwningQuestSystem() const;
};
