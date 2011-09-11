/*
 * GrandOrgue - free pipe organ simulator based on MyOrgan
 *
 * MyOrgan 1.0.6 Codebase - Copyright 2006 Milan Digital Audio LLC
 * MyOrgan is a Trademark of Milan Digital Audio LLC
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef GORGUESOUNDTYPES_H
#define GORGUESOUNDTYPES_H

/* Number of samples to store from previous block decode as history. This may
 * be needed for features such as release alignment and compression. */
#define BLOCK_HISTORY          2

/* BLOCKS_PER_FRAME specifies the number of mono samples or stereo sample
 * pairs which are decoded for each iteration of the audio engines main loop.
 * Setting this value too low will result in inefficiencies or certain
 * features (compression) failing to work. */
#define BLOCKS_PER_FRAME       64

/* Maximum number of blocks (1 block is nChannels samples) per frame */
#define MAX_FRAME_SIZE         1024

/* Maximum number of channels the engine supports. This value can not be
 * changed at present.
 */
#define MAX_OUTPUT_CHANNELS    2

class GOrguePipe;

#define DATA_TYPE_MONO_COMPRESSED     0
#define DATA_TYPE_MONO_UNCOMPRESSED   1
#define DATA_TYPE_STEREO_COMPRESSED   2
#define DATA_TYPE_STEREO_UNCOMPRESSED 3

#pragma pack(push, 1)

struct struct_WAVE
{
	char ChunkID[4];
	int ChunkSize;
	char Format[4];
    char Subchunk1ID[4];
	int Subchunk1Size;
	short AudioFormat;
	short NumChannels;
	int SampleRate;
	int ByteRate;
	short BlockAlign;
	short BitsPerSample;
	char Subchunk2ID[4];
	int Subchunk2Size;
};

#pragma pack(pop)

typedef enum
{
	AC_COMPRESSED_MONO = 0,
	AC_UNCOMPRESSED_MONO = 1,
	AC_COMPRESSED_STEREO = 2,
	AC_UNCOMPRESSED_STEREO = 3
} AUDIO_SECTION_TYPE;

typedef enum {
	GSS_ATTACK = 0,
	GSS_LOOP = 1,
	GSS_RELEASE = 2
} AUDIO_SECTION_STAGE;

struct AUDIO_SECTION_T;
struct GO_SAMPLER_T;

/* data structure required to support release alignment tracking. */
/*typedef struct
{
	int f[2];
} GO_RELEASE_TRACKING_INFO;*/

typedef struct AUDIO_SECTION_T
{

	/* Size of the section in BYTES */
	unsigned                   size;
	unsigned                   alloc_size;

	/* Type of the data which is stored in the data pointer */
	AUDIO_SECTION_TYPE         type;
	AUDIO_SECTION_STAGE        stage; /*overflowing,*/

	/* The starting sample and derivatives for each channel (used in the
	 * compression and release-alignment schemes */
	int                        history[BLOCK_HISTORY * MAX_OUTPUT_CHANNELS];

	/* Pointer to (size) bytes of data encoded in the format (type) */
	unsigned char*             data;

} AUDIO_SECTION;

typedef struct GO_SAMPLER_T
{
	struct GO_SAMPLER_T       *next;		// must be first!
	GOrguePipe                *pipe;
	const AUDIO_SECTION       *pipe_section;
	int                        history[BLOCK_HISTORY * MAX_OUTPUT_CHANNELS];
	/* the fade parameter is would be more appropriately named "gain". It is
	 * modified on a frame-by frame basis by the fadein and fadeout parameters
	 * which get added to it. The maximum value is defined by fademax and is
	 * a NEGATIVE value. When fade is zero, the sampler will be discarded back
	 * into the sampler pool. When it is equal to fademax, the sample will be
	 * being played back at its appropriate volume (determined by amplitude
	 * factors throughout the organ definition file.
	 *
	 * The time taken for a sample to fade out for a given a value of fadeout
	 * is:
	 * { frames to fadeout } =  { fademax } / { fadeout }
	 * { frame length } = 2 / { sample rate }
	 * { time to fadeout } = { frames to fadeout } * { frame length }
	 *                     = ( 2 * { fademax } ) / ( { fadeout } * { samplerate } )
	 * therefore:
	 * { fadeout } = ( 2 * { fademax } ) / ( { samplerate } * { time to fadeout } )
	 */
	int                        fade;
	int                        fadein;
	int                        fadeout;
	unsigned                   faderemain;
	int                        fademax;
	int                        time;
	int                        shift;
	/* current byte index of the current block into this sample */
	unsigned                   position;
} GO_SAMPLER;


#endif