#include "Runtime.h"

class CLR_Runtime;

NetException::NetException(const char* prefix, HRESULT hr)
{
	this->prefix = prefix;
	this->hr = hr;
}


// *****************************************************************
// ********            Class   CLR_Object                 ***********
// *****************************************************************

CLR_Object::CLR_Object(CLR_Assembly* assembly, const _variant_t& object, _TypePtr type, PWSTR typeName)
{
	this->assembly = assembly;
	this->object = object;
	this->type = type;
	this->typeName = typeName;
}

variant_t CLR_Object::Call(PWSTR pwszMethod)
{
	CComSafeArray<VARIANT> arguments;
	return Call(pwszMethod, &arguments);
}

variant_t CLR_Object::Call(PWSTR pwszMethod, CComSafeArray<VARIANT>* arguments)
{
	const bstr_t getArrayMethod(pwszMethod);
	variant_t retValue;

	const HRESULT hr = this->type->InvokeMember_3(
		getArrayMethod, static_cast<BindingFlags>(
			BindingFlags_InvokeMethod | BindingFlags_Instance |
			BindingFlags_Public),
		nullptr, object, *arguments, &retValue);

	if (FAILED(hr)) throw NetException("Type->InvokeMember_3", hr);
	return retValue;
}

variant_t CLR_Object::CallStatic(PWSTR pwszMethod)
{
	CComSafeArray<VARIANT> arguments;
	return CallStatic(pwszMethod, &arguments);
}

variant_t CLR_Object::CallStatic(PWSTR pwszMethod, CComSafeArray<VARIANT>* arguments)
{
	const bstr_t getArrayMethod(pwszMethod);
	variant_t retValue;

	const HRESULT hr = this->type->InvokeMember_3(getArrayMethod, static_cast<BindingFlags>(
		                                              BindingFlags_InvokeMethod | BindingFlags_Static |
		                                              BindingFlags_Public),
	                                              nullptr, object, *arguments, &retValue);

	if (FAILED(hr)) throw NetException("Type->InvokeMember_3", hr);
	return retValue;
}

_variant_t CLR_Object::RawValue() const
{
	return this->object;
}

// *****************************************************************
// ********            Class   CLR_Assembly                 ***********
// *****************************************************************

CLR_Assembly::CLR_Assembly(CLR_Runtime* runtime, PWSTR assemblyName, _AssemblyPtr assembly)
{
	this->runtime = runtime;
	this->assemblyName = assemblyName;
	this->assembly = assembly;
}


variant_t CLR_Assembly::CallStatic(PWSTR pszClassName, PWSTR pszMethod)
{
	CComSafeArray<VARIANT> arguments;
	return CallStatic(pszClassName, pszMethod, &arguments);
}

variant_t CLR_Assembly::CallStatic(PWSTR pszClassName, PWSTR pszMethod, CComSafeArray<VARIANT>* arguments)
{
	const bstr_t bstrClassName(pszClassName);
	const bstr_t bstrMethodName(pszMethod);

	const variant_t vtEmpty;
	variant_t vtResult;

	_TypePtr spType = this->runtime->GetType(this->assembly, pszClassName);

	HRESULT hr = spType->InvokeMember_3(bstrMethodName, static_cast<BindingFlags>(
		BindingFlags_InvokeMethod | BindingFlags_Static | BindingFlags_Public),
		nullptr, vtEmpty, *arguments, &vtResult);

	if (FAILED(hr)) throw NetException("invoke static", hr);
	return vtResult;
}

CLR_Object CLR_Assembly::CreateInstance(PWSTR pszClassName)
{
	CComSafeArray<VARIANT> emptyArgs;
	return CreateInstance(pszClassName, &emptyArgs);
}

CLR_Object CLR_Assembly::CreateInstance(PWSTR pszClassName, CComSafeArray<VARIANT>* arguments)
{
	// The .NET class to instantiate.
	const bstr_t bstrClassName(pszClassName);

	HRESULT hr;
	_TypePtr spType = NULL;
	variant_t vtObject;

	// Get the Type of CSSimpleObject.
	hr = this->assembly->GetType_2(bstrClassName, &spType);
	if (FAILED(hr)) throw NetException("Get the Type interface", hr);

	// Instantiate the class.
	hr = this->assembly->CreateInstance_3(
		bstrClassName, true, BindingFlags_Default,
		nullptr, *arguments, nullptr, nullptr, &vtObject);

	//hr = spAssembly->CreateInstance(bstrClassName, &vtObject);
	if (FAILED(hr)) throw NetException("Assembly::CreateInstance", hr);
	if (vtObject.vt == VT_EMPTY) throw NetException("Object is empty");

	return CLR_Object(this, vtObject, spType, pszClassName);
}


// *****************************************************************
// ********            Class   CLR_Runtime                 ***********
// *****************************************************************

// ---- Protected Functions ----

_AppDomainPtr CLR_Runtime::getDefaultAppDomain()
{
	IUnknownPtr spAppDomainThunk = NULL;
	_AppDomainPtr spDefaultAppDomain = NULL;

	// Get a pointer to the default AppDomain in the CLR.
	hr = pCorRuntimeHost->GetDefaultDomain(&spAppDomainThunk);
	if (FAILED(hr)) throw NetException("ICorRuntimeHost::GetDefaultDomain", hr);

	hr = spAppDomainThunk->QueryInterface(IID_PPV_ARGS(&spDefaultAppDomain));
	if (FAILED(hr)) throw NetException("Get default AppDomain", hr);

	return spDefaultAppDomain;
}

_AssemblyPtr CLR_Runtime::LoadAssembly(PWSTR pszAssemblyName)
{
	if (isCacheAssemblies && cachedAssemblies.count(pszAssemblyName) > 0)
	{
		return cachedAssemblies.at(pszAssemblyName);
	}

	_AssemblyPtr spAssembly = NULL;

	const bstr_t bstrAssemblyName(pszAssemblyName);

	hr = spDefaultAppDomain->Load_2(bstrAssemblyName, &spAssembly);
	if (FAILED(hr)) throw NetException("Assembly loading", hr);
	if (spAssembly == NULL) throw NetException("Can't load Assembly!");

	cachedAssemblies.emplace(pszAssemblyName, spAssembly);
	return spAssembly;
}

inline _TypePtr CLR_Runtime::GetType(_AssemblyPtr spAssembly, PWSTR pszClassName)
{
	if (isCacheTypes && cachedTypes.count(pszClassName) > 0)
	{
		return cachedTypes.at(pszClassName);
	}


	const bstr_t bstrTypeName(pszClassName);

	_TypePtr spType = NULL;

	// Get the Type of CSSimpleObject.
	hr = spAssembly->GetType_2(bstrTypeName, &spType);
	if (FAILED(hr)) throw NetException("Get the Type interface", hr);
	if (spType == NULL) throw NetException("Can't load Type!");

	cachedTypes.emplace(pszClassName, spType);
	return spType;
}



// ---- Public Functions ----

CLR_Assembly CLR_Runtime::Load(PWSTR pszAssemblyName)
{
	return CLR_Assembly(this, pszAssemblyName, LoadAssembly(pszAssemblyName));
}


CLR_Runtime::~CLR_Runtime()
{
	for (auto const& x : cachedAssemblies)
		((_AssemblyPtr)x.second).Release();

	for (auto const& x : cachedTypes)
		((_TypePtr)x.second).Release();

	spDefaultAppDomain.Release();
	pMetaHost.Release();
	pRuntimeInfo.Release();
	pCorRuntimeHost.Release();
}

CLR_Runtime::CLR_Runtime(PCWSTR pszVersion)
{
	hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
	if (FAILED(hr)) throw NetException("CLRCreateInstance", hr);

	// Get the ICLRRuntimeInfo corresponding to a particular CLR version. It 
	// supersedes CorBindToRuntimeEx with STARTUP_LOADER_SAFEMODE.
	hr = pMetaHost->GetRuntime(pszVersion, IID_PPV_ARGS(&pRuntimeInfo));
	if (FAILED(hr)) throw NetException("ICLRMetaHost::GetRuntime", hr);

	// Check if the specified runtime can be loaded into the process. This 
	// method will take into account other runtimes that may already be 
	// loaded into the process and set pbLoadable to TRUE if this runtime can 
	// be loaded in an in-process side-by-side fashion. 
	BOOL fLoadable;
	hr = pRuntimeInfo->IsLoadable(&fLoadable);
	if (FAILED(hr)) throw NetException("ICLRRuntimeInfo::IsLoadable", hr);
	if (!fLoadable) throw NetException(".NET runtime cannot be loaded");

	// Load the CLR into the current process and return a runtime interface 
	// pointer. ICorRuntimeHost and ICLRRuntimeHost are the two CLR hosting  
	// interfaces supported by CLR 4.0. Here we demo the ICorRuntimeHost 
	// interface that was provided in .NET v1.x, and is compatible with all 
	// .NET Frameworks. 
	hr = pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_PPV_ARGS(&pCorRuntimeHost));
	if (FAILED(hr)) throw NetException("ICLRRuntimeInfo::GetInterface", hr);

	// Start the CLR.
	hr = pCorRuntimeHost->Start();
	if (FAILED(hr)) throw NetException("ICorRuntimeHost->Start()", hr);


	// WE ARE ALWAYS USE DEFAULT APP DOMAIN
	spDefaultAppDomain = getDefaultAppDomain();
}

const char* CLR_Runtime::type_name(VARTYPE type)
{
	const auto vt = (VARENUM)type;
	switch (vt)
	{
	case VT_I1:
		return "Byte";
	case VT_I2:
		return "Short";
	case VT_I4:
		return "Int";
	case VT_I8:
		return "Long";
	case VT_UI1:
		return "Unsigned Byte";
	case VT_UI2:
		return "Unsigned Short";
	case VT_UI4:
		return "Unsigned Int";
	case VT_UI8:
		return "Unsigned Long";
	case VT_R4:
		return "Float";
	case VT_R8:
		return "Double";
	case VT_BSTR:
		return "BSTR";
	case VT_VARIANT:
		return "VT_VARIANT";
	case VT_EMPTY:
		return "VT_EMPTY";
	case VT_NULL:
		return "VT_NULL";
	case VT_DISPATCH:
		return "IDispatch";
	case VT_UNKNOWN:
		return "IUnknown";
	case VT_SAFEARRAY:
		return "VT_SAFEARRAY";
	case VT_ARRAY:
		return "VT_ARRAY";
	default:
		if (type == 8204) return "VT_ARRAY_8204";
		return "#UNKNOWN#";
	}
}

void CLR_Runtime::print_variant(variant_t value)
{
	_variant_t x;
	const auto vt = (VARENUM)value.vt;
	switch (vt)
	{
	case VT_EMPTY:
		printf("[EMPTY]");
		break;
	case VT_NULL:
		printf("NULL");
		break;
	case VT_I1:
		printf("%d", (char)value);
		break;
	case VT_I2:
		printf("%d", (SHORT)value);
		break;
	case VT_I4:
		printf("%d", (INT)value);
		break;
	case VT_I8:
		printf("%ld", (LONG)value);
		break;
	case VT_UI1:
		printf("%d", (BYTE)value);
		break;
	case VT_UI2:
		printf("%d", (USHORT)value);
		break;
	case VT_UI4:
		printf("%d", (UINT)value);
		break;
	case VT_UI8:
		printf("%ld", (ULONG)value);
		break;
	case VT_R4:
		printf("%f", (FLOAT)value);
		break;
	case VT_R8:
		printf("%f", (DOUBLE)value);
		break;
	case VT_BSTR:
		{
			auto bstr = (_bstr_t)value;
			if (!bstr) break;

			// I have no idea what I'm doing
			const int size_needed = WideCharToMultiByte(CP_UTF8, 0, bstr, bstr.length(), nullptr, 0, nullptr,
			                                            nullptr);
			std::string strTo(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, bstr, (int)bstr.length(), &strTo[0], size_needed, nullptr, nullptr);
		}
		break;
	case VT_DISPATCH:
	case VT_UNKNOWN:
	case VT_SAFEARRAY:
	case VT_ARRAY:
	default:
		;
	}
}