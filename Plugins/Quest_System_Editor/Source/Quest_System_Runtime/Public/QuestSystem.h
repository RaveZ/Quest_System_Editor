// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystem.generated.h"

class UQuestSystemGraphNode;
class UQuestSystemGraphEdge;
class UQuestSystemGraphNode_Root;

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	E_Active		UMETA(DisplayName = "ACTIVE"),
	E_Complete		UMETA(DisplayName = "COMPLETE"),
	E_Fail			UMETA(DisplayName = "FAIL"),
	E_Inactive		UMETA(DisplayName = "INACTIVE"),
};

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "QuestAssets")
		UQuestSystemGraph* QuestGraph;

	/** Quest ID */
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "QuestSystem")
		FName QuestID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QuestSystem")
		FText Description;

		
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestSystem")
		EQuestState QuestState = EQuestState::E_Active;
	
	UPROPERTY(BlueprintReadWrite, Category = "QuestSystem")
		UQuestSystemGraphNode* CurrentNode;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		TArray<UQuestSystemGraphNode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		TArray<UQuestSystemGraphNode*> AllNodes;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystem")
		TMap<FName, UQuestSystemGraphNode*> NodeMap;


	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		void Print(bool ToConsole = true, bool ToScreen = true);


	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		int GetLevelNum() const;

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		void GetNodesByLevel(int Level, TArray<UQuestSystemGraphNode*>& Nodes);

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		AController* GetOwningController();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		APawn* GetOwningPawn();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		UQuestComponent* GetQuestComponent();

	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
		UQuestSystemGraph* GetOwningQuestGraph() const;
	void ClearGraph();


	UFUNCTION(CallInEditor, BlueprintCallable, Category = "QuestSystem")
	static TArray<FName> GetAllQuestID() 
	{
		TArray<FName> QuestIDs;
		for (TObjectIterator<UQuestSystem> It; It; ++It)
		{
			if (UQuestSystem* QuestSystem = Cast<UQuestSystem>(*It))
			{
				QuestIDs.Add(QuestSystem->QuestID);
			}
		}

		for (int i = 0; i < 100; i++)
		{
			QuestIDs.Add(FName("Test"));
		}
		return QuestIDs;
	}

	

};

USTRUCT(BlueprintType)
struct QUEST_SYSTEM_RUNTIME_API FQuestIDHandle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestIDHandle, meta = (GetOptions = "Quest_System_Runtime.QuestSystem.GetAllQuestID"))
		FName QuestID;
	
	
};