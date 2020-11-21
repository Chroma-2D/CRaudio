#include <stdio.h>
#include <stdlib.h>

#include "craudio.h"

#include "AL/al.h"

#include "ogg/ogg.h"
#include "vorbis/vorbisfile.h"

#include "modplug/modplug.h"

CRerror _current_error = CR_ERROR_NONE;

static void SetError(CRerror error) {
	_current_error = error;
}

CRbool CR_LoadOgg(const char* path, CRaudio_LoadInfo* info) {
	FILE* fp = NULL;
	vorbis_info* vi = NULL;
	OggVorbis_File vf = { 0 };

	if (!info) {
		SetError(CR_ERROR_INVALID_POINTER);
		goto failure;
	}

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
		if (info) {
			if (info->data) {
				free(info->data);
			}
		}

		if (fp) {
			fclose(fp);
		}

		ov_clear(&vf);
		return CR_FALSE;
	}
}

CRbool CR_LoadTrackerModule(const char* path, CRaudio_LoadInfo* info) {
	ModPlugFile* module = NULL;
	FILE* fp = NULL;

	if (!info) {
		SetError(CR_ERROR_INVALID_POINTER);
		goto failure;
	}

	fp = fopen(path, "rb");

	if (!fp) {
		SetError(CR_ERROR_FOPEN_FAILED);
		goto failure;
	}

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* file_buf = (char*)malloc(file_size);

	if (!file_buf) {
		SetError(CR_ERROR_OUT_OF_MEMORY);
		goto failure;
	}
	fread(file_buf, 1, file_size, fp);
	fclose(fp);
	fp = NULL;

	ModPlug_Settings settings = { 0 };
	
	ModPlug_GetSettings(&settings);
	settings.mBits = 16;
	settings.mFrequency = CR_MODPLUG_SAMPLERATE;
	settings.mChannels = 2;
	ModPlug_SetSettings(&settings);

	module = ModPlug_Load(file_buf, file_size);
	if (!module) {
		SetError(CR_ERROR_MODPLUG_FILE_CORRUPT);
		goto failure;
	}

	char tmp[CR_MODPLUG_CHUNKSIZE] = { 0 };
	size_t pcm_size = 0;

	while (1)
	{
		size_t count = ModPlug_Read(module, tmp, CR_MODPLUG_CHUNKSIZE);

		if (count == 0)
			break;

		pcm_size += count;
	}
	ModPlug_Seek(module, 0);

	info->size = pcm_size;
	info->data = (short*)malloc(info->size);
	info->freq = CR_MODPLUG_SAMPLERATE;
	info->format = AL_FORMAT_STEREO16;

	if (!info->data) {
		SetError(CR_ERROR_OUT_OF_MEMORY);
		goto failure;
	}

	ModPlug_Read(module, info->data, pcm_size);
	ModPlug_Unload(module);

	SetError(CR_ERROR_NONE);
	return CR_TRUE;

failure:
	{
		if (info) {
			if (info->data) {
				free(info->data);
			}
		}

		if (fp) {
			fclose(fp);
		}

		if (module) {
			ModPlug_Unload(module);
		}
		
		return CR_FALSE;
	}
}

CRbool CR_Free(CRaudio_LoadInfo* info) {
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