#include "stdafx.h"

_variant_t PHBITM_TO_VARIANT(PHB_ITEM pItem, bool *allowedType)
{
	HB_TYPE type = pItem->type;

	if (type & HB_IT_LOGICAL) {
		*allowedType = true;
		const bool value = pItem->item.asLogical.value;
		return _variant_t(value);
	}

	if (type & HB_IT_INTEGER) {
		*allowedType = true;
		const int value = pItem->item.asInteger.value;
		return _variant_t(value);
	}

	if (type & HB_IT_LONG) {
		*allowedType = true;
		const long value = pItem->item.asLong.value;
		return _variant_t(value);
	}

	if (type & HB_IT_DOUBLE) {
		*allowedType = true;
		const double value = pItem->item.asDouble.value;
		return _variant_t(value);
	}

	if (type & HB_IT_STRING) {
		*allowedType = true;
		const char *value = pItem->item.asString.value;
		return _variant_t(value);
	}

	return 0;
}

PHB_ITEM VARIANT_TO_PHBITM(variant_t value)
{
	PHB_ITEM pItem = hb->itemNew(nullptr);

	const auto vt = (VARENUM)value.vt;
	switch (vt)
	{
	case VT_EMPTY:
		break;
	case VT_NULL:
		break;
		// Signed numbers
	case VT_I1:
	case VT_I2:
	case VT_I4:
	case VT_I8:
		// Unsigned numbers
	case VT_UI1:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
		hb->itemPutNL(pItem, (LONG)value);
		break;
	case VT_R4: // Float
	case VT_R8: // Double
		hb->itemPutND(pItem, (DOUBLE)value);
		break;
	case VT_BSTR:
	{
		auto bstr = (_bstr_t)value;
		if (!bstr) break;

		// I have no idea what I'm doing
		const int size_needed = WideCharToMultiByte(CP_UTF8, 0, bstr, bstr.length(), nullptr, 0, nullptr,
			nullptr);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, bstr, (int)bstr.length(), &strTo[0], size_needed, nullptr, nullptr);

		hb->itemPutC(pItem, strTo.c_str());
	}
	break;
	case VT_DISPATCH:
	case VT_UNKNOWN:
	case VT_SAFEARRAY:
	case VT_ARRAY:
	default:
		;
	}

	return pItem;
}

CComSafeArray<VARIANT> HBARRAY_TO_SAFEARRAY(PHB_ITEM pInput) {
	CComSafeArray<VARIANT> sharp_arguments((ULONG)0);

	std::stack<std::tuple<CComSafeArray<VARIANT>, PHB_ITEM, int>> arrStack;
	PHB_ITEM pArgs = pInput;
	int iStart = 0;
	PHB_ITEM pItem;

go_deep:
	int arrLength = pArgs->item.asArray.value->nLen;
	for (int i = iStart; i<arrLength; i++) {

		pItem = pArgs->item.asArray.value->pItems + i;
		HB_TYPE type = pItem->type;
		bool allowedType = false;

		const variant_t value = PHBITM_TO_VARIANT(pItem, &allowedType);
		if (allowedType) sharp_arguments.Add(value);

		if (pItem->type & HB_IT_ARRAY) {
			CComSafeArray<VARIANT> deep_array((ULONG)0);
			arrStack.push(std::make_tuple(sharp_arguments, pArgs, i + 1));
			pArgs = pItem;
			sharp_arguments = deep_array;
			iStart = 0;
			goto go_deep;
		}

		if (!allowedType) {
			char buffer[200];
			sprintf_s(buffer, 200, "Argument %d has unsupported type - 0x%05X", i, type);
#ifdef __DEBUG
			printf("[WARNING]: %s\n\n", buffer);
#else
			hb->errInternal(2001, (const char*)&buffer, NULL, NULL);
#endif
		}
	}
	if (!arrStack.empty())
	{
		const auto current = arrStack.top();
		arrStack.pop();
		CComSafeArray<VARIANT> deep_array = std::get<0>(current);
		pArgs = std::get<1>(current);
		iStart = std::get<2>(current);

		// Unfortunately _variant_t don't have constructor for SafeArray
		// So we need fill it manually
		_variant_t vArray;
		vArray.vt = VT_ARRAY | VT_VARIANT;
		vArray.parray = sharp_arguments.Detach();
		deep_array.Add(vArray);

		sharp_arguments = deep_array;
		goto go_deep;
	}

	return sharp_arguments;
}

PHB_ITEM SAFEARRAY_TO_HBARRAY(variant_t tValue)
{
	HRESULT hr;
	int off_len = 0;

	PHB_ITEM pArray = hb->itemArrayNew(0);
	std::stack<std::tuple<variant_t, int, PHB_ITEM>> arrStack;
	int startI = 0;

array_foreach:
	if (V_ISARRAY(&tValue)) // Is Array
	{

		VARTYPE itemType;
		if (FAILED(hr = SafeArrayGetVartype(tValue.parray, &itemType))) throw NetException("Getting array type", hr);
		if (itemType != VT_VARIANT) throw NetException("Unknown array item type");

		CComSafeArray<VARIANT> pSafeArray(tValue.parray);
		if (pSafeArray.GetDimensions() > 1) throw NetException("Multidimensional arrays are not supported!");
		const long cnt_elements = pSafeArray.GetCount();

		for (int i = startI; i < cnt_elements; i++)
		{
			if (V_ISARRAY(&pSafeArray[i]))
			{
				off_len += 3;
				startI = 0;
				arrStack.push(std::make_tuple(tValue, i + 1, pArray));
				const variant_t tempVal = (variant_t)pSafeArray[i];
				tValue = tempVal;
				pArray = hb->itemArrayNew(0);
				goto array_foreach;
			}

			hb->arrayAdd(pArray, VARIANT_TO_PHBITM(pSafeArray[i]));
		}

		if (!arrStack.empty())
		{
			off_len -= 3;
			const auto current = arrStack.top();
			arrStack.pop();
			tValue = std::get<0>(current);
			startI = std::get<1>(current);

			PHB_ITEM pTemp = std::get<2>(current);
			hb->arrayAdd(pTemp, pArray);
			pArray = pTemp;

			goto array_foreach;
		}
	}
	else
	{
		// TODO: Release Item allocated before
		pArray = VARIANT_TO_PHBITM(tValue);
	}
	return pArray;
}