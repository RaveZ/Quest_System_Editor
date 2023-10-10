// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestEditorFactory.h"

#define LOCTEXT_NAMESPACE "Quest_Editor"
UQuestEditorFactory::UQuestEditorFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	//SupportedClass = UMyObject::StaticClass();
}

UQuestEditorFactory::~UQuestEditorFactory()
{
}


//UObject* UQuestEditorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
//{
//	UMyObject* NewObjectAsset = ConstructObject(Class, InParent, Name, Flags | RF_Transactional);
//	return NewObjectAsset;
//}

#undef LOCTEXT_NAMESPACE