#ifndef _DYNSTR_H_
#define _DYNSTR_H_

#include <Debug.hpp>

#ifdef WIN32
// warning C4200: nonstandard extension used : zero-sized array in struct/union
#pragma warning(disable: 4200)
#endif

// this is a simple string class that helps efficiently build strings
// compatibility with standard C string is of high priority. We do it by:
// - DynStr can always be read as a C string (always NULL-terminated)
// It can also be treated as a generic, binary buffer since it's binary-safe
// (i.e. you can put 0 in it and it won't screw up the buffer. It will, however
typedef struct DynStrTag {
    // how much space do we have in total for this string. We can use
    // only bufSize-1 since the last byte is always 0 for NULL termination
    ulong_t   bufSize;
    // real length of the string. does not include terminating NULL
    ulong_t   strLen;
    // by how much increase the buffer when a re-allocation is needed.
    // if 0, add only as much as necessary. This can be used to optimize
    // appending i.e. if you expect to append a lot of small strings, set
    // this to minimize the number of re-allocations due to appending
    ulong_t   reallocIncrement;
    // that's where the data begins
    char_t     *str;
} DynStr;

/* this macro returns a pointer to a string in this DynStr.
   Note that DynStr doesn't have a notion of NULL string,
   it only has empty string */
#define DYNSTR_STR(dstr) (dstr->str)

DynStr *   DynStrInit(DynStr* dstr, ulong_t bufSize);
DynStr *   DynStrNew__(ulong_t bufSize, const char* file, int line);
DynStr *   DynStrFromCharP__(const char_t *str, ulong_t initBufSize, const char* file, int line);

#define DynStrNew(size) DynStrNew__((size), __FILE__, __LINE__)
#define DynStrFromCharP(str, initBufSize) DynStrFromCharP__((str), (initBufSize), __FILE__, __LINE__)

void       DynStrSetReallocIncrement(DynStr *dstr, ulong_t increment);
DynStr *   DynStrFromCharP2(const char_t *strOne, const char_t *strTwo);
DynStr *   DynStrFromCharP3(const char_t *strOne, const char_t *strTwo, const char_t *strThree);
DynStr *   DynStrAssignCharP(DynStr *dstr, const char_t *str);
void       DynStrTruncate(DynStr *dstr, ulong_t len);
char_t *   DynStrGetCStr(DynStr *dstr);
char_t *   DynStrReleaseStr(DynStr *dstr);
char *     DynStrGetData(DynStr *dstr);
ulong_t     DynStrGetDataLen(DynStr *dstr);
ulong_t     DynStrLen(DynStr *dstr);
DynStr *   DynStrAppendData(DynStr *dstr, const char *data, ulong_t dataSize);
DynStr *   DynStrAppendCharP(DynStr *dstr, const char_t *str);
DynStr *   DynStrAppendCharP2(DynStr *dstr, const char_t *str1, const char_t *str2);
DynStr *   DynStrAppendCharP3(DynStr *dstr, const char_t *str1, const char_t *str2, const char_t *str3);
DynStr *   DynStrAppendCharPBuf(DynStr *dstr, const char_t *str, ulong_t strLen);
DynStr *   DynStrAppendChar(DynStr *dstr, const char_t c);
void       DynStrFree(DynStr *dstr);
void       DynStrDelete(DynStr *dstr);
char_t *   DynStrCharPCopy(DynStr *dstr);
void       DynStrRemoveStartLen(DynStr *dstr, ulong_t start, ulong_t len);
DynStr *   DynStrAppendDynStr(DynStr *dstr, DynStr *toAppend);
DynStr *   DynStrUrlEncode(DynStr *srcUrl);
void       DynStrReplace(DynStr *dstr, char_t orig, char_t replace);
void        DynStrAttachCharPBuf(DynStr* dstr, char_t* str, ulong_t len, ulong_t bufSize);
void        DynStrAttachStr(DynStr* dstr, char_t* str);
void        DynStrSwap(DynStr* s1, DynStr* s2);

class CDynStr : public DynStr
{
public:
    CDynStr() { bufSize = 0; strLen = 0; reallocIncrement = 0; str = NULL; }
    ~CDynStr() { DynStrFree(this); }
    void SetReallocIncrement(ulong_t increment) { DynStrSetReallocIncrement(this, increment); }
    CDynStr *AppendData(const char *data, ulong_t dataSize) { return (CDynStr*) DynStrAppendData(this, data, dataSize); }
    void Truncate(ulong_t len) { DynStrTruncate(this, len); }
    CDynStr *AssignCharP(const char_t *str) { return (CDynStr*) DynStrAssignCharP(this, str); }
    char_t *GetCStr() { return DynStrGetCStr(this); }
    char_t *GetCharPCopy() { return DynStrCharPCopy(this); }
    ulong_t Len() { return DynStrLen(this); }
    CDynStr *AppendCharP(const char_t *str) { return (CDynStr*)DynStrAppendCharP(this, str); }
    CDynStr *AppendCharP2(const char_t *str1, const char_t *str2) { return (CDynStr*)DynStrAppendCharP2(this, str1, str2); }
    CDynStr *AppendCharP3(const char_t *str1, const char_t *str2, const char_t *str3) { return (CDynStr*)DynStrAppendCharP3(this, str1, str2, str3); }
    CDynStr *AppendCharPBuf(const char_t *str, ulong_t strLen) { return (CDynStr*)DynStrAppendCharPBuf(this, str, strLen); }
    CDynStr *AppendChar(const char_t c) { return (CDynStr*)DynStrAppendChar(this, c); }
    CDynStr *Append(DynStr *dynStr) { return (CDynStr *)DynStrAppendDynStr(this, dynStr); }
    char_t* ReleaseStr() {return DynStrReleaseStr(this);}
    void AttachStr(char_t* str) {DynStrAttachStr(this, str);}
    void AttachCharPBuf(char_t* buf, ulong_t len, ulong_t bufLen) {DynStrAttachCharPBuf(this, buf, len, bufLen);}
};

namespace std {
template<> inline void swap(DynStr& s1, DynStr& s2) {DynStrSwap(&s1, &s2);}
}

void ReplaceCDynStrP(CDynStr** target, CDynStr* newValue);
void ReplaceCharP(char_t** target, char_t* newValue);
void FreeCharP(char_t ** str);

#ifdef DEBUG
void test_DynStrAll();
#endif

#endif // _DYNSTR_H_
