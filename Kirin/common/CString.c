#include "common/CString.h"

#include "common/Math.h"

#include <string.h>
#include <ctype.h>

int64 StrLen(const char* a)
{
	return strlen(a);
}

int64 WStrLen(const wchar_t* a)
{
	return wcslen(a);
}

int64 StrCmp(const char* a, const char* b, bool caseSensitive)
{
	if (caseSensitive)
	{
		return strcmp(a, b);
	}
	else
	{
		while (true)
		{
			int32 d = (int32)ToLower(*a) - (int32)ToLower(*b);
			if (d != 0 || !*a)
			{
				return d;
			}
			a++;
			b++;
		}
		return 0;
	}
}

int64 WStrCmp(const wchar_t* a, const wchar_t* b, bool caseSensitive)
{
	if (caseSensitive)
	{
		return wcscmp(a, b);
	}
	else
	{
		while (true)
		{
			int32 d = (int32)WToLower(*a) - (int32)WToLower(*b);
			if (d != 0 || !*a)
			{
				return d;
			}
			a++;
			b++;
		}
		return 0;
	}
}

int64 StrNCmp(const char* a, const char* b, int64 count, bool caseSensitive)
{
	if (caseSensitive)
	{
		return strncmp(a, b, (size_t)count);
	}
	else
	{
		for (int64 i = 0; i < count; i++)
		{
			int32 d = (int32)ToLower(*a) - (int32)ToLower(*b);
			if (d != 0 || !*a)
			{
				return d;
			}
			a++;
			b++;
		}
		return 0;
	}
}

int64 WStrNCmp(const wchar_t* a, const wchar_t* b, int64 count, bool caseSensitive)
{
	if (caseSensitive)
	{
		return wcsncmp(a, b, (size_t)count);
	}
	else
	{
		for (int64 i = 0; i < count; i++)
		{
			int32 d = (int32)WToLower(*a) - (int32)WToLower(*b);
			if (d != 0 || !*a)
			{
				return d;
			}
			a++;
			b++;
		}
		return 0;
	}
}

char* StrCpy(char* a, const char* b, int64 count)
{
	a[0] = (char)0;
	return StrCat(a, b, count);
}

wchar_t* WStrCpy(wchar_t* a, const wchar_t* b, int64 count)
{
	a[0] = (wchar_t)0;
	return WStrCat(a, b, count);
}

char* StrCat(char* a, const char* b, int64 count)
{
	return count >= 0 ? strncat(a, b, (size_t)count) : strcat(a, b);
}

wchar_t* WStrCat(wchar_t* a, const wchar_t* b, int64 count)
{
	return count >= 0 ? wcsncat(a, b, (size_t)count) : wcscat(a, b);
}

char* StrFindChar(const char* str, char c)
{
	return strchr(str, c);
}

wchar_t* WStrFindChar(const wchar_t* str, char c)
{
	return wcschr(str, c);
}

char* StrFindCharReverse(const char* str, char c)
{
	return strrchr(str, c);
}

wchar_t* WStrFindCharReverse(const wchar_t* str, char c)
{
	return wcsrchr(str, c);
}

char* StrFind(char* str, const char* substr)
{
	return strstr(str, substr);
}

wchar_t* WStrFind(wchar_t* str, const wchar_t* substr)
{
	return wcsstr(str, substr);
}

char* StrFindReverse(char* str, const char* substr)
{
	char* result = null;
	while (str = StrFind(str, substr))
	{
		result = str;
	}
	return result;
}

wchar_t* WStrFindReverse(wchar_t* str, const wchar_t* substr)
{
	wchar_t* result = null;
	while (str = WStrFind(str, substr))
	{
		result = str;
	}
	return result;
}

bool IsSpace(char a)
{
	if (a < 0)
	{
		return false;
	}
	return isspace(a);
}

bool WIsSpace(wchar_t a)
{
	if (a < 0)
	{
		return false;
	}
	return (bool)iswspace(a);
}

bool IsDigit(char a)
{
	if (a < 0)
	{
		return false;
	}
	return (bool)isdigit(a);
}

bool WIsDigit(wchar_t a)
{
	if (a < 0)
	{
		return false;
	}
	return (bool)iswdigit(a);
}

bool IsAlpha(char a)
{
	if (a < 0)
	{
		return false;
	}
	return (bool)isalpha(a);
}

bool WIsAlpha(wchar_t a)
{
	if (a < 0)
	{
		return false;
	}
	return (bool)iswalpha(a);
}

bool IsAlphaNumeric(char a)
{
	return IsAlpha(a) || IsDigit(a);
}

bool WIsAlphaNumeric(wchar_t a)
{
	return WIsAlpha(a) || WIsDigit(a);
}

char ToUpper(char a)
{
	return (char)toupper(a);
}

wchar_t WToUpper(wchar_t a)
{
	return (wchar_t)towupper(a);
}

char ToLower(char a)
{
	return (char)tolower(a);
}

wchar_t WToLower(wchar_t a)
{
	return (wchar_t)towlower(a);
}

wchar_t* CStringToWideStringAlloc(const char* source, int64 sourceLength)
{
	int64 required = CStringToWideString(null, -1, source, sourceLength)+1;
	wchar_t* dest = MAlloc(sizeof(wchar_t)*(size_t)required);
	CStringToWideString(dest, required, source, sourceLength);
	return dest;
}

int64 CStringToWideString(wchar_t* dest, int64 destDataLength, const char* source, int64 sourceLength)
{
	if (destDataLength >= 0 && destDataLength < sourceLength+1)
	{
		Error("destDataLength is less than sourceLength+1.");
	}

	size_t convertCount = sourceLength < 0 ? (size_t)-1 : (size_t)sourceLength+1;

	int64 convertedCount = (int64)mbstowcs(dest, source, convertCount);
	if (convertedCount == (size_t)-1)
	{
		Error("source had invalid characters.")
	}

	if (dest)
	{
		// always null terminate dest.
		dest[convertedCount] = 0;
	}

	return convertedCount;
}