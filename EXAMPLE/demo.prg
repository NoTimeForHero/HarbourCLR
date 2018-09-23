PROCEDURE Main
	LOCAL aData, pCLR, pAssembly
	LOCAL aValue

	__CLR_INIT()

	aData := { NIL, .T., 244.44, 2,4,4,5,5, {10,10, .T.}, {20, 255.55}, {NIL},{{{{{44}}}}} }
	//? "aData := { NIL, .T., 244.44, 2,4,4,5,5, {10,10, .T.}, {20, 255.55}, {NIL},{{{{{44}}}}} }"
	pCLR := __CLR_GET_RUNTIME("v4.0.30319")
	//? pCLR, ValType(pCLR)
	//? pCLR
	
	pAssembly := __CLR_LOAD_ASSEMBLY(pCLR, "MyAssembly")

	//__CLR_CALL_STATIC(pCLR, "MyAssembly", "MyNamespace.Class1", "GetData")	
	aValue := __CLR_CALL_STATIC(pAssembly, "MyNamespace.Class1", "getData2")
	//? "VALUE TYPE: ", ValType(aValue), " LEN: ", LEN(aValue)
	? HB_ValToExp(aValue)
	INKEY(0)	
	__CLR_CALL_STATIC(pAssembly, "MyNamespace.Class1", "ShowArgs", "Level One", {"Level Two: First", {"Level Three",{"Level Four", {"Level Five"}}}, "Level Two: Two"})
	__CLR_CALL_STATIC(pAssembly, "MyNamespace.Class1", "ShowArgs", "Hello from Harbour!",255,.T.,128.884567,NIL,900000000,{.T.,1,2,3},DATE())
	//__CLR_CALL_STATIC(pCLR, "MY_ASSEMBLY", "MyNamespace.Class1", "TEST", aData)
	//? "SUM: ", ARRAY_SUM(aData)
//	? PRINT_FUNC_POINTER(aData)
	INKEY(0)	
RETURN