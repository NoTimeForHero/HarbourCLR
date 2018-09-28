#pragma region Includes and Imports
#include <windows.h>

#include <comdef.h>
#include <metahost.h>
#include <atlbase.h>
#include <atlsafe.h>
#include <exception>
#include <cstdio>
#include <map>
#include <stack>
#pragma comment(lib, "mscoree.lib")

// Import mscorlib.tlb (Microsoft Common Language Runtime Class Library).
#import "mscorlib.tlb" raw_interfaces_only				\
    high_property_prefixes("_get","_put","_putref")		\
    rename("ReportEvent", "InteropServices_ReportEvent")
using namespace mscorlib;
#pragma endregion

#ifdef __DEBUG_X
	#define protected public
	#define private public
#endif

class CLR_Object;
class CLR_Assembly;
class CLR_Runtime;

struct NetException : std::exception
{
	const char* prefix;
	HRESULT hr;

	PWSTR assemblyName = nullptr;
	PWSTR typeName = nullptr;
	PWSTR methodName = nullptr;

	const char* what() const noexcept override
	{
		return "NetException";
	}

	NetException(const char* prefix, HRESULT hr = NULL);
};

class CLR_Object
{
	friend class CLR_Assembly;
	friend class CLR_Runtime;

protected:
	CLR_Assembly *assembly;
	_variant_t object;
	_TypePtr type{};
	PWSTR typeName;

public:
	CLR_Object(CLR_Assembly* assembly, const _variant_t& object, _TypePtr type, PWSTR typeName);

	variant_t Call(PWSTR pwszMethod);
	variant_t Call(PWSTR pwszMethod, CComSafeArray<VARIANT>* arguments);

	variant_t CallStatic(PWSTR pwszMethod);
	variant_t CallStatic(PWSTR pwszMethod, CComSafeArray<VARIANT>* arguments);

	variant_t RawValue() const;
};


class CLR_Assembly
{
	friend class CLR_Runtime;

protected:
	CLR_Runtime *runtime;
	_AssemblyPtr assembly;
	PWSTR assemblyName;

public:
	CLR_Assembly(CLR_Runtime *parent, PWSTR assemblyName, _AssemblyPtr assembly);

	variant_t CallStatic(PWSTR pszClassName, PWSTR pwszMethod);
	variant_t CallStatic(PWSTR pszClassName, PWSTR pwszMethod, CComSafeArray<VARIANT>* arguments);

	CLR_Object CreateInstance(PWSTR pszClassName);
	CLR_Object CreateInstance(PWSTR pszClassName, CComSafeArray<VARIANT>* arguments);
};

class CLR_Runtime
{
protected:
	HRESULT hr = NULL;
	CComPtr<ICLRRuntimeInfo> pRuntimeInfo = nullptr;
	CComPtr<ICorRuntimeHost> pCorRuntimeHost = nullptr;
	CComPtr<ICLRMetaHost> pMetaHost = nullptr;
	_AppDomainPtr spDefaultAppDomain = NULL;

	std::map<PWSTR, _AssemblyPtr> cachedAssemblies = std::map<PWSTR, _AssemblyPtr>();
	std::map<PWSTR, _TypePtr> cachedTypes = std::map<PWSTR, _TypePtr>();

	_AppDomainPtr getDefaultAppDomain();
	_AssemblyPtr LoadAssembly(PWSTR pszAssemblyName);

public:

	_TypePtr GetType(_AssemblyPtr spAssembly, PWSTR pszClassName);
	CLR_Assembly Load(PWSTR pszAssemblyName);

	bool isCacheTypes = true;
	bool isCacheAssemblies = true;

	~CLR_Runtime();

	CLR_Runtime(PCWSTR pszVersion);

	static const char* type_name(VARTYPE type);

	static void print_variant(variant_t value);
};