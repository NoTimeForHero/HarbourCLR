#include "stdafx.h"
//#pragma comment(lib,"harbour-32.lib")

EXP_FUNCTIONS *hb;

PHB_ITEM CALL_STATIC(CLR_Runtime *runtime, const char *assemblyName, const char *className, const char *methodName, PHB_ITEM pArgs) {
  
	CComSafeArray<VARIANT> sharp_arguments = HBARRAY_TO_SAFEARRAY(pArgs); //= GET_ARGS(args);

	printf("SafeArray filled with %lu elements\n", sharp_arguments.GetCount());
	printf("Runtime pointer: %p\n", runtime);
	printf("Loading assembly - %s\nCreating class - %s\nCalling method - %s\n\n\n", assemblyName, className, methodName);
	system("pause");

	CLR_Assembly assembly = runtime->Load(to_wchar(assemblyName));		
	_variant_t value = assembly.CallStatic(to_wchar(className), to_wchar(methodName), &sharp_arguments);
	return SAFEARRAY_TO_HBARRAY(value);
}

PHB_ITEM CLR_CALL_STATIC(CLR_Runtime *runtime, const char *assemblyName, const char *className, const char *methodName, PHB_ITEM pArgs) {
	try {
		return CALL_STATIC(runtime, assemblyName, className, methodName, pArgs);
	}
	catch (const _com_error &com) {
		printf("ComError\n");
		system("pause");
		char buffer[255];
		sprintf_s(buffer, 255, "[COM_ERROR] HRESULT %08X, message: %s\n", com.Error(), com.ErrorMessage());
		hb->errInternal(2040, (const char*)&buffer, NULL, NULL);
	}
	catch (const NetException &net) {
		printf("NetException\n");
		system("pause");
		char buffer[255];
		sprintf_s(buffer, 255, "Some NetException occured: %s (HR %08X)\n", net.prefix, net.hr);
		hb->errInternal(2000, (const char*)&buffer, NULL, NULL);
	}
	return nullptr;
}

void CLR_INIT(EXP_FUNCTIONS *funcs) {
	printf("[CLR_INIT] Success\n");
	hb = funcs;
}

CLR_Runtime *CLR_GET_RUNTIME(char *version) {
	CLR_Runtime *runtime = new CLR_Runtime(to_wchar(version));
	return runtime;
}