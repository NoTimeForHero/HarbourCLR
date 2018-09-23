#pragma once
#define DLL_VERSION "0.2 Alpha"

// For direct access to PHB_ITEM.asXXX.value methods
// HB_IS_XXX(xxx) has a lot of additional useless checkings
#define _HB_API_INTERNAL_ 1

// For warning instead errors
#define __DEBUG
#define EXPORT_FUNC extern "C" __declspec(dllexport)

#include <windows.h>
#include <cstdio>

// Harbour includes
#include <hbapi.h>
#include <hbapiitm.h>
#include "runtime.h"

typedef struct {
	PHB_ITEM(*itemNew)(PHB_ITEM);
	PHB_ITEM(*itemPutNI)(PHB_ITEM, int);
	PHB_ITEM(*itemPutNL)(PHB_ITEM, long);
	PHB_ITEM(*itemPutND)(PHB_ITEM, double);
	PHB_ITEM(*itemPutC)(PHB_ITEM, const char*);
	PHB_ITEM(*itemArrayNew)(HB_SIZE);
	HB_BOOL(*arrayAdd)(PHB_ITEM, PHB_ITEM);
	void (*errInternal)(HB_ERRCODE,const char*,const char*,const char*);
} EXP_FUNCTIONS;

extern EXP_FUNCTIONS *hb;

// Signatures from: dllmain.cpp
wchar_t *to_wchar(char *input);
wchar_t *to_wchar(const char *input);
wchar_t *to_wchar(char *input, size_t size);
char *sprintf_auto(char const *format, int *outLength, ...);

// Signatures from: demo_lib.cpp
EXPORT_FUNC void CLR_INIT(EXP_FUNCTIONS *funcs);
EXPORT_FUNC CLR_Runtime* CLR_GET_RUNTIME(char*);
EXPORT_FUNC CLR_Assembly* CLR_LOAD_ASSEMBLY(CLR_Runtime *runtime, const char *assemblyName);
EXPORT_FUNC PHB_ITEM CLR_CALL_STATIC(CLR_Assembly*, const char*, const char*, PHB_ITEM);

// Signatures from: arrays.cpp
_variant_t PHBITM_TO_VARIANT(PHB_ITEM pItem, bool *allowedType);
CComSafeArray<VARIANT> HBARRAY_TO_SAFEARRAY(PHB_ITEM pInput);
PHB_ITEM SAFEARRAY_TO_HBARRAY(variant_t tValue);