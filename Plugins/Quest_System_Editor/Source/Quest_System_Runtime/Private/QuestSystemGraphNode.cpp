// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode.h"
#include "QuestSystem.h"
#include "QuestSystemGraph.h"

#define LOCTEXT_NAMESPACE "QuestSystemGraphNode"

UQuestSystemGraphNode::UQuestSystemGraphNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UQuestSystemGraph::StaticClass();

	BackgroundColor = FLinearColor::Black;
#endif
}

UQuestSystemGraphNode::~UQuestSystemGraphNode()
{
}

UQuestSystemGraphEdge* UQuestSystemGraphNode::GetEdge(UQuestSystemGraphNode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}






FText UQuestSystemGraphNode::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "Quest Graph Node");
}

FString UQuestSystemGraphNode::GetNodeName() const
{
	return NodeName.Len() ? NodeName : UQuestSystemGraphNode::GetShortTypeName(this);
}

FString UQuestSystemGraphNode::GetShortTypeName(const UObject* Ob)
{
	if ((Ob == nullptr) || (Ob->GetClass() == nullptr))
	{
		return TEXT("None");
	}

	if (Ob->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
	{
		return Ob->GetClass()->GetName().LeftChop(2);
	}

	FString TypeDesc = Ob->GetClass()->GetName();
	const int32 ShortNameIdx = TypeDesc.Find(TEXT("_"), ESearchCase::CaseSensitive);
	if (ShortNameIdx != INDEX_NONE)
	{
		TypeDesc.MidInline(ShortNameIdx + 1, MAX_int32, false);
	}

	return TypeDesc;
}

#if WITH_EDITOR
bool UQuestSystemGraphNode::IsNameEditable() const
{
	return true;
}

FLinearColor UQuestSystemGraphNode::GetBackgroundColor() const
{
	return BackgroundColor;
}

FText UQuestSystemGraphNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? GetDescription() : NodeTitle;
}

void UQuestSystemGraphNode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

bool UQuestSystemGraphNode::CanCreateConnection(UQuestSystemGraphNode* Other, FText& ErrorMessage)
{
	return true;
}

bool UQuestSystemGraphNode::CanCreateConnectionTo(UQuestSystemGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{
	if (ChildrenLimitType == ENodeLimits::Limited && NumberOfChildrenNodes >= ChildrenLimit)
	{
		ErrorMessage = FText::FromString("Children limit exceeded");
		return false;
	}

	return CanCreateConnection(Other, ErrorMessage);
	
}

bool UQuestSystemGraphNode::CanCreateConnectionFrom(UQuestSystemGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage)
{
	if (ParentLimitType == ENodeLimits::Limited && NumberOfParentNodes >= ParentLimit)
	{
		ErrorMessage = FText::FromString("Parent limit exceeded");
		return false;
	}

	return true;
}

#endif

bool UQuestSystemGraphNode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}

UQuestSystemGraph* UQuestSystemGraphNode::GetGraph() const
{
	return Graph;
}

void UQuestSystemGraphNode::GoToNextNode() 
{
	if (Graph == NULL)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Current node name: %s"), *QuestSystem->CurrentNode->NodeTitle.ToString());

	
	if (ChildrenNodes.Num() > 0) 
	{
		for (UQuestSystemGraphNode* Node : ChildrenNodes)
		{
			//TODO: Make thema able to detect the children node
			QuestSystem->CurrentNode = Node;
		}
		QuestSystem->CurrentNode->GoToNextNode();
	}
	
}

#undef LOCTEXT_NAMESPACE


