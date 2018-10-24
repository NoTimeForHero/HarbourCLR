PROCEDURE Main_Raw
	LOCAL aData, pCLR, pAssembly, pObj, xVal
	LOCAL aValue

	__CLR_INIT()
	? "Harbour CLR: Raw DLL Functions"
	? "Library Version", __CLR_VERSION()
	INKEY(5)

	pCLR := __CLR_GET_RUNTIME("v4.0.30319")
	pAssembly := __CLR_LOAD_ASSEMBLY(pCLR, "MyAssembly")
	
	// ================= TEST1 ============================
	? __CLR_CREATE_INSTANCE(pAssembly, "MyNamespace.Class2", 322, 3.141569)
	? __CLR_CREATE_INSTANCE(pAssembly, "MyNamespace.Class2", "Is constructor of CLR Object has been called?")

	// ================= TEST2 ============================
	pObj := __CLR_CREATE_INSTANCE(pAssembly, "MyNamespace.Class3")
	__CLR_CALL(pObj, "Add", 25)
	__CLR_CALL(pObj, "Add", 50)
	__CLR_CALL(pObj, "Add", 100)
	xVal := __CLR_CALL(pObj, "Result")
	? "Object Sum: ", xVal
	INKEY(0)

	// ================= TEST3 ============================
	aValue := __CLR_CALL_STATIC(pAssembly, "MyNamespace.Class1", "getData2")
	? HB_ValToExp(aValue)
	INKEY(0)	

	__CLR_CALL_STATIC(pAssembly, "MyNamespace.Class1", "ShowArgs", "Level One", {"Level Two: First", {"Level Three",{"Level Four", {"Level Five"}}}, "Level Two: Two"})
	__CLR_CALL_STATIC(pAssembly, "MyNamespace.Class1", "ShowArgs", "Hello from Harbour!",255,.T.,128.884567,NIL,900000000,{.T.,1,2,3},DATE())
	INKEY(0)	
RETURN