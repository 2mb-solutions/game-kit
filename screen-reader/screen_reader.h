#include <string>
#include <queue>

#ifdef _WIN32
#include "nvdaController.h"
#include "wineyes.h"
#include <Windows.h>
#include <Ole2.h>
#include <comutil.h>
#include <tchar.h>
#include "saapi.h"
#include <sphelper.h>
#include "apiproto.h"
#include "fsapi.tlh"
#elif defined(__gnu_linux__)
#include <libspeechd.h>
#elif defined(__MACH__)
#include <ApplicationServices/ApplicationServices.h>
#endif

using std::string;
using std::queue;

/*
#ifdef _WIN32
#ifdef _MSC_VER
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "nvdaControllerClient32.lib")
#pragma comment(lib, "SAAPI32.lib")
#pragma comment(lib, "dolapi.lib")
//#import "C:\Program Files (x86)\Freedom Scientific\Shared\FSAPI\1.0\FSAPI.dll"
#endif
#endif
*/

#pragma once
class screen_reader
{
public:
    screen_reader(void);
    ~screen_reader(void);
    bool jaws_is_available();
    bool nvda_is_available();
    bool sa_is_available();
    bool wineyes_is_available();
    bool dolphin_is_available();
    bool sapi_is_available();
    bool vo_is_available();
    bool sd_is_available();
    bool speak_jaws(string text);
    bool speak_nvda(string text);
    bool speak_sa(string text);
    bool speak_wineyes(string text);
    bool speak_dolphin(string text);
    bool speak_sapi(string text);
    bool speak_vo(string text);
    bool speak_sd(string text);
    bool speak_jaws_interrupt(string text);
    bool speak_nvda_interrupt(string text);
    bool speak_sa_interrupt(string text);
    bool speak_wineyes_interrupt(string text);
    bool speak_dolphin_interrupt(string text);
    bool speak_sapi_interrupt(string text);
    bool speak_vo_interrupt(string text);
    bool speak_sd_interrupt(string text);
    int speak_vo_continue();
    bool speak_any(string text);
    bool speak_any_interrupt(string text);
#ifdef __MACH__
    static const void speak_vo_done(SpeechChannel, SRefCon);
#endif
private:
    static int classes;
    static bool windows;
    static bool is_linux;
    static bool mac;
#ifdef _WIN32
    static wchar_t* convert(const char* text);
    static _Application* application;
    static _Speech* speech;
    static bool initialized;
    static ISpVoice* sapi;
    static FSAPILib::IJawsApiPtr jawsapi;
#elif defined(__gnu_linux__)
    static bool sd_initialized;
    static SPDConnection* sd;
#elif defined(__MACH__)
    static SpeechChannel* chan;
    static queue<string> speechqueue;
#endif
    static screen_reader* classptr;
};

