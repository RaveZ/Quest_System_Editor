#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FQuest_System_RuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};