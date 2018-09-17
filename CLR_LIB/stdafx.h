// For direct access to PHB_ITEM.asXXX.value methods
// HB_IS_XXX(xxx) have a lot of additional useless checkings
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

typedef struct {
	int count;
	#pragma warning(suppress:4200) 
	PHB_ITEM items[0];
} HB_FUNC_ARGS;

wchar_t *to_wchar(char *input);
wchar_t *to_wchar(const char *input);
wchar_t *to_wchar(char *input, size_t size);

EXPORT_FUNC void CLR_INIT(EXP_FUNCTIONS *funcs);
EXPORT_FUNC CLR_Runtime* CLR_GET_RUNTIME(char*);
EXPORT_FUNC PHB_ITEM CLR_CALL_STATIC(CLR_Runtime*, const char*, const char*, const char*, PHB_ITEM);