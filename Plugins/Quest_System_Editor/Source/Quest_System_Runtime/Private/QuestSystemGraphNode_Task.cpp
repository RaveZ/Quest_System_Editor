// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode_Task.h"

#define LOCTEXT_NAMESPACE "QuestSystemGraphNode_Task"

UQuestSystemGraphNode_Task::UQuestSystemGraphNode_Task()
{
}

#if WITH_EDITOR
FText UQuestSystemGraphNode_Task::GetNodeTitle() const
{
	return Description.IsEmpty() ? LOCTEXT("EmptyParagraph", "(Empty paragraph)") : Description;

}

void UQuestSystemGraphNode_Task::SetNodeTitle(const FText& NewTitle)
{
	Description = NewTitle;
}

FLinearColor UQuestSystemGraphNode_Task::GetBackgroundColor() const
{
    return Super::GetBackgroundColor();
    /*UQuestSystemGraph* Graph = Cast<UQuestSystemGraph>(GetGraph());

    if (Graph == nullptr)
        return Super::GetBackgroundColor();*/

    /*switch (DialoguerPostion)
    {
    case EDialoguerPostion::Left:
        return Graph->LeftDialoguerBgColor;
    case EDialoguerPostion::Right:
        return Graph->RightDialoguerBgColor;
    default:
        return FLinearColor::Black;
    }*/
}
#endif

#undef LOCTEXT_NAMESPACE