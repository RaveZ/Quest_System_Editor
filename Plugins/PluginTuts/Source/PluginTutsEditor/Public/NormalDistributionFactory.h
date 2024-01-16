// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "NormalDistributionFactory.generated.h"

/**
 * 
 */
UCLASS()
class PLUGINTUTSEDITOR_API UNormalDistributionFactory : public UFactory
{
	GENERATED_BODY()
public:
	UNormalDistributionFactory();
	UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);

};
