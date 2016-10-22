/*
screen_reader.cpp: Easy access to screen reader speech on windows, linux, and mac.
 * By 2MB Solutions: https//2mb.solutions
 * Released under the MIT license. See license.txt for details.
 * Billy: https://stormdragon.tk/
 * Michael: https://michaeltaboada.me
 * */
#include "screen_reader.h"
#include <iostream>
using namespace std;
int screen_reader::classes = 0;
#ifdef _WIN32
bool screen_reader::windows = true;
bool screen_reader::mac = false;
bool screen_reader::is_linux = false;
bool screen_reader::initialized = false;
_Application* screen_reader::application = NULL;
_Speech* screen_reader::speech = NULL;
ISpVoice* screen_reader::sapi = NULL;
FSAPILib::IJawsApiPtr screen_reader::jawsapi = NULL;
#elif defined(__linux__)
bool screen_reader::windows = false;
bool screen_reader::mac = false;
bool screen_reader::is_linux = true;
bool screen_reader::sd_initialized = false;
SPDConnection* screen_reader::sd = NULL;
#elif defined(__MACH__)
bool screen_reader::windows = false;
bool screen_reader::is_linux = false;
bool screen_reader::mac = true;
SpeechChannel* screen_reader::chan = NULL;
queue<string> screen_reader::speechqueue = queue<string>();
#endif
screen_reader* screen_reader::classptr = NULL;


screen_reader::screen_reader(void)
{
classes++;
classptr=this;
}


screen_reader::~screen_reader(void)
{
classes--;
if(classes == 0) {
classptr = NULL;
#ifdef _WIN32
	if(sapi) {
		sapi->Release();
		sapi = NULL;
	}
	if(speech) {
		speech->Release();
		speech = NULL;
	}
	if(application) {
		application->Release();
		application = NULL;
	}
	if(jawsapi) {
		jawsapi->Release();
		jawsapi = NULL;
	}
	if(initialized) {
		CoUninitialize();
		initialized = false;
	}
#elif defined(__linux__)
	if(sd) {
spd_close(sd);
	}
#elif defined(__MACH__)
	if(chan) {
		DisposeSpeechChannel(*chan);
	}
#endif
}
}

bool screen_reader::nvda_is_available() {
#ifdef _WIN32
	return (nvdaController_testIfRunning() == 0);
#endif
	return false;
}

bool screen_reader::speak_nvda(string text) {
#ifdef _WIN32
	if(!nvda_is_available()) {
		return false;
	}
	else {
		return (nvdaController_speakText(convert(text.c_str())) == 0);
	}
#endif
	return false;
}

bool screen_reader::speak_nvda_interrupt(string text) {
#ifdef _WIN32
	if(!nvda_is_available()) {
		return false;
	}
	else if(nvdaController_cancelSpeech() != 0) {
		return false;
	}
	else {
		return (nvdaController_speakText(convert(text.c_str())) == 0);
	}
#endif
	return false;
}

#ifdef _WIN32
wchar_t* screen_reader::convert(const char* text) {
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa,text,size);
	return wa;
}
#endif

bool screen_reader::wineyes_is_available() {
#ifdef _WIN32
	if(!application && !speech) {
		if(!initialized) {
			initialized = true;
			CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
		}
		if (FindWindow(_T("GWMExternalControl"), _T("External Control"))) {
			HRESULT hr = CoCreateInstance( __uuidof(Application), NULL, CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER, __uuidof(*application), (void **)&application );
			if(SUCCEEDED(hr) && application) {
				hr = application->get_Speech( &speech );
			}
		}
		return (application && speech);
	}	
#endif
	return false;
}

bool screen_reader::speak_wineyes(string text) {
#ifdef _WIN32
	if(wineyes_is_available()) {
		_bstr_t bstr(text.c_str());
		VARIANT vMissing; 
		VariantInit( &vMissing );
		vMissing.vt = VT_ERROR;
		vMissing.scode = DISP_E_PARAMNOTFOUND;
		HRESULT hr = speech->Speak(bstr,  vMissing);
		if(FAILED(hr)) {
			speech->Release();
			speech = NULL;
			application->Release();
			application= NULL;
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::speak_wineyes_interrupt(string text) {
#ifdef _WIN32
	if(wineyes_is_available()) {
		HRESULT hr = speech->Silence();
		if(FAILED(hr)) {
			return false;
		}
		return speak_wineyes(text);
	}
#endif
	return false;
}

bool screen_reader::sd_is_available() {
#ifdef __linux__
	if(sd) {
		return true;
	}
	else {
		sd = spd_open("mbengine", NULL, NULL, SPD_MODE_SINGLE);
		if(!sd) {
			return false;
		}
		else {
			return true;
		}
	}
#endif
	return false;
}

bool screen_reader::speak_sd(string text) {
#ifdef __linux__
	if(sd_is_available()) {
		int val = spd_say(sd, SPD_MESSAGE, text.c_str());
		if(val!=-1) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::speak_sd_interrupt(string text) {
#ifdef __linux__
	if(sd_is_available()) {
		if(spd_cancel_all(sd) == 0) {
			return speak_sd(text);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::vo_is_available() {
#ifdef __MACH__
	if(chan) {
		return true;
	}
	else {
		chan = (SpeechChannel*)(malloc(sizeof(chan)));
		OSErr val = NewSpeechChannel(NULL, chan);
		if(val == 0) {
			if(SetSpeechInfo(*chan, soSpeechDoneCallBack, (void*)(&screen_reader::speak_vo_done)) != 0) {
				DisposeSpeechChannel(*chan);
				chan = NULL;
				return false;
			}
			return true;
		}
		else {
			chan = NULL;
			return false;
		}
	}
#endif
	return false;
}

bool screen_reader::speak_vo(string text) {
#ifdef __MACH__
	if(vo_is_available()) {
		const char* text2 = text.c_str();
		OSErr val = SpeakBuffer(*chan, text2, strlen(text2)+1, kNoSpeechInterrupt);
		if(val == 0) {
			return true;
		}
		else {
			speechqueue.push(text);
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::speak_vo_interrupt(string text) {
#ifdef __MACH__
	if(vo_is_available()) {
		const char* text2 = text.c_str();
		OSErr val = SpeakBuffer(*chan, text2, strlen(text2)+1, 0);
		if(val == 0) {
			speechqueue = queue<string>();;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::sa_is_available() {
#ifdef _WIN32
	if(SA_IsRunning()) {
		return true;
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::speak_sa(string text) {
#ifdef _WIN32
	if(sa_is_available()) {
		if(SA_SayA(text.c_str())) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::speak_sa_interrupt(string text) {
#ifdef _WIN32
	if(sa_is_available()) {
		if(SA_StopAudio()) {
			return speak_sa(text);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::sapi_is_available() {
#ifdef _WIN32
	if(!initialized) {
		CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
		initialized = true;
	}
	if(!sapi) {
		HRESULT res = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (LPVOID *)&sapi);
		if(SUCCEEDED(res)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
#endif
	return false;
}

bool screen_reader::speak_sapi(string text) {
#ifdef _WIN32
	if(sapi_is_available()) {
		bstr_t object(text.c_str());
		HRESULT res2 = sapi->Speak(object, SPF_ASYNC, NULL);
		if(SUCCEEDED(res2)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::speak_sapi_interrupt(string text) {
#ifdef _WIN32
	if(sapi_is_available()) {
		bstr_t object(text.c_str());
		HRESULT res2 = sapi->Speak(object, SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
		if(SUCCEEDED(res2)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::dolphin_is_available() {
#ifdef _WIN32
	DWORD res = DolAccess_GetSystem();
	if(res == DOLACCESS_NONE || res == DOLACCESS_LUNAR) {
		return false;
	}
	else {
		return true;
	}
#endif
	return false;
}

bool screen_reader::speak_dolphin(string text) {
#ifdef _WIN32
	if(dolphin_is_available()) {
		WCHAR* str = convert(text.c_str());
		DWORD len = (wcslen(str)+1)*sizeof(WCHAR);
		if(DolAccess_Command((void *)str, len, DOLAPI_COMMAND_SPEAK) == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::speak_dolphin_interrupt(string text) {
#ifdef _WIN32
	if(dolphin_is_available()) {
		if(DolAccess_Action(141) == 0) {
			return speak_dolphin(text);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::jaws_is_available() {
#ifdef _WIN32
	if(initialized && jawsapi) {
		return true;
	}
	else {
		if(!initialized) {
			CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
			initialized = true;
		}
		if(!jawsapi && FindWindow("JFWUI2", NULL)) {
			FSAPILib::IJawsApiPtr jawsobj("freedomsci.jawsapi");
			jawsapi = jawsobj;
			if(!jawsobj) {
				return false;
			}
			else {
				return true;
			}
		}
	}
#endif
	return false;
}

bool screen_reader::speak_jaws(string text) {
#ifdef _WIN32
	if(jaws_is_available()) {
		_bstr_t str(text.c_str());
		if(jawsapi->SayString(str, VARIANT_FALSE)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

bool screen_reader::speak_jaws_interrupt(string text) {
#ifdef _WIN32
	if(jaws_is_available()) {
		_bstr_t str(text.c_str());
		if(jawsapi->SayString(str, VARIANT_TRUE)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
#endif
	return false;
}

int screen_reader::speak_vo_continue() {
#ifdef __MACH__
	if(speechqueue.empty()) {
		return -1;
	}
	else {
		string text = speechqueue.front();
		if(speak_vo(text)) {
			speechqueue.pop();
			return 1;
		}
		else {
			return 0;
		}
	}
#endif
	return 0;
}

bool screen_reader::speak_any(string text) {
bool success = false;
#ifdef _WIN32
	if(jaws_is_available()) {
		success = speak_jaws(text);
	}
	if(nvda_is_available() && !success) {
		success = (success || speak_nvda(text));
	}
	if(sa_is_available() && !success) {
		success = (success || speak_sa(text));
	}
	if(dolphin_is_available() && !success) {
		success = (success || speak_dolphin(text));
	}
	if(wineyes_is_available() && !success) {
		success = (success || speak_wineyes(text));
	}
	if(sapi_is_available() && !success) {
		success = speak_sapi(text);
	}
	return success;
#elif defined(__linux__)
	if(sd_is_available()) {
		return speak_sd(text);
	}
	else {
		return false;
	}
#elif defined(__MACH__)
	if(vo_is_available()) {
		return speak_vo(text);
	}
	else {
		return false;
	}
#endif
}

bool screen_reader::speak_any_interrupt(string text) {
bool success = false;
#ifdef _WIN32
	if(jaws_is_available()) {
		success = speak_jaws_interrupt(text);
	}
	if(nvda_is_available() && !success) {
		success = (success || speak_nvda_interrupt(text));
	}
	if(sa_is_available() && !success) {
		success = (success || speak_sa_interrupt(text));
	}
	if(dolphin_is_available() && !success) {
		success = (success || speak_dolphin_interrupt(text));
	}
	if(wineyes_is_available() && !success) {
		success = (success || speak_wineyes_interrupt(text));
	}
	if(sapi_is_available() && !success) {
		success = speak_sapi_interrupt(text);
	}
	return success;
#elif defined(__linux__)
	if(sd_is_available()) {
		return speak_sd_interrupt(text);
	}
	else {
		return false;
	}
#elif defined(__MACH__)
	if(vo_is_available()) {
		return speak_vo_interrupt(text);
	}
	else {
		return false;
	}
#endif
}

#ifdef __MACH__
const void screen_reader::speak_vo_done(SpeechChannel ch, SRefCon refCon) {
	if(classptr) {
		classptr->speak_vo_continue();
	}
}
#endif
