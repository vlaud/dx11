#pragma once
#include <comdef.h>
#include "Debug.h"

#define COM_ERROR(hr, msg) if(FAILED(hr)) throw ComException(hr, msg, __FILEW__, __FUNCTIONW__, __LINE__)
class ComException
{
	wstring errMsg;
public:
	ComException(HRESULT hr, const wstring& msg, const wstring& file, const wstring& function, int line)
	{
		_com_error error(hr);
		errMsg = L"Message: " + msg + L"\n";
		errMsg += error.ErrorMessage();
		errMsg += L"\nFile: " + file;
		errMsg += L"\nFunction: " + function;
		errMsg += L"\nLine: " + to_wstring(line);
	}
	const wchar_t* what() const
	{
		return errMsg.c_str();
	}
};