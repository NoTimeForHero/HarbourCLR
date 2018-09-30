PROCEDURE Main
	LOCAL pCLR, pAssembly, aValue

	__CLR_INIT()
	pCLR := __CLR_GET_RUNTIME("v4.0.30319")
	pAssembly := __CLR_LOAD_ASSEMBLY(pCLR, "Tests")

	// Signed numbers: byte 120, short 30000, int 5000000, long 2000000000
	aValue := __CLR_CALL_STATIC(pAssembly, "MyNamespace.Test1", "GetSignedNumbers")
	TEST(1, {120, 30000, 5000000, 2000000000}, aValue)

	aValue := __CLR_CALL_STATIC(pAssembly, "MyNamespace.Test1", "SumSignedNumbers", 5000, 5000000)
	TEST(2, 5000000+5000, aValue)

	aValue := __CLR_CALL_STATIC(pAssembly, "MyNamespace.Test1", "StringConcat", "Hello", "World")
	TEST(3, "Hello World", aValue)	

	aValue := __CLR_CALL_STATIC(pAssembly, "MyNamespace.Test1", "SumArrayElements", {1,{{2,3,4}, {{5}}}})
	TEST(4, 1+2+3+4+5, aValue)	

	// TODO: Add tests with float numbers (required relative number comparasion instead default)

RETURN

FUNCTION ARRVALTYPE(aData)
LOCAL nI
	IF ValType(aData) == "A"	
		FOR nI := 1 TO LEN(aData)
			IF ValType(aData[nI]) == "A"
				aData[nI] := ARRVALTYPE(aData[nI])
			ELSE
				aData[nI] := ValType(aData[nI])
			ENDIF
		NEXT
	ELSE
		aData := ValType(aData)
	ENDIF
RETURN aData


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