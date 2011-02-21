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

#include "OrganView.h"
#include "GrandOrgue.h"
#include "GrandOrgueFrame.h"
#include "GOrgueSound.h"
#include "GrandOrgueFile.h"
#include "OrganDocument.h"
#include "GOrgueMeter.h"
#include "OrganPanel.h"

IMPLEMENT_DYNAMIC_CLASS(OrganView, wxView)

extern void MIDIStopChange(GOrgueStop& stop, bool on, int manual);
extern void MIDICouplerChange(GOrgueCoupler& stop, bool on);
extern void MIDIDivisionalCouplerChange(GOrgueDivisionalCoupler& stop, bool on);
extern void MIDIDivisionalPush(int manual, int which, int depth = 0);
extern void MIDIGeneralPush(GOrgueGeneral& general);

extern GrandOrgueFile* organfile;



BEGIN_EVENT_TABLE(OrganView, wxView)
EVT_COMMAND(0, wxEVT_DRAWSTOP, OrganView::OnDrawstop)
EVT_COMMAND(0, wxEVT_PUSHBUTTON, OrganView::OnDrawstop)
EVT_COMMAND(0, wxEVT_ENCLOSURE, OrganView::OnDrawstop)
EVT_COMMAND(0, wxEVT_NOTEONOFF, OrganView::OnNoteOnOff)
END_EVENT_TABLE()



bool OrganView::OnCreate(wxDocument *doc, long flags)
{
  m_panel=new OrganPanel(::wxGetApp().frame);
  SetFrame(::wxGetApp().frame);
  // Activate(true);
  // GetFrame()->SetFocus();

  return true;
}

void OrganView::OnUpdate(wxView *sender, wxObject *hint)
{
  m_panel->OnUpdate(sender,hint);
}


void OrganView::OnDraw(wxDC* dc)
{
  m_panel->OnDraw(dc);
}

void OrganView::OnDrawstop(wxCommandEvent& event)
{
  m_panel->OnDrawstop(event);
}


void OrganView::OnNoteOnOff(wxCommandEvent& event)
{
  m_panel->OnNoteOnOff(event);
}

bool OrganView::OnClose(bool deleteWindow)
{
  m_panel->Destroy();
  GetFrame()->SetBackgroundStyle(wxBG_STYLE_SYSTEM);
  GetFrame()->Refresh();
  if (!GetDocument()->Close())
	return false;

  SetFrame(0);
  Activate(false);

  return true;
}
