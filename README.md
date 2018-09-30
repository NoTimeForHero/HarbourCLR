# Harbour CLR Interop

A library that allows to Harbour load and execute any CLR (.NET) assemblies.
In current stage of this library you may need a write own additional CLR assembly to fix memory leaks and unxepected types conversion.

## Roadmap
* Write normal README.md
* Classes instead *__CLR_xxxx(\*ptr, ...)* methods
* Realize cleanup (with hb_retptrGC?)
* Check is it ok to free HB_errInternal const char * message after call
* Normal error messages in harbour (rewrite NetException for support call stack)
* Callbacks (with additional C# assembly for normal syntax?)
* More samples

## Finished
* Static library for CLR support (with HB_FUNCs like __CLR_INIT)
* Unit-testing

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