#pragma once

#include "LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLD, Log, All);

#define LOG_LD(LogLevel, Format, ...)\
	UE_LOG(LogLD, LogLevel, TEXT("%s - %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString::Printf(Format, ##__VA_ARGS__)); 