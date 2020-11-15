#include <stdio.h>
#include <stdlib.h>

#include "craudio.h"

#include "ogg/ogg.h"
#include "vorbis/vorbisfile.h"
#include "AL/al.h"

CRerror _current_error = CR_ERROR_NONE;

static void SetError(CRerror error) {
    _current_error = error;
}

CRbool CR_LoadOgg(const char* path, CRaudio_LoadInfo* info) {
    FILE* fp = NULL;
    vorbis_info* vi = NULL;
    OggVorbis_File vf = { 0 };
    
    fp = fopen(path, "rb");
    if (!fp) {
        SetError(CR_ERROR_FOPEN_FAILED);
        goto failure;
    }
    
    if (ov_open_callbacks(fp, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
        SetError(CR_ERROR_OGGVORBIS_STREAM_INVALID);
        goto failure;
    }
    
    vi = ov_info(&vf, -1);
    info->format = vi->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    info->size = ov_pcm_total(&vf, -1) * vi->channels * 2;
    info->data = (short*)malloc(info->size);
    info->freq = vi->rate;
    
    if (!info->data) {
        SetError(CR_ERROR_OUT_OF_MEMORY);
        goto failure;
    }
    
    for (size_t size = 0, offset = 0, sel = 0;
         (size = ov_read(&vf, (char*)info->data + offset, 4096, 0, 2, 1, (int*)&sel)) != 0;
         offset += size) {
        if (size < 0) {
            SetError(CR_ERROR_OGGVORBIS_FILE_CORRUPT);
            goto failure;
        }
    }    
    
    fclose(fp);
    ov_clear(&vf);
    
    SetError(CR_ERROR_NONE);
    return CR_TRUE;
    
    failure: 
    {
        if (info->data) {
            free(info->data);
        }

        if (fp) {
            fclose(fp);
        }

        ov_clear(&vf);
        return CR_FALSE;
    }
}

CRbool CR_FreeOgg(CRaudio_LoadInfo* info) {
    if (!info) {
        SetError(CR_ERROR_INVALID_POINTER);
        return CR_FALSE;
    }
    
    if (!info->data) {
        SetError(CR_ERROR_INVALID_POINTER);
        return CR_FALSE;
    }
    
    free(info->data);
    return CR_TRUE;
}

CRerror CR_GetError(void) {
    return _current_error;
}