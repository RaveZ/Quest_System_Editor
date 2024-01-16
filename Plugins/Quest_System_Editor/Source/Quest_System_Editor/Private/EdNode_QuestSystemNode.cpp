// Fill out your copyright notice in the Description page of Project Settings.


#include "EdNode_QuestSystemNode.h"

UEdNode_QuestSystemNode::UEdNode_QuestSystemNode()
{
	bCanRenameNode = true;
}

UEdNode_QuestSystemNode::~UEdNode_QuestSystemNode()
{
}

void UEdNode_QuestSystemNode::SetQuestSystemGraphNode(UQuestSystemGraphNode* InNode)
{
	QuestSystemGraphNode = InNode;
}

UEdGraph_QuestSystemGraph* UEdNode_QuestSystemNode::GetQuestSystemGraphEdGraph()
{
	return Cast<UEdGraph_QuestSystemGraph>(GetGraph());
}

void UEdNode_QuestSystemNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

FText UEdNode_QuestSystemNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (QuestSystemGraphNode == nullptr)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return QuestSystemGraphNode->GetNodeTitle();
	}
}

void UEdNode_QuestSystemNode::PrepareForCopying()
{
	QuestSystemGraphNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_QuestSystemNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != nullptr) {
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

FLinearColor UEdNode_QuestSystemNode::GetBackgroundColor() const
{
	return QuestSystemGraphNode == nullptr ? FLinearColor::Black : QuestSystemGraphNode->GetBackgroundColor();
}

UEdGraphPin* UEdNode_QuestSystemNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_QuestSystemNode::GetOutputPin() const
{
	return Pins[1];
}

void UEdNode_QuestSystemNode::PostEditUndo()
{
	UEdGraphNode::PostEditUndo();
}
