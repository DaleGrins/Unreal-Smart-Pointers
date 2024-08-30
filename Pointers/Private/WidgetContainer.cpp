// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetContainer.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

// Sets default values
AWidgetContainer::AWidgetContainer()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AWidgetContainer::BeginPlay()
{
	Super::BeginPlay();

	if (KeyWidgetRef)
	{
		KeyWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), KeyWidgetRef);
		KeyWidget->AddToViewport();

		UWidget* WidgetTextRef = KeyWidget->WidgetTree->FindWidget(FName("ActionText"));
		if (WidgetTextRef)
		{
			ActionTextRef = Cast<UTextBlock>(WidgetTextRef);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find and retrieve ActionText widget"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Key Widget invalid"));
	}
}

void AWidgetContainer::UpdateWidget(EActionTaken ActionTaken)
{
	GetWorldTimerManager().ClearTimer(ClearTextTimer);

	if (ActionTextRef)
	{
		switch (ActionTaken)
		{
		case SharedRefCreated:
			ActionTextRef->SetText(FText::FromString(FString(TEXT("Shared reference created"))));
			break;
		case SharedRefRemoved:
			ActionTextRef->SetText(FText::FromString(FString(TEXT("Shared reference removed"))));
			break;
		case WeakRefCreated:
			ActionTextRef->SetText(FText::FromString(FString(TEXT("Weak reference created"))));
			break;
		case WeakRefRemoved:
			ActionTextRef->SetText(FText::FromString(FString(TEXT("Weak reference removed"))));
			break;
		case UniqueRefCreated:
			ActionTextRef->SetText(FText::FromString(FString(TEXT("Unique reference created"))));
			break;
		case UniqueRefRemoved:
			ActionTextRef->SetText(FText::FromString(FString(TEXT("Unique reference removed"))));
			break;
		default:
			ActionTextRef->SetText(FText::FromString(FString(TEXT(""))));
			break;
		}
	}

	GetWorldTimerManager().SetTimer(ClearTextTimer, this, &AWidgetContainer::ClearWidgetText, 1.5f, false);
}

void AWidgetContainer::ClearWidgetText()
{
	ActionTextRef->SetText(FText::FromString(FString(TEXT(""))));
}

