#ifndef CRAUDIO_H_
#define CRAUDIO_H_

#include "ogg/ogg.h"
#include "vorbis/vorbisfile.h"
#include "AL/al.h"

#ifdef __cplusplus
extern("C") {
#endif
    
#ifndef CR_API
 #if defined CR_STATIC
  #define CR_API
 #elif defined(_WIN32)
  #define CR_API __declspec(dllimport)
 #else
  #define CR_API extern
 #endif
#endif

#if defined(_WIN32)
 #define CR_APIENTRY __cdecl
#else
 #define CR_APIENTRY
#endif

#define CR_MODPLUG_CHUNKSIZE 512
#define CR_MODPLUG_SAMPLERATE 48000

#define CR_FALSE 0
#define CR_TRUE 1

#define CR_ERROR_NONE                     0
#define CR_ERROR_OUT_OF_MEMORY            0xA000
#define CR_ERROR_FOPEN_FAILED             0xA001
#define CR_ERROR_INVALID_POINTER          0xA002
#define CR_ERROR_OGGVORBIS_STREAM_INVALID 0xA010
#define CR_ERROR_OGGVORBIS_FILE_CORRUPT   0xA011
#define CR_ERROR_MODPLUG_FILE_CORRUPT     0xA020

typedef int CRerror;
typedef char CRbool;

typedef struct CRaudio_LoadInfo {
    ALenum format;
    ALvoid* data;
    ALsizei size;
    ALsizei freq;
} CRaudio_LoadInfo;

CR_API CRbool CR_APIENTRY CR_LoadOgg(const char* path, CRaudio_LoadInfo* info);
CR_API CRbool CR_APIENTRY CR_LoadTrackerModule(const char* path, CRaudio_LoadInfo* info);

CR_API CRbool CR_APIENTRY CR_Free(CRaudio_LoadInfo* info);
CR_API CRerror CR_APIENTRY CR_GetError(void);

#ifdef __cplusplus
}
#endif
    
#endif
