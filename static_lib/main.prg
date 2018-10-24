#include "hbclass.ch"

FUNCTION __CLR_VERSION
RETURN "0.3.5 Alpha"

FUNCTION __CLR_VALIDATE_ARGUMENT_TYPE(oSelf, cTypeName)
	LOCAL oError
	IF ValType(cTypeName) != "C"
		oError := ErrorNew()
		oError:Args = { oSelf, cTypeName }
		oError:Description := "Invalid type of CLR_Type argument"
		Eval( ErrorBlock(), oError )
	ENDIF
RETURN NIL

CREATE CLASS CLR_RUNTIME
	VAR pRuntime
	METHOD New(cVersion)
	METHOD LoadAssembly(cName)
END CLASS

METHOD New(cVersion) CLASS CLR_RUNTIME
	__CLR_INIT()
	::pRuntime := __CLR_GET_RUNTIME(cVersion)	
RETURN Self

METHOD LoadAssembly(cName) CLASS CLR_RUNTIME
	LOCAL pAssembly := __CLR_LOAD_ASSEMBLY(::pRuntime, cName)	
RETURN CLR_ASSEMBLY():New(pAssembly)



CREATE CLASS CLR_ASSEMBLY
	VAR pAssembly
	METHOD New(pAssembly)
	METHOD CreateInstance(cTypeName)
	METHOD CallStatic(cTypeName)
END CLASS

METHOD New(pAssembly) CLASS CLR_ASSEMBLY
	::pAssembly := pAssembly
RETURN Self

METHOD CreateInstance(cTypeName) CLASS CLR_ASSEMBLY
	LOCAL oInstance, aParams := HB_AParams()
	__CLR_VALIDATE_ARGUMENT_TYPE(Self, cTypeName)
	HB_AIns(aParams, 1, ::pAssembly, .T.)
	oInstance := __CLR_CREATE_INSTANCE(hb_arrayToParams(aParams))
RETURN CLR_OBJECT():New(oInstance)

METHOD CallStatic(cTypeName) CLASS CLR_ASSEMBLY
	LOCAL oValue, aParams := HB_AParams()
	__CLR_VALIDATE_ARGUMENT_TYPE(Self, cTypeName)
	HB_AIns(aParams, 1, ::pAssembly, .T.)
	oValue := __CLR_CALL_STATIC(hb_arrayToParams(aParams))
RETURN oValue



CREATE CLASS CLR_OBJECT
	VAR pObject
	METHOD New(pObject)
	METHOD Call()
END CLASS

METHOD New(pObject) CLASS CLR_OBJECT
	::pObject := pObject
RETURN Self

METHOD Call() CLASS CLR_OBJECT
	LOCAL oValue, aParams := HB_AParams()
	HB_AIns(aParams, 1, ::pObject, .T.)
	oValue := __CLR_CALL(hb_arrayToParams(aParams))
RETURN oValue