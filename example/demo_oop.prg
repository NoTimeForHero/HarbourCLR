PROCEDURE Main_OOP
	LOCAL aData, oCLR, oAssembly, oClass3, xVal
	LOCAL aValue

	? "Harbour CLR: Object Oriented Programming"
	? "Library Version", __CLR_VERSION()
	INKEY(5)

	// Creating new CLR runtime (look %WINDIR%\Microsoft.NET\Framework for installed versions)
	oCLR := CLR_RUNTIME():New("v4.0.30319")

	// Loading assembly (DLL)
	oAssembly := oCLR:LoadAssembly("MyAssembly")
	
	// ================= TEST1 ============================
	// Calling constructor for type "MyNamespace.Class2" with different arguments
	oAssembly:CreateInstance("MyNamespace.Class2", 322, 3.141569)
    oAssembly:CreateInstance("MyNamespace.Class2", "Is constructor of CLR Object has been called?")
	INKEY(0)

	// ================= TEST2 ============================
	// Creating instance of type MyNamespace.Class3 with default constructor
	oClass3 := oAssembly:CreateInstance("MyNamespace.Class3")
	// Calling instance void methods with name "Add" for CLR_OBJECT oClass3
	oClass3:Call("Add", 25)
	oClass3:Call("Add", 50)
	oClass3:Call("Add", 100)
	// Get value from instance method "Result"
	xVal := oClass3:Call("Result")
	? "Object Sum: ", xVal
	INKEY(0)

	// ================= TEST3 ============================
	// Calling static methods of type MyNamespace.Class1 that returns a array
	aValue := oAssembly:CallStatic("MyNamespace.Class1", "getData2")
	? HB_ValToExp(aValue)
	INKEY(0)	
	// Passing Harbour arrays to static methods of type MyNamespace.Class1
	oAssembly:CallStatic("MyNamespace.Class1", "ShowArgs", "Level One", {"Level Two: First", {"Level Three",{"Level Four", {"Level Five"}}}, "Level Two: Two"})
	oAssembly:CallStatic("MyNamespace.Class1", "ShowArgs", "Hello from Harbour!",255,.T.,128.884567,NIL,900000000,{.T.,1,2,3},DATE())

	INKEY(0)	
RETURN