#pragma once

#include "common/Standard.h"

int64 StrLen(const char* a);
int64 StrLenW(const wchar_t* a);
int64 StrCmp(const char* a, const char* b, bool caseSensitive);
int64 StrCmpW(const wchar_t* a, const wchar_t* b, bool caseSensitive);
int64 StrNCmp(const char* a, const char* b, int64 count, bool caseSensitive);
int64 StrNCmpW(const wchar_t* a, const wchar_t* b, int64 count, bool caseSensitive);
char* StrCpy(char* a, const char* b, int64 count);
wchar_t* StrCpyW(wchar_t* a, const wchar_t* b, int64 count);
char* StrCat(char* a, const char* b, int64 count);
wchar_t* StrCatW(wchar_t* a, const wchar_t* b, int64 count);
char* StrFindChar(const char* str, char c);
wchar_t* StrFindCharW(const wchar_t* str, char c);
char* StrFindCharReverse(const char* str, char c);
wchar_t* StrFindCharReverseW(const wchar_t* str, char c);
char* StrFind(char* str, const char* substr);
wchar_t* StrFindW(wchar_t* str, const wchar_t* substr);
char* StrFindReverse(char* str, const char* substr);
wchar_t* StrFindReverseW(wchar_t* str, const wchar_t* substr);
bool IsSpace(char a);
bool IsSpaceW(wchar_t a);
bool IsDigit(char a);
bool IsDigitW(wchar_t a);
bool IsAlpha(char a);
bool IsAlphaW(wchar_t a);
bool IsAlphaNumeric(char a);
bool IsAlphaNumericW(wchar_t a);
char ToUpper(char a);
wchar_t ToUpperW(wchar_t a);
char ToLower(char a);
wchar_t ToLowerW(wchar_t a);
void StrToUpper(char* value, int64 count);
void StrToUpperW(wchar_t* value, int64 count);
void StrToLower(char* value, int64 count);
void StrToLowerW(wchar_t* value, int64 count);
// sourceLength is the number of characters to convert, or -1.
// returns the a pointer to a newly allocated wide string.
wchar_t* CStringToWideStringAlloc(const char* source, int64 sourceLength);
// destDataLength the total available length of the dest buffer, including the null terminator, or -1.
// sourceLength is the number of characters to convert, or -1.
// returns the number of copied characters. if dest is null, returns the number of characters that would have been copied (not including null terminator).
int64 CStringToWideString(wchar_t* dest, int64 destDataLength, const char* source, int64 sourceLength);

char* WideStringToCStringAlloc(const wchar_t* source, int64 sourceLength);
int64 WideStringToCString(char* dest, int64 destDataLength, const wchar_t* source, int64 sourceLength);
