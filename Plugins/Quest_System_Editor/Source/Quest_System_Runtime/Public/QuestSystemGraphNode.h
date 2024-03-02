// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGraphTypes.h"
#include "UObject/NoExportTypes.h"
#include "QuestSystemGraphNode.generated.h"

class UQuestSystemGraph;
class UQuestSystem;
class UQuestSystemGraphEdge;

UENUM(BlueprintType)
enum class ENodeLimits : uint8
{
	Unlimited,
	Limited
};

UCLASS(Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestSystemGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UQuestSystemGraphNode();
	virtual ~UQuestSystemGraphNode();


	UPROPERTY(VisibleDefaultsOnly, Category = "QuestSystemGraphNode")
		UQuestSystemGraph* QuestGraph;

	UPROPERTY(VisibleDefaultsOnly, Category = "QuestSystemGraphNode")
		UQuestSystem* QuestSystem;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystemGraphNode")
		TArray<UQuestSystemGraphNode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystemGraphNode")
		TArray<UQuestSystemGraphNode*> ChildrenNodes;

	UPROPERTY(BlueprintReadOnly, Category = "QuestSystemGraphNode")
		TMap<UQuestSystemGraphNode*, UQuestSystemGraphEdge*> Edges;

	/** node name */
	UPROPERTY(Category=Description, EditAnywhere)
		FString NodeName;


	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraphNode")
		virtual UQuestSystemGraphEdge* GetEdge(UQuestSystemGraphNode* ChildNode);

	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraphNode")
		bool IsLeafNode() const;


	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraphNode")
		AController* GetOwningController();

	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraphNode")
		APawn* GetOwningPawn();

	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraphNode")
		UActorComponent* GetOwningComponent();

	UFUNCTION(BlueprintCallable, Category = "QuestSystemGraphNode")
		UQuestSystemGraph* GetOwningQuest() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuestSystemGraphNode")
		FText GetDescription() const;


	virtual FText GetDescription_Implementation() const;

	/** @return name of node */
	FString GetNodeName() const;

	/** returns short name of object's class (BTTaskNode_Wait -> Wait) */
	static FString GetShortTypeName(const UObject* Ob);



	//////////////////////////////////////////////////////////////////////////
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Editor")
		FText NodeTitle;

	UPROPERTY(VisibleDefaultsOnly, Category = "QuestSystemGraphNode_Editor")
		TSubclassOf<UQuestSystemGraph> CompatibleGraphType;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Editor")
		FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Editor")
		FText ContextMenuName;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Editor")
		ENodeLimits ParentLimitType;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Editor", meta = (ClampMin = "0", EditCondition = "ParentLimitType == ENodeLimit::Limited", EditConditionHides))
		int32 ParentLimit;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Editor")
		ENodeLimits ChildrenLimitType;

	UPROPERTY(EditDefaultsOnly, Category = "QuestSystemGraphNode_Editor", meta = (ClampMin = "0", EditCondition = "ChildrenLimitType == ENodeLimit::Limited", EditConditionHides))
		int32 ChildrenLimit;

#endif

#if WITH_EDITOR
	virtual bool IsNameEditable() const;

	virtual FLinearColor GetBackgroundColor() const;

	virtual FText GetNodeTitle() const;

	virtual void SetNodeTitle(const FText& NewTitle);

	virtual bool CanCreateConnection(UQuestSystemGraphNode* Other, FText& ErrorMessage);

	virtual bool CanCreateConnectionTo(UQuestSystemGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage);
	virtual bool CanCreateConnectionFrom(UQuestSystemGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage);
#endif
};
