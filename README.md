# Harbour CLR Interop - Alpha

A library that allows to Harbour load and execute any CLR (.NET) assemblies.
In current stage of this library you may need a write own additional CLR assembly to prevent memory leaks and unxepected types conversion.

Installed versions of .NET Framework in your machine you can get inside directory %WINDIR%\Microsoft.NET\Framework

## How to use
1. Add reference to CLR library into your HBP file (-oclr).
2. Put compiled dynamic library (clrdyn.dll) to your project output directory.
3. Add to project output directory needed CLR assemblies.
4. Load CLR runtime by creating instance CLR_RUNTIME():New("v4.0.30319")
5. Call CLR_RUNTIME:LoadAssembly(ASSEMBLY_NAME) to load needed assembly (it return new instance of class CLR_ASSEMBLY)
6. Now you can CLR call static method by CLR_ASSEMBLY->CallStatic(TYPE_NAME, ...ARGS...)
7. To create new CLR_OBJECT instance use CLR_ASSEMBLY->CreateInstance(TYPE_NAME, ...ARGS..)
8. To call instance methods on created objects use CLR_OBJECT->CALL(...ARGS...)

You can also look [example directory](example\demo_oop.prg) for more information

## Type Conversions
### Supported Types:
* NIL
* Array
* Logical
* Integer
* Long
* Double
* String

### Unsupported Types:
* Date
* Timestamp
* Alias
* Symbol
* Pointer

**Warning!** Next C# types have unexpected conversion due Harbour limitations: 
* (unsigned) byte → HB_LONG
* (unsigned) short → HB_LONG
* (unsigned) int → HB_LONG 
* unsigned long → HB_LONG
* float → HB_DOUBLE
* object/unknown → HB_NIL


## Roadmap
* Write normal README.md
* Return CLR Objects from methods like __CLR_CALL_STATIC/__CLR_CALL, not only primitive types
* Realize cleanup (with hb_retptrGC?)
* Check is it ok to free HB_errInternal const char * message after call
* Normal error messages in harbour (rewrite NetException for support call stack)
* Callbacks (with additional C# assembly for normal syntax?)
* More samples

## Finished
* Static library for CLR support (with HB_FUNCs like __CLR_INIT)
* Classes instead *__CLR_xxxx(\*ptr, ...)* methods
* Unit-testing