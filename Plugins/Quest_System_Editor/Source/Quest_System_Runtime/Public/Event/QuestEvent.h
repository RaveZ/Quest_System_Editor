// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestEvent.generated.h"

class UQuestSystemGraph;
class UQuestSystemGraphNode;
class UQuestSystem;

UENUM(BlueprintType)
enum class EEventRunType : uint8
{
	/** will trigger at the beggining of the node */
	E_Start			UMETA(DisplayName = "START"),
	/** will trigger if next node conditions met || at the end of state node */
	E_End			UMETA(DisplayName = "END"),
	/** will trigger twice at start and end of node */
	E_Both			UMETA(DisplayName = "BOTH"),
};

UCLASS(Blueprintable, EditInlineNew, HideDropdown)
class QUEST_SYSTEM_RUNTIME_API UQuestEvent : public UObject
{
	GENERATED_BODY()
public:
	UQuestEvent();

	UPROPERTY(BlueprintReadOnly, Category = "QuestEvent")
		UQuestSystemGraph* QuestGraph;
		
	UPROPERTY(BlueprintReadOnly, Category = "QuestEvent")
		bool bUseQuestID = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestEvent", meta = (GetOptions = "GetAllQuestID", EditCondition = "bUseQuestID == true", HideEditConditionToggle, EditConditionHides))
		FName QuestID;

	UPROPERTY(BlueprintReadOnly, Category = "QuestEvent")
		UQuestSystem* QuestSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestEvent")
		EEventRunType EventRunType = EEventRunType::E_Start;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuestEvent")
		void BeginEvent();

	virtual void BeginEvent_Implementation();

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

	UFUNCTION(CallInEditor)
	TArray<FName> GetAllQuestID();

	

};
