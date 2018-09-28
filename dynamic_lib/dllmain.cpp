#include "stdafx.h"

wchar_t *to_wchar(char *input, size_t size)
{
	wchar_t *value = (wchar_t*) calloc(size+1, sizeof(wchar_t));
	#ifdef _MSC_VER
		size_t unused;
		mbstowcs_s(&unused, value, size+1, input, size+1);
	#else
		mbstowcs(value, input, size);
	#endif
	return value;
}

wchar_t *to_wchar(const char *input) {
	return to_wchar((char*)input, strlen(input));
}

wchar_t *to_wchar(char *input) {
	return to_wchar(input, strlen(input));
}

char *sprintf_auto(char const *format, int *outLength, ...)
{
	va_list args;
	va_start(args, outLength);

	const int buffer_size = _vscprintf(format, args) + 1;
	char *buffer = (char*)malloc(buffer_size * sizeof(char));

	const int printf_len = vsprintf_s(buffer, buffer_size, format, args);
	if (outLength != nullptr) *outLength = printf_len;

	va_end(args);
	return buffer;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}