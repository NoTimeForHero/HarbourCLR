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
		free(buffer); // TODO: Must catch it in Harbour to confirm is it safe to free this const char*
	}
	catch (const NetException &net) {
		char *buffer = sprintf_auto("[DLL %s][NetException] HRESULT %08X, message: %s\n", nullptr, DLL_VERSION, net.hr, net.prefix);
		hb->errInternal(2041, (const char*)buffer, NULL, NULL);
		free(buffer); // TODO: Must catch it in Harbour to confirm is it safe to free this const char*
	}
}

/*EXPORT_FUNC*/ CLR_Assembly* CLR_LOAD_ASSEMBLY(CLR_Runtime *runtime, const char *assemblyName)
{
	try {
		const CLR_Assembly assembly = runtime->Load(to_wchar(assemblyName));
		return new CLR_Assembly(assembly);
	}
	catch (...) { error_handler(); }
	return nullptr;
}

/*EXPORT_FUNC*/ CLR_Object* CLR_CREATE_INSTANCE(CLR_Assembly* assembly, const char *className, PHB_ITEM pArgs)
{
	try {
		CComSafeArray<VARIANT> sharp_arguments = HBARRAY_TO_SAFEARRAY(pArgs);
		const CLR_Object object = assembly->CreateInstance(to_wchar(className), &sharp_arguments);
		return new CLR_Object(object);
	}
	catch (...) { error_handler(); }
	return nullptr;
}

/*EXPORT_FUNC*/ PHB_ITEM CLR_CALL(CLR_Object* object, const char *methodName, PHB_ITEM pArgs)
{
	try {
		CComSafeArray<VARIANT> sharp_arguments = HBARRAY_TO_SAFEARRAY(pArgs);
		_variant_t value = object->Call(to_wchar(methodName), &sharp_arguments);
		return SAFEARRAY_TO_HBARRAY(value);
	}
	catch (...) { error_handler(); }
	return nullptr;
}

/*EXPORT_FUNC*/ PHB_ITEM CLR_CALL_STATIC(CLR_Assembly* assembly, const char *className, const char *methodName, PHB_ITEM pArgs) {
	try {
		CComSafeArray<VARIANT> sharp_arguments = HBARRAY_TO_SAFEARRAY(pArgs);
		_variant_t value = assembly->CallStatic(to_wchar(className), to_wchar(methodName), &sharp_arguments);
		return SAFEARRAY_TO_HBARRAY(value);
	}
	catch (...) { error_handler(); }
	return nullptr;
}

void CLR_INIT(EXP_FUNCTIONS *funcs) {
	hb = funcs;
}

CLR_Runtime *CLR_GET_RUNTIME(char *version) {
	try { return new CLR_Runtime(to_wchar(version)); }
	catch (...) { error_handler(); }
	return nullptr;
}