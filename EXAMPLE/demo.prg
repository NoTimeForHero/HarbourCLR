PROCEDURE Main
	LOCAL aData, pCLR, pAssembly, pObj, xVal
	LOCAL aValue

	__CLR_INIT()

	aData := { NIL, .T., 244.44, 2,4,4,5,5, {10,10, .T.}, {20, 255.55}, {NIL},{{{{{44}}}}} }
	//? "aData := { NIL, .T., 244.44, 2,4,4,5,5, {10,10, .T.}, {20, 255.55}, {NIL},{{{{{44}}}}} }"
	pCLR := __CLR_GET_RUNTIME("v4.0.30319")
	//? pCLR, ValType(pCLR)
	//? pCLR
	
	pAssembly := __CLR_LOAD_ASSEMBLY(pCLR, "MyAssembly")
	
	// ================= TEST2 ============================
	? __CLR_CREATE_INSTANCE(pAssembly, "MyNamespace.Class2", 322, 3.141569)
	? __CLR_CREATE_INSTANCE(pAssembly, "MyNamespace.Class2", "Is constructor of CLR Object has been called?")

	// ================= TEST3 ============================
	pObj := __CLR_CREATE_INSTANCE(pAssembly, "MyNamespace.Class3")
	__CLR_CALL(pObj, "Add", 25)
	__CLR_CALL(pObj, "Add", 50)
	__CLR_CALL(pObj, "Add", 100)
	xVal := __CLR_CALL(pObj, "Result")
	? "Object Sum: ", xVal
	INKEY(0)

	// ================= TEST1 ============================
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