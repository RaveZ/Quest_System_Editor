// Fill out your copyright notice in the Description page of Project Settings.


#include "SQuestPalette.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SQuestPaletteItem::Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData, TWeakPtr<FAssetEditor_QuestSystem> InQuestEditor)
{
	Construct(InArgs, InCreateData, InQuestEditor.Pin()->GetQuestSystemObj(), InQuestEditor);

}

void SQuestPaletteItem::Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData, UQuestSystemGraph* InQuestSystemGraph)
{
}

void SQuestPaletteItem::Construct(const FArguments& InArgs, FCreateWidgetForActionData* const InCreateData, UQuestSystemGraph* InQuestSystemGraph, TWeakPtr<FAssetEditor_QuestSystem> InQuestEditor)
{
}

void SQuestPaletteItem::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
}

TSharedRef<SWidget> SQuestPaletteItem::CreateTextSlotWidget(FCreateWidgetForActionData* const InCreateData, TAttribute<bool> bIsReadOnly)
{
	return SNew(SBox);
}

FText SQuestPaletteItem::GetDisplayText() const
{
	return FText();
}

bool SQuestPaletteItem::OnNameTextVerifyChanged(const FText& InNewText, FText& OutErrorMessage)
{
	return false;
}

void SQuestPaletteItem::OnNameTextCommitted(const FText& NewText, ETextCommit::Type InTextCommit)
{
}

TSharedPtr<SToolTip> SQuestPaletteItem::ConstructToolTipWidget() const
{
	return TSharedPtr<SToolTip>();
}

FText SQuestPaletteItem::GetToolTipText() const
{
	return FText();
}

void SQuestPalette::Construct(const FArguments& InArgs, TWeakPtr<FAssetEditor_QuestSystem> InQuestEditor)
{
}

void SQuestPalette::OnSplitterResized() const
{
}
