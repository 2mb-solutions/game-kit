#ifndef saapi_h
#define saapi_h

#ifdef __cplusplus
extern "C"
{
#endif

#include <wchar.h>
#include <windows.h>

BOOL WINAPI SA_IsRunning(void);
BOOL WINAPI SA_SayU(const char *text);
BOOL WINAPI SA_SayW(const WCHAR *textW);
BOOL WINAPI SA_SayA(const char *textA);
BOOL WINAPI SA_StopAudio(void);
BOOL WINAPI SA_BrlShowTextU(const char *text);
BOOL WINAPI SA_BrlShowTextW(const WCHAR *text);
BOOL WINAPI SA_BrlShowTextA(const char *text);

#ifdef __cplusplus
}
#endif

#endif
