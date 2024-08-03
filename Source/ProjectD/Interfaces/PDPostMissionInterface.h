// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PDPostMissionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPDPostMissionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTD_API IPDPostMissionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FVector GetPosition() = 0;
};
