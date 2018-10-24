PROCEDURE Main_OOP
	LOCAL aData, oCLR, oAssembly, oClass3, xVal
	LOCAL aValue

	? "Harbour CLR: Object Oriented Programming"
	? __CLR_VERSION()
	INKEY(5)

	aData := { NIL, .T., 244.44, 2,4,4,5,5, {10,10, .T.}, {20, 255.55}, {NIL},{{{{{44}}}}} }
	? HB_ValToExp(aData)
	oCLR := CLR_RUNTIME():New("v4.0.30319")
	
	oAssembly := oCLR:LoadAssembly("MyAssembly")
	
	// ================= TEST1 ============================
	oAssembly:CreateInstance("MyNamespace.Class2", 322, 3.141569)
        oAssembly:CreateInstance("MyNamespace.Class2", "Is constructor of CLR Object has been called?")
	INKEY(0)

	// ================= TEST2 ============================
	oClass3 := oAssembly:CreateInstance("MyNamespace.Class3")
	oClass3:Call("Add", 25)
	oClass3:Call("Add", 50)
	oClass3:Call("Add", 100)
	xVal := oClass3:Call("Result")
	? "Object Sum: ", xVal
	INKEY(0)

	// ================= TEST3 ============================
	aValue := oAssembly:CallStatic("MyNamespace.Class1", "getData2")
	? HB_ValToExp(aValue)
	INKEY(0)	
	oAssembly:CallStatic("MyNamespace.Class1", "ShowArgs", "Level One", {"Level Two: First", {"Level Three",{"Level Four", {"Level Five"}}}, "Level Two: Two"})
	oAssembly:CallStatic("MyNamespace.Class1", "ShowArgs", "Hello from Harbour!",255,.T.,128.884567,NIL,900000000,{.T.,1,2,3},DATE())

	INKEY(0)	
RETURN