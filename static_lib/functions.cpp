// TODO: Refactor this file
// It looks like a codegenerated crazy boilerplate code.

#define _HB_API_INTERNAL_ 1

#include <hbapi.h>
#include <hbapierr.h>
#include <hbstack.h>
#include <windows.h>
#include <hbapiitm.h>

char* GLOBAL_DLL_NAME = "clrdyn.dll";

typedef struct {
	PHB_ITEM(*itemNew)(PHB_ITEM);
	PHB_ITEM(*itemPutNI)(PHB_ITEM, int);
	PHB_ITEM(*itemPutNL)(PHB_ITEM, long);
	PHB_ITEM(*itemPutND)(PHB_ITEM, double);
	PHB_ITEM(*itemPutC)(PHB_ITEM, const char*);
	PHB_ITEM(*itemArrayNew)(HB_SIZE);
	HB_BOOL(*arrayAdd)(PHB_ITEM, PHB_ITEM);
	void(*errInternal)(HB_ERRCODE, const char*, const char*, const char*);
} EXP_FUNCTIONS;

EXP_FUNCTIONS *hb;

EXP_FUNCTIONS* LOAD_EXP_FUNCTIONS() {
	EXP_FUNCTIONS *funcs = (EXP_FUNCTIONS*) calloc(sizeof(EXP_FUNCTIONS), 1);
	funcs->itemNew = &hb_itemNew;
	funcs->itemPutNI = &hb_itemPutNI;
	funcs->itemPutNL = &hb_itemPutNL;
	funcs->itemPutND = &hb_itemPutND;
	funcs->itemPutC = &hb_itemPutC;
	funcs->itemArrayNew = &hb_itemArrayNew;
	funcs->arrayAdd = &hb_arrayAdd;
	funcs->errInternal = &hb_errInternal;
	return funcs;
}

typedef void CLR_Runtime;
typedef void CLR_Assembly;
typedef void CLR_Object;

typedef void (*FUNC_CLR_INIT)(EXP_FUNCTIONS *funcs);
typedef CLR_Runtime* (*FUNC_CLR_GET_RUNTIME)(const char *version);
typedef CLR_Assembly* (*FUNC_CLR_LOAD_ASSEMBLY)(CLR_Runtime *runtime, const char *assemblyName);
typedef CLR_Object*(*FUNC_CLR_CREATE_INSTANCE)(CLR_Assembly* assembly, const char *className, PHB_ITEM pArgs);
typedef PHB_ITEM(*FUNC_CLR_CALL)(CLR_Object* object, const char *methodName, PHB_ITEM pArgs);
typedef PHB_ITEM(*FUNC_CLR_CALL_STATIC)(CLR_Assembly *assembly, const char *className, const char *methodName, PHB_ITEM args);

FUNC_CLR_INIT clr_init;
FUNC_CLR_GET_RUNTIME clr_get_runtime;
FUNC_CLR_LOAD_ASSEMBLY clr_load_assembly;
FUNC_CLR_CALL_STATIC clr_call_static;
FUNC_CLR_CREATE_INSTANCE clr_create_instance;
FUNC_CLR_CALL clr_call;

HINSTANCE HINST_DLL;

// FUNC(cAssemblyName, cClassName, cMethod, ....ARGS)
// cAssemblyName - .NET DLL filename without extension to load
// cClassName - Full class name (INCLUDING NAMESPACE!)
// cMethod - Public static method name
// ...ARGS - arguments for cMethod (METHOD SIGNATURE MUST BE CORRECT!)

void throwFuncLoadError(char *funcName) {
	char buffer[255];
#ifdef _MSC_VER
	sprintf_s(buffer, 255, "Can't load function (GetLastError code %d): %s\n", GetLastError(), funcName);	
#else
	sprintf(buffer, "Can't load function (GetLastError code %d): %s\n", GetLastError(), funcName);
#endif
	hb_errInternal(2091, (const char*)buffer, NULL, NULL);		
}

HB_FUNC(__CLR_INIT) {
        HINST_DLL = LoadLibrary(GLOBAL_DLL_NAME);
	if (!HINST_DLL) {
		char buffer[255];
#ifdef _MSC_VER
		sprintf_s(buffer, 255, "Can't load DLL (GetLastError code %d): %s\n", GetLastError(), GLOBAL_DLL_NAME);
#else
		sprintf(buffer, "Can't load DLL (GetLastError code %d): %s\n", GetLastError(), GLOBAL_DLL_NAME);
#endif
		hb_errInternal(2090, (const char*)buffer, NULL, NULL);		
		return;
	}
	
	clr_init = (FUNC_CLR_INIT) GetProcAddress(HINST_DLL, "CLR_INIT");
	if (!clr_init) {
		throwFuncLoadError("CLR_INIT");
		return;
	}

	clr_get_runtime = (FUNC_CLR_GET_RUNTIME) GetProcAddress(HINST_DLL, "CLR_GET_RUNTIME");
	if (!clr_get_runtime) {
		throwFuncLoadError("CLR_GET_RUNTIME");
		return; 
	}

	clr_load_assembly = (FUNC_CLR_LOAD_ASSEMBLY)GetProcAddress(HINST_DLL, "CLR_LOAD_ASSEMBLY");
	if (!clr_load_assembly) {
		throwFuncLoadError("CLR_LOAD_ASSEMBLY");
		return;
	}

	clr_create_instance = (FUNC_CLR_CREATE_INSTANCE)GetProcAddress(HINST_DLL, "CLR_CREATE_INSTANCE");
	if (!clr_load_assembly) {
		throwFuncLoadError("CLR_CREATE_INSTANCE");
		return;
	}

	clr_call = (FUNC_CLR_CALL)GetProcAddress(HINST_DLL, "CLR_CALL");
	if (!clr_call) {
		throwFuncLoadError("CLR_CALL");
		return;
	}

	clr_call_static = (FUNC_CLR_CALL_STATIC) GetProcAddress(HINST_DLL, "CLR_CALL_STATIC");
	if (!clr_call_static) {
		throwFuncLoadError("CLR_CALL_STATIC");
		return;
	}

	// TODO: Add CLEANUP function to library
	clr_init(LOAD_EXP_FUNCTIONS());
}

HB_FUNC(__CLR_LOAD_ASSEMBLY)
{
	if (!HINST_DLL || !clr_load_assembly)
		hb_errInternal(2111, "Library is not loaded or already has been unloaded", NULL, NULL);

	const int params_count = hb_pcount();
	const int required_params = 2;

	if (params_count != required_params) hb_errInternal(2000, "Invalid param count", NULL, NULL);

	CLR_Runtime *runtime = hb_parptr(1);
	if (!runtime) hb_errInternal(2001, "HARBOUR_CLR: Invalid Runtime Pointer", NULL, NULL);

	const char *assemblyName = hb_parc(2);
	if (!assemblyName) hb_errInternal(2003, "HARBOUR_CLR: Invalid Class Name", NULL, NULL);

	void *result = (void*)clr_load_assembly(runtime, assemblyName);
	hb_retptr(result);
}

HB_FUNC(__CLR_GET_RUNTIME) {
	if (!HINST_DLL || !clr_get_runtime)
		hb_errInternal(2111, "Library is not loaded or already has been unloaded", NULL, NULL);

	const char *clrVersion = hb_parc(1);
	if (!clrVersion) hb_errInternal(2005, "Invalid CLR version...", NULL, NULL);	

	void *result = (void*) clr_get_runtime(clrVersion);
	hb_retptr(result);
}

HB_FUNC(__CLR_CREATE_INSTANCE) {
	if (!HINST_DLL || !clr_create_instance)
		hb_errInternal(2111, "Library is not loaded or already has been unloaded", NULL, NULL);

	const int params_count = hb_pcount();
	const int required_params = 2;

	if (params_count < required_params) hb_errInternal(2000, "Invalid param count", NULL, NULL);

	CLR_Assembly *assembly = hb_parptr(1);
	if (!assembly) hb_errInternal(2001, "HARBOUR_CLR: Invalid Assembly Pointer", NULL, NULL);

	const char *className = hb_parc(2);
	if (!className) hb_errInternal(2003, "HARBOUR_CLR: Invalid Class Name", NULL, NULL);

	PHB_ITEM pArray = hb_itemArrayNew(params_count - required_params);
	for (int i = required_params + 1; i <= params_count; i++)
		hb_itemArrayPut(pArray, i - required_params, hb_param(i, HB_IT_ANY));

	void *result = (void*)clr_create_instance(assembly, className, pArray);
	hb_retptr(result);
}

HB_FUNC(__CLR_CALL) {
	if (!HINST_DLL || !clr_call)
		hb_errInternal(2111, "Library is not loaded or already has been unloaded", NULL, NULL);

	const int params_count = hb_pcount();
	const int required_params = 2;

	if (params_count < required_params) hb_errInternal(2000, "Invalid param count", NULL, NULL);

	CLR_Object *object = hb_parptr(1);
	if (!object) hb_errInternal(2001, "HARBOUR_CLR: Invalid .NET Object Pointer", NULL, NULL);

	const char *methodName = hb_parc(2);
	if (!methodName) hb_errInternal(2004, "HARBOUR_CLR: Invalid Method Name", NULL, NULL);

	PHB_ITEM pArray = hb_itemArrayNew(params_count - required_params);
	for (int i = required_params + 1; i <= params_count; i++)
		hb_itemArrayPut(pArray, i - required_params, hb_param(i, HB_IT_ANY));

	PHB_ITEM result = clr_call(object, methodName, pArray);
	hb_itemMove(hb_stackReturnItem(), result);
}
 
HB_FUNC(__CLR_CALL_STATIC) {
	if (!HINST_DLL || !clr_call_static)
		hb_errInternal(2111, "Library is not loaded or already has been unloaded", NULL, NULL);	

	const int params_count = hb_pcount();
 	const int required_params = 3;

	if (params_count < required_params) hb_errInternal(2000, "Invalid param count", NULL, NULL);	

	CLR_Assembly *assembly = hb_parptr ( 1 );
	if (!assembly) hb_errInternal(2001, "HARBOUR_CLR: Invalid Assembly Pointer", NULL, NULL);

	const char *className = hb_parc( 2 );
	if (!className) hb_errInternal(2003, "HARBOUR_CLR: Invalid Class Name", NULL, NULL);

	const char *methodName = hb_parc( 3 );
	if (!methodName) hb_errInternal(2004, "HARBOUR_CLR: Invalid Method Name", NULL, NULL);

	PHB_ITEM pArray = hb_itemArrayNew( params_count - required_params );
	for (int i=required_params+1;i<=params_count;i++)
		hb_itemArrayPut(pArray, i-required_params, hb_param(i, HB_IT_ANY));
	 
	PHB_ITEM result = clr_call_static(assembly, className, methodName, pArray);
	hb_itemMove(hb_stackReturnItem(), result);
}