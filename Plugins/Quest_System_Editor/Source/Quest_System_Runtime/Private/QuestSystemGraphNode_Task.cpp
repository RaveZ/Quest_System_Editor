// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemGraphNode_Task.h"
#include "QuestSystemGraphNode_TaskManager.h"
#include "QuestComponent.h"
#include "QuestSystem.h"
#include "QuestSystemGraphNode.h"

#define LOCTEXT_NAMESPACE "QuestSystemGraphNode_Task"

UQuestSystemGraphNode_Task::UQuestSystemGraphNode_Task()
{
}

bool UQuestSystemGraphNode_Task::CompleteTask()
{
    if (QuestSystem == NULL)
        return false;

    TaskState = ETaskState::E_Complete;
    OnTaskCompleted();
    GetQuestComponent()->OnQuestObjectiveUpdated.Broadcast(QuestSystem);
    
    return true;
}



void UQuestSystemGraphNode_Task::Tick(float DeltaTime)
{
    if (LastFrameNumberWeTicked == GFrameCounter)
        return;
    //Do The Tick Logic
    OnTaskTick();
    LastFrameNumberWeTicked = GFrameCounter;
}

#if WITH_EDITOR
FText UQuestSystemGraphNode_Task::GetNodeTitle() const
{
    return Super::GetNodeTitle();

}

FText UQuestSystemGraphNode_Task::GetNodeDescription() const
{
    return Description;
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