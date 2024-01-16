// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalDistribution.h"
#include "Toolkits/AssetEditorToolkit.h"
/**
 * 
 */
class PLUGINTUTSEDITOR_API FNormalDistributionEditorToolkit : public FAssetEditorToolkit
{
public:
	void InitEditor(const TArray<UObject*>& InObjects);

	void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	FName GetToolkitFName() const override { return "NormalDistributionEditor"; }
	FText GetBaseToolkitName() const override { return INVTEXT("Normal Distribution Editor"); }
	FString GetWorldCentricTabPrefix() const override { return "Normal Distribution "; }
	FLinearColor GetWorldCentricTabColorScale() const override { return {}; }

	float GetMean() const;
	float GetStandardDeviation() const;
	void SetMean(float Mean);
	void SetStandardDeviation(float StandardDeviation);
private:
	UNormalDistribution* NormalDistribution;
};
