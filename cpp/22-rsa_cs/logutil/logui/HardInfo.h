#ifndef HARDINFO_H_
#define HARDINFO_H_

#define SDK_CAPI_EXPORT extern "C" __declspec(dllexport)
#define SDK_CAPI_IMPORT extern "C" __declspec(dllimport)

#ifdef FPDSYS_EXPORTS
#define FPD_EXPORT SDK_CAPI_EXPORT
#else
#define FPD_EXPORT SDK_CAPI_IMPORT
#endif

FPD_EXPORT int GetHardInfo(char str[256]);
//FPD_EXPORT int GetHardInfo(unsigned char *str);
FPD_EXPORT int GetBordId();

#endif
