// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointerActor.h"

#include "WidgetContainer.generated.h"

UCLASS()
class POINTERS_API AWidgetContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AWidgetContainer();

	class UUserWidget* KeyWidget;

	class UTextBlock* ActionTextRef;

	void UpdateWidget(EActionTaken ActionTaken);

protected:
	
	virtual void BeginPlay() override;

private:	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>  KeyWidgetRef;

	void ClearWidgetText();

	FTimerHandle ClearTextTimer;
};
