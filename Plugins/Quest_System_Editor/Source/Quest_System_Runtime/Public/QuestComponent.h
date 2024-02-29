// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemGraph.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class QUEST_SYSTEM_RUNTIME_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	UPROPERTY(BlueprintReadOnly, Category = "QuestComponent")
		AController* OwningController;

	UPROPERTY(BlueprintReadWrite, Category = "QuestSystemGraph")
		APawn* OwningPawn;

	UPROPERTY(BlueprintReadWrite, Category = "QuestSystemGraph")
		UActorComponent* OwningComponent;

	UFUNCTION(BlueprintCallable, Category = "QuestComponent")
		bool StartQuest(UQuestSystemGraph* QuestAsset);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
