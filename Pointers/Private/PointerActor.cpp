// Fill out your copyright notice in the Description page of Project Settings.


#include "PointerActor.h"

#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetContainer.h"

APointerActor::APointerActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = SphereComp;
	SphereComp->bHiddenInGame = true;

	TriggerPointer = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	TriggerPointer->SetupAttachment(RootComponent);
}

void APointerActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerPointer->OnComponentBeginOverlap.AddDynamic(this, &APointerActor::OnOverlapBegin);
	TriggerPointer->OnComponentEndOverlap.AddDynamic(this, &APointerActor::OnOverlapEnd);

	//Need to enable input before binding keys
	if (GetWorld()->GetFirstPlayerController())
	{
		EnableInput(GetWorld()->GetFirstPlayerController());
	}

	if (InputComponent)
	{
		InputComponent->BindKey(EKeys::One, EInputEvent::IE_Pressed, this, &APointerActor::OneKeyPressed);
		InputComponent->BindKey(EKeys::Two, EInputEvent::IE_Pressed, this, &APointerActor::TwoKeyPressed);
		InputComponent->BindKey(EKeys::Three, EInputEvent::IE_Pressed, this, &APointerActor::ThreeKeyPressed);
	}

	if (GetWorld()->GetFirstPlayerController())
	{
		DisableInput(GetWorld()->GetFirstPlayerController());
	}

	GetWidgetRef();
}

//Resetting all non-garabge collection managed pointers
void APointerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (WeakPtrRef.IsValid())
	{
		WeakPtrRef.Reset();
	}

	if (ShPtrRef3)
	{
		ShPtrRef3.Reset();
	}

	if (ShPtrRef2)
	{
		ShPtrRef2.Reset();
	}

	if (ShPtrRef)
	{
		ShPtrRef.Reset();
	}

	if (UniquePtrRef.IsValid())
	{
		UniquePtrRef.Reset();
	}
}

void APointerActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

void APointerActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetWorld()->GetFirstPlayerController())
	{
		if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			DisableInput(GetWorld()->GetFirstPlayerController());
			GEngine->ClearOnScreenDebugMessages();
		}
	}
}

//Create references
void APointerActor::OneKeyPressed()
{
	if (SharedPointerDemo)
	{
		if (!ShPtrRef)
		{
			ShPtrRef = MakeShared<Referencer>();
			ShPtrRef.Get()->Instantiated(this);

			WidgetRef->UpdateWidget(EActionTaken::SharedRefCreated);
		}

		else if (!ShPtrRef2)
		{
			ShPtrRef2 = ShPtrRef;

			WidgetRef->UpdateWidget(EActionTaken::SharedRefCreated);
		}

		else if (!ShPtrRef3)
		{
			ShPtrRef3 = ShPtrRef;

			WidgetRef->UpdateWidget(EActionTaken::SharedRefCreated);
		}

		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue,
			FString::Printf(TEXT("Reference Count: %i"), ShPtrRef.GetSharedReferenceCount()));
	}

	if (UniquePointerDemo)
	{
		UniquePtrRef = MakeUnique<Referencer>();
		UniquePtrRef.Get()->Instantiated(this);

		WidgetRef->UpdateWidget(EActionTaken::UniqueRefCreated);

		GEngine->AddOnScreenDebugMessage(3, 60.f, FColor::Red,
			FString::Printf(TEXT("Reference Count: %s"), UniquePtrRef.IsValid() ? TEXT("true") : TEXT("false")));
	}
}

//Remove references
void APointerActor::TwoKeyPressed()
{
	if (SharedPointerDemo)
	{
		if (ShPtrRef3)
		{
			ShPtrRef3.Reset();

			WidgetRef->UpdateWidget(EActionTaken::SharedRefRemoved);
		}

		else if (ShPtrRef2)
		{
			ShPtrRef2.Reset();

			WidgetRef->UpdateWidget(EActionTaken::SharedRefRemoved);
		}

		else if (ShPtrRef)
		{
			ShPtrRef.Reset();

			WidgetRef->UpdateWidget(EActionTaken::SharedRefRemoved);
		}

		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue,
			FString::Printf(TEXT("Reference Count: %i"), ShPtrRef.GetSharedReferenceCount()));

		if (WeakPointerDemo)
		{
			if (WeakPtrRef.IsValid())
			{
				GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Green,
					FString::Printf(TEXT("Weak pointer valid")));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Green,
					FString::Printf(TEXT("Weak pointer invalid")));
			}
		}
	}

	if (UniquePointerDemo)
	{
		if (UniquePtrRef.IsValid())
		{
			UniquePtrRef.Reset();

			WidgetRef->UpdateWidget(EActionTaken::UniqueRefRemoved);

			GEngine->AddOnScreenDebugMessage(3, 60.f, FColor::Red,
				FString::Printf(TEXT("Reference Count: %s"), UniquePtrRef.IsValid() ? TEXT("true") : TEXT("false")));
		}
	}
}

//Create weak references
void APointerActor::ThreeKeyPressed()
{
	if (SharedPointerDemo && WeakPointerDemo)
	{
		if (ShPtrRef)
		{
			WeakPtrRef = ShPtrRef;
			WidgetRef->UpdateWidget(EActionTaken::WeakRefCreated);

			if (WeakPtrRef.IsValid())
			{
				GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Green,
					FString::Printf(TEXT("Weak pointer valid")));

			}
			else
			{
				GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Green,
					FString::Printf(TEXT("Weak pointer invalid")));
			}
		}

		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue,
			FString::Printf(TEXT("Reference Count: %i"), ShPtrRef.GetSharedReferenceCount()));
	}
}

//Get reference to widget
void APointerActor::GetWidgetRef()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, AWidgetContainer::StaticClass(), OutActors);

	if (OutActors.Num() == 1)
	{
		WidgetRef = Cast<AWidgetContainer>(OutActors[0]);
		if (!WidgetRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor cast to widget container failed."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget containers in level does not equal 1"));
	}
}

//Referencer class definitions
Referencer::Referencer()
{

}

Referencer::~Referencer()
{
	PointerActor->SphereComp->SetHiddenInGame(true);
}

void Referencer::Instantiated(APointerActor* PointActor)
{
	PointerActor = PointActor;
	PointerActor->SphereComp->SetHiddenInGame(false);
}
