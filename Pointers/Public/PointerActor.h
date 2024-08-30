// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PointerActor.generated.h"

UENUM()
enum EActionTaken
{
	SharedRefCreated UMETA(DisplayName = "SharedRefCreated"),
	SharedRefRemoved UMETA(DisplayName = "SharedRefRemoved"),
	WeakRefCreated UMETA(DisplayName = "WeakRefCreated"),
	WeakRefRemoved UMETA(DisplayName = "WeakRefRemoved"),
	UniqueRefCreated UMETA(DisplayName = "UniqueRefCreated"),
	UniqueRefRemoved UMETA(DisplayName = "UniqueRefRemoved")
};

UCLASS()
class POINTERS_API APointerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APointerActor();

	UPROPERTY()
	class UStaticMeshComponent* SphereComp;

protected:
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* TriggerPointer;

	UPROPERTY(EditAnywhere)
	bool SharedPointerDemo = false;

	UPROPERTY(EditAnywhere)
	bool WeakPointerDemo = false;

	UPROPERTY(EditAnywhere)
	bool UniquePointerDemo = false;

	TSharedPtr<class Referencer> ShPtrRef;

	TSharedPtr<Referencer> ShPtrRef2;

	TSharedPtr<Referencer> ShPtrRef3;

	TWeakPtr<Referencer> WeakPtrRef;

	TUniquePtr<Referencer> UniquePtrRef;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OneKeyPressed();

	UFUNCTION()
	void TwoKeyPressed();

	UFUNCTION()
	void ThreeKeyPressed();

	//Widget variables and functions
	class AWidgetContainer* WidgetRef;

	void GetWidgetRef();
};

class POINTERS_API Referencer
{

public:

	APointerActor* PointerActor;

	Referencer();

	~Referencer();

	void Instantiated(APointerActor* PointActor);

private:

};