// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// TODO replace all Nullcheck regions with this macro
#if !UE_BUILD_SHIPPING
/**
 * Checks the validity of a UObject pointer, logs a warning if it is invalid to help prevent editor crashes, and then returns.
 * This has no effect in shipping builds.
 */
#define UVALID_LOG_DEBUG(PointerParam)																						\
{																															\
	if (!IsValid(PointerParam))																									\
	{																														\
		UE_LOG(LogTemp, Warning, TEXT("%s|%s is nullptr#L%d"), TEXT(__FUNCTION__), TEXT(#PointerParam), __LINE__)		    \
		return;																												\
	}																														\
}
#define UVALID_LOG_DEBUGF(PointerParam)																						\
{																															\
	if (!IsValid(PointerParam))																									\
	{																														\
		UE_LOG(LogTemp, Warning, TEXT("%s|%s is nullptr#L%d"), TEXT(__FUNCTION__), TEXT(#PointerParam), __LINE__)		    \
		return 0.f;																												\
	}																														\
}
#else
#define UVALID_LOG_DEBUG(PointerParam)  /* No-op in shipping builds */
#define UVALID_LOG_DEBUGF(PointerParam) /* No-op in shipping builds */
#endif