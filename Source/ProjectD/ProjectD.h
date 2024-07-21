// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#define LOG_SUBREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetRemoteRole()))
#define LOG_SUBLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetLocalRole()))


#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))


#define PD_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s] [%s / %s] %s %s"),LOG_NETMODEINFO,LOG_LOCALROLEINFO,LOG_REMOTEROLEINFO, CUR_LINE, *FString::Printf(Format,##__VA_ARGS__))

//GetAvatarActor
#define PD_SUBLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s] [%s / %s] %s %s"),LOG_NETMODEINFO,LOG_SUBLOCALROLEINFO, LOG_SUBREMOTEROLEINFO, CUR_LINE, *FString::Printf(Format,##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(PDLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PDNetLog, Log, All);