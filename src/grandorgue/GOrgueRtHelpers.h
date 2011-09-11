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

#ifndef GORGUERTHELPERS_H_
#define GORGUERTHELPERS_H_

#include <wx/wx.h>
#include "RtAudio.h"

class GOrgueRtHelpers
{

private:

	static void GetDirectSoundConfig(const int latency, unsigned *nb_buffers, unsigned *buffer_size);
	static void GetAsioSoundConfig(const int latency, unsigned *nb_buffers, unsigned *buffer_size);
	static void GetJackSoundConfig(const int latency, unsigned *nb_buffers, unsigned *buffer_size);
	static void GetUnknownSoundConfig(const int latency, unsigned *nb_buffers, unsigned *buffer_size);

public:

	static void GetBufferConfig
		(const RtAudio::Api   rt_api
		,const unsigned       latency_ms
		,unsigned            *nb_buffers
		,unsigned            *buffer_size_samples
		);

	static const wxChar* GetApiName(const RtAudio::Api api);
	static const wxChar* GetAudioFormatName(const int rt_audio_format);

};

#endif /* GORGUERTHELPERS_H_ */
