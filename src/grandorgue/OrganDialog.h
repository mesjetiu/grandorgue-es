/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2011 GrandOrgue contributors (see AUTHORS)
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
 * MA 02111-1307, USA.
 */

#ifndef ORGANDIALOG_H_
#define ORGANDIALOG_H_

#include <wx/wx.h>

class GrandOrgueFile;
class wxTreeCtrl;
class wxSpinButton;
class OrganTreeItemData;

class OrganDialog : public wxDialog
{
	DECLARE_CLASS(OrganDialog)
private:
	GrandOrgueFile* m_organfile;
	wxTreeCtrl* m_Tree;
	wxTextCtrl* m_Amplitude;
	wxSpinButton* m_AmplitudeSpin;
	wxButton* m_Apply;
	wxButton* m_Reset;
	wxButton* m_Default;
	OrganTreeItemData* m_Last;

	void FillTree();
	void Load();
	bool Changed();
	void Modified();

public:
	OrganDialog (wxWindow* parent, GrandOrgueFile* organfile);
	~OrganDialog();

	void OnTreeChanging(wxTreeEvent& e);
	void OnTreeChanged(wxTreeEvent& e);
	void OnAmplitudeSpinChanged(wxSpinEvent& e);
	void OnAmplitudeChanged(wxCommandEvent &e);
	void OnEventApply(wxCommandEvent &e);
	void OnEventReset(wxCommandEvent &e);
	void OnEventDefault(wxCommandEvent &e);
	void OnEventOK(wxCommandEvent &e);

	DECLARE_EVENT_TABLE()

protected:

	enum {
		ID_EVENT_TREE = 200,
		ID_EVENT_APPLY,
		ID_EVENT_RESET,
		ID_EVENT_DEFAULT,
		ID_EVENT_AMPLITUDE,
		ID_EVENT_AMPLITUDE_SPIN,
	};
};

#endif