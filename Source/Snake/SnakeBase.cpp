// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 10.f;
	LastMoveDirection = EMovementDirection::UP;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	AddSnakeElement(4);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; i++)
	{
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElem->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		SnakeElements.Add(NewSnakeElem);
	}
	
}

void ASnakeBase::Move(float DeltaTime)
{
	FVector MovementVector(ForceInitToZero);
	float MovementSpeedDelta = MovementSpeed * DeltaTime;
	switch (LastMoveDirection)
	{
		case EMovementDirection::UP:
			MovementVector.X += MovementSpeedDelta;
			break;
		case EMovementDirection::DOWN:
			MovementVector.X -= MovementSpeedDelta;
			break;
		case EMovementDirection::LEFT:
			MovementVector.Y += MovementSpeedDelta;
			break;
		case EMovementDirection::RIGHT:
			MovementVector.Y -= MovementSpeedDelta;
			break;
	}

	AddActorWorldOffset(MovementVector);
}


