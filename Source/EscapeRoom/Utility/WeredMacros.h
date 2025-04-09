// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// TODO replace all Nullcheck regions with this macro
#if !UE_BUILD_SHIPPING
#define NULLCHECK_DEBUG(PointerParam)																						\
{																															\
	if (!(PointerParam))																									\
	{																														\
		UE_LOG(LogTemp, Warning, TEXT("%s|%s is nullptr#L%d"), TEXT(__FUNCTION__), TEXT(#PointerParam), __LINE__)		    \
		return;																												\
	}																														\
}
#else
#define NULLCHECK_DEBUG(PointerParam) /* No-op in shipping builds */
#endif
