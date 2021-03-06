/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2015 GrandOrgue contributors (see AUTHORS)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "wxGaugeAudio.h"

#include "Images.h"
#include <wx/dcclient.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(wxGaugeAudio, wxControl)
	EVT_ERASE_BACKGROUND(wxGaugeAudio::OnErase)
END_EVENT_TABLE()

wxGaugeAudio::wxGaugeAudio(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxControl(parent, id, pos, wxSize(73, 11), wxNO_BORDER)
{
	m_value = 0;
	m_clip = false;

        m_gauge = wxBitmap(GetImage_gauge());
	m_gaugedc.SelectObject(m_gauge);
}

wxGaugeAudio::~wxGaugeAudio(void)
{
}

void wxGaugeAudio::OnErase(wxEraseEvent& event)
{
	wxDC* dc = event.GetDC();
	int split = (m_value + 1) << 1;
	dc->Blit(0, 0, split, 11, &m_gaugedc, 0, 11);
	if (66 - split)
		dc->Blit(split, 0, 66 - split, 11, &m_gaugedc, split, 0);
	dc->Blit(66, 0, 7, 11, &m_gaugedc, 66, m_clip ? 11 : 0);
}

void wxGaugeAudio::SetValue(int what)
{
	wxClientDC dc(this);
	if (what < 0)
		what = 0;
	if (what > 32)
	{
		what = 32;
		if (!m_clip)
		{
			m_clip = true;
			dc.Blit(66, 0, 7, 11, &m_gaugedc, 66, 11);
		}
	}

        int split;
        if (what > m_value)
        {
            split = (m_value + 1) << 1;
            dc.Blit(split, 0, (what - m_value) << 1, 11, &m_gaugedc, split, 11);
        }
        else if (what < m_value)
        {
            split = (what + 1) << 1;
            dc.Blit(split, 0, (m_value - what) << 1, 11, &m_gaugedc, split, 0);
        }

	m_value = what;
}

void wxGaugeAudio::ResetClip()
{
	wxClientDC dc(this);
	if (m_clip)
	{
		m_clip = false;
		dc.Blit(66, 0, 7, 11, &m_gaugedc, 66, 0);
	}
}
