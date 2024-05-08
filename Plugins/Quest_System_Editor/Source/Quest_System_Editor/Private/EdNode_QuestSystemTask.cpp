// Fill out your copyright notice in the Description page of Project Settings.


#include "EdNode_QuestSystemTask.h"
#include "QuestSystemGraphNode.h"

FText UEdNode_QuestSystemTask::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	const UQuestSystemGraphNode* MyNode = Cast<UQuestSystemGraphNode>(QuestSystemGraphNode);

	if (MyNode != NULL)
	{
		return MyNode->GetNodeTitle();
	}
	else if (!ClassData.GetClassName().IsEmpty())
	{
		FString StoredClassName = ClassData.GetClassName();
		StoredClassName.RemoveFromEnd(TEXT("_C"));

		return FText::Format(NSLOCTEXT("QuestGraph", "NodeClassError", "Class {0} not found, make sure it's saved!"), FText::FromString(StoredClassName));
	}

	return Super::GetNodeTitle(TitleType);
}
