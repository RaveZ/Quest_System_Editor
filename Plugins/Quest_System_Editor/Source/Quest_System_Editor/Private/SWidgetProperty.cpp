// Fill out your copyright notice in the Description page of Project Settings.


#include "SWidgetProperty.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SWidgetProperty::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(STextBlock)
            .Text(FText::FromString(TEXT("Your Property Editor:")))
        ]
    + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SEditableText)
            .Text_Lambda([this]() -> FText { return FText::FromString(YourPropertyText.ToString()); })
        .OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
            {
                YourPropertyText = NewText;
                // Update the property value or perform any other action here
            })
        ]
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
