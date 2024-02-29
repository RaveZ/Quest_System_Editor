// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UQuestComponent::StartQuest(UQuestSystemGraph* QuestAsset)
{
	if (QuestAsset == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("RunQuest: Unable to run NULL Quest Graph"));
		return false;
	}

	//Set Dependency
	OwningController = Cast<AController>(GetOwner());
	OwningPawn = OwningController->GetPawn();
	OwningComponent = this;

	//Set Quest Asset Dependency
	QuestAsset->OwningController = OwningController;
	QuestAsset->OwningPawn = OwningPawn;
	QuestAsset->OwningComponent = OwningComponent;


	bool bSuccess = true;
	for (UQuestSystem* Quest : QuestAsset->QuestSystems)
	{
		for (UQuestSystemGraphNode* Node : Quest->RootNodes)
		{
			//TODO : pengecekan state quest dan state custom untuk menggantikan ENUM QuestState
			if (Quest->QuestState == EQuestState::E_Active)
			{
				Quest->CurrentNode = Node;
				Node->GoToNextNode();
			}
		}
	}

	return bSuccess;
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

