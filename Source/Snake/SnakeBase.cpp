// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactable.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 0.5f;
	LastMoveDirection = EMovementDirection::DOWN;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(5);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; i++)
	{
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);
		if (SnakeElements.Num() > ElementsNum)
		{
			auto LastElem = SnakeElements[SnakeElements.Num() - 1];
			FVector LastElemLocation = LastElem->GetActorLocation();
			NewTransform.SetLocation(LastElemLocation);
		}
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		//NewSnakeElem->SetActorHiddenInGame(true);
		NewSnakeElem->SnakeOwner = this;
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0)
		{
			NewSnakeElem->SetFirstElementType();
		}
		//NewSnakeElem->SetActorHiddenInGame(false);
	}	
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);

	switch (LastMoveDirection)
	{
		case EMovementDirection::UP:
			MovementVector.X += ElementSize;
			break;
		case EMovementDirection::DOWN:
			MovementVector.X -= ElementSize;
			break;
		case EMovementDirection::LEFT:
			MovementVector.Y += ElementSize;
			break;
		case EMovementDirection::RIGHT:
			MovementVector.Y -= ElementSize;
			break;
	}

	SnakeElements[0]->ToogleCollision();

	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}

	SnakeElements[0] -> AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToogleCollision();
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedBlock, AActor* Other)
{
	if (IsValid(OverlappedBlock))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedBlock, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(Other);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, bIsFirst);
		}
	}
}


