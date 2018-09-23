#include "stdafx.h"
//#pragma comment(lib,"harbour-32.lib")

EXP_FUNCTIONS *hb;

void error_handler()
{
	try {
		throw;
	}
	catch (const _com_error &com) {
		char *buffer = sprintf_auto("[DLL %s][COM_ERROR] HRESULT %08X, message: %s\n", nullptr, DLL_VERSION, com.Error(), com.ErrorMessage());
		hb->errInternal(2040, (const char*)buffer, NULL, NULL);
		free(buffer);
	}
	catch (const NetException &net) {
		char *buffer = sprintf_auto("[DLL %s][NetException] HRESULT %08X, message: %s\n", nullptr, DLL_VERSION, net.hr, net.prefix);
		hb->errInternal(2041, (const char*)buffer, NULL, NULL);
		free(buffer);
	}
}

/*EXPORT_FUNC*/ PHB_ITEM CLR_CALL_STATIC(CLR_Runtime *runtime, const char *assemblyName, const char *className, const char *methodName, PHB_ITEM pArgs) {
	try {
		CComSafeArray<VARIANT> sharp_arguments = HBARRAY_TO_SAFEARRAY(pArgs);

		printf("SafeArray filled with %lu elements\n", sharp_arguments.GetCount());
		printf("Runtime pointer: %p\n", runtime);
		printf("Loading assembly - %s\nCreating class - %s\nCalling method - %s\n\n\n", assemblyName, className, methodName);
		system("pause");

		CLR_Assembly assembly = runtime->Load(to_wchar(assemblyName));
		_variant_t value = assembly.CallStatic(to_wchar(className), to_wchar(methodName), &sharp_arguments);
		return SAFEARRAY_TO_HBARRAY(value);
	}
	catch (...) { error_handler(); }
	return nullptr;
}

void CLR_INIT(EXP_FUNCTIONS *funcs) {
	printf("[CLR_INIT] Success\n");
	hb = funcs;
}

CLR_Runtime *CLR_GET_RUNTIME(char *version) {
	try { return new CLR_Runtime(to_wchar(version)); }
	catch (...) { error_handler(); }
	return nullptr;
}