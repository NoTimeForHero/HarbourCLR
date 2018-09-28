PROCEDURE Main
	LOCAL aData, pCLR, pAssembly, pObj, xVal
	LOCAL aValue

	TEST(1, 25, 25)
	TEST(2, "TEST", "TEST")
	TEST(3, {5, 25, {{50}}}, {5, 25, {{30}}})
	TEST(4, 300, 500)
	
	// TODO: Write Real Tests
	ExitProcess(0)

	__CLR_INIT()

	aData := { NIL, .T., 244.44, 2,4,4,5,5, {10,10, .T.}, {20, 255.55}, {NIL},{{{{{44}}}}} }
	//? "aData := { NIL, .T., 244.44, 2,4,4,5,5, {10,10, .T.}, {20, 255.55}, {NIL},{{{{{44}}}}} }"
	pCLR := __CLR_GET_RUNTIME("v4.0.30319")
	//? pCLR, ValType(pCLR)
	//? pCLR
	
	pAssembly := __CLR_LOAD_ASSEMBLY(pCLR, "Tests")
	
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

PROCEDURE TEST(nID, Expected, Result)
LOCAL nI
	IF ValType(Expected) != ValType(Result)
		?
		? "Test #" + ALLTRIM(STR(nID)) + " Failed!"
		? "Expected Type: ", ValType(Expected)
		? "Result Type: ", ValType(Result)
		ExitProcess(nID)
	ENDIF
	
	IF ValType(Expected) == "A"	
		IF (LEN(Expected) != LEN(Result))
			?
			? "Test #" + ALLTRIM(STR(nID)) + " Failed!"
			? "Length of Expected array: ", LEN(Expected)
			? "Length of Result array: ", LEN(Result)
			ExitProcess(nID)
		ENDIF
	
		FOR nI := 1 TO LEN(Expected)
			IF ValType(Expected[nI]) == "A" .OR. ValType(Result[nI]) == "A"
				TEST(nID, Expected[nI], Result[nI])
			ELSE
				IF Expected[nI] != Result[nI]
					?
					? "Test #" + ALLTRIM(STR(nID)) + " Failed!"
					? "Array item: " + ALLTRIM(STR(nI))
					? "Expected value:", Expected[nI]
					? "Result value:", Result[nI]
					ExitProcess(nID)
				ENDIF
			ENDIF
		NEXT

		RETURN
	ENDIF

	IF Expected != Result
		?
		? "Test #" + ALLTRIM(STR(nID)) + " Failed!"
		? "Expected: ", Expected
		? "Got result: ", Result
		ExitProcess(nID)
	ENDIF

	? "Test #" + ALLTRIM(STR(nID)) + " Passed -> "
	?? Expected
RETURN

#PRAGMA BEGINDUMP
#include "windows.h"
#include "hbapi.h"

HB_FUNC(EXITPROCESS) {
	ExitProcess(hb_parni(1));
}
#PRAGMA ENDDUMP