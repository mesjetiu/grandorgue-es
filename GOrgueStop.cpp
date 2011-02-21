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

#include "GOrgueStop.h"
#include "GrandOrgueFile.h"
#include "GOrgueSound.h"
#include "GOrgueDisplayMetrics.h"

/* TODO: This should not be... */
extern GrandOrgueFile* organfile;

GOrgueStop::GOrgueStop() :
	GOrgueDrawstop(),
	m_ManualNumber(0),
	Percussive(false),
	m_auto(false),
	AmplitudeLevel(0),
	NumberOfLogicalPipes(0),
	FirstAccessiblePipeLogicalPipeNumber(0),
	FirstAccessiblePipeLogicalKeyNumber(0),
	NumberOfAccessiblePipes(0),
	WindchestGroup(0),
	pipe(NULL)
{

}

void GOrgueStop::Load(IniFileConfig& cfg, const char* group, GOrgueDisplayMetrics* displayMetrics)
{
	AmplitudeLevel = cfg.ReadInteger(group, "AmplitudeLevel", 0, 1000);
	NumberOfLogicalPipes = cfg.ReadInteger(group, "NumberOfLogicalPipes", 1, 192);
	FirstAccessiblePipeLogicalPipeNumber = cfg.ReadInteger(group, "FirstAccessiblePipeLogicalPipeNumber", 1, NumberOfLogicalPipes);
	FirstAccessiblePipeLogicalKeyNumber = cfg.ReadInteger(group, "FirstAccessiblePipeLogicalKeyNumber", 1,  128);
	NumberOfAccessiblePipes = cfg.ReadInteger(group,"NumberOfAccessiblePipes", 1, NumberOfLogicalPipes);
	WindchestGroup = cfg.ReadInteger(group,"WindchestGroup", 1, organfile->GetWinchestGroupCount());
	Percussive = cfg.ReadBoolean(group,"Percussive");

	WindchestGroup += organfile->GetTremulantCount();    // we would + 1 but it already has it: clever!

	int i;
	char buffer[64];

	pipe = new wxInt16[NumberOfLogicalPipes];
	for (i = 0; i < NumberOfLogicalPipes; i++)
	{
		sprintf(buffer, "Pipe%03d", i + 1);
		wxString file = cfg.ReadString(group, buffer);

		// FIXME: this is one of the important fixes for this file, these
		// need to change... they should probably be local to GOrgueStop and
		// accessible by organ file... possibly, they should even be pushed
		// into a vector stored in the parameters (at least for the file
		// list)
		organfile->m_pipe_ptrs.push_back(&pipe[i]);
		organfile->m_pipe_files.push_back(file);
		organfile->m_pipe_windchests.push_back(WindchestGroup);
		organfile->m_pipe_percussive.push_back(Percussive);
		if (!file.StartsWith("REF:"))
		{
			organfile->m_pipe_amplitudes.push_back(organfile->GetAmplitude() * AmplitudeLevel);
			organfile->m_NumberOfPipes++;
		}
	}

	m_auto = NumberOfLogicalPipes == 1;

	GOrgueDrawstop::Load(cfg, group, displayMetrics);
}

void GOrgueStop::Save(IniFileConfig& cfg, bool prefix)
{
	GOrgueDrawstop::Save(cfg, prefix, "Stop");
}

bool GOrgueStop::Set(bool on)
{
	if (DefaultToEngaged == on)
		return on;
	DefaultToEngaged = on;
	GOrgueSound::MIDIPretend(true);
	DefaultToEngaged = !on;
	GOrgueSound::MIDIPretend(false);

	bool retval = GOrgueDrawstop::Set(on);

	if (m_auto)
	{
		GOrgueManual* manual = organfile->GetManual(m_ManualNumber);
		manual->Set(manual->FirstAccessibleKeyMIDINoteNumber + FirstAccessiblePipeLogicalKeyNumber - 1, on);
	}

	return retval;
}

GOrgueStop::~GOrgueStop(void)
{
	if (pipe)
		delete[] pipe;
}
