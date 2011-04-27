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

#ifndef GORGUEPIPE_H
#define GORGUEPIPE_H

#include <wx/wx.h>
#include "GOrgueSound.h"

class GOrgueTremulant;
class GOrgueReleaseAlignTable;

class GOrguePipe
{

private:

	void SetOn();
	void SetOff();

	unsigned int m_Channels;
	unsigned int m_SampleRate;

	GOrgueReleaseAlignTable* m_ra_table;

	void GetMaxAmplitudeAndDerivative(AUDIO_SECTION& section, int& runningMaxAmplitude, int& runningMaxDerivative);
	void ComputeReleaseAlignmentInfo();

public:

	~GOrguePipe();
	GOrguePipe();

	void Set(bool on);
	void LoadFromFile(const wxString& filename, int amp);
	void CreateFromTremulant(GOrgueTremulant* tremulant);

	float pitch;
	GO_SAMPLER* sampler;
	int instances;

	/* states which windchest this pipe belongs to... groups from
	 * 0 to GetTremulantCount-1 are purely there for tremulants. */
	int WindchestGroup;
	int ra_amp;
	int ra_shift;

	AUDIO_SECTION m_attack;
	AUDIO_SECTION m_loop;
	AUDIO_SECTION m_release;

};

#endif
