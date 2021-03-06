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

#ifndef ORGANDIALOG_H_
#define ORGANDIALOG_H_

#include "GOrgueView.h"
#include <wx/dialog.h>
#include <vector>

class GOrguePipeConfigNode;
class GrandOrgueFile;
class OrganTreeItemData;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxComboBox;
class wxSpinButton;
class wxSpinEvent;
class wxTextCtrl;
class wxTreeCtrl;
class wxTreeEvent;
class wxTreeItemId;

class OrganDialog : public wxDialog, public GOrgueView
{
private:
	GrandOrgueFile* m_organfile;
	wxTreeCtrl* m_Tree;
	wxTextCtrl* m_Amplitude;
	wxSpinButton* m_AmplitudeSpin;
	wxTextCtrl* m_Gain;
	wxSpinButton* m_GainSpin;
	wxTextCtrl* m_Tuning;
	wxSpinButton* m_TuningSpin;
	wxTextCtrl* m_Delay;
	wxSpinButton* m_DelaySpin;
	wxComboBox* m_AudioGroup;
	wxString m_LastAudioGroup;
	wxChoice* m_BitsPerSample;
	int m_LastBitsPerSample;
	wxChoice* m_Channels;
	int m_LastChannels;
	wxChoice* m_Compress;
	int m_LastCompress;
	wxChoice* m_LoopLoad;
	int m_LastLoopLoad;
	wxChoice* m_AttackLoad;
	int m_LastAttackLoad;
	wxChoice* m_ReleaseLoad;
	int m_LastReleaseLoad;
	wxButton* m_Apply;
	wxButton* m_Reset;
	wxButton* m_Default;
	wxButton* m_AudioGroupAssistant;
	wxButton* m_Collapse;
	wxCheckBox* m_IgnorePitch;
	OrganTreeItemData* m_Last;
	unsigned m_LoadChangeCnt;
	wxDialog* m_ModalDialog;
	bool m_Destroying;
	bool m_DestroyPending;

	bool CloseModal();
	void FillTree();
	void Load();
	bool Changed();
	void Modified();
	void SetEmpty(wxChoice* choice);
	void RemoveEmpty(wxChoice* choice);
	void UpdateAudioGroup(std::vector<wxString> audio_group, unsigned& pos, wxTreeItemId item);
	void FillTree(wxTreeItemId parent, GOrguePipeConfigNode& config);
	void CloseTree(wxTreeItemId parent);

	void OnTreeChanging(wxTreeEvent& e);
	void OnTreeChanged(wxTreeEvent& e);
	void OnTreeUpdated(wxCommandEvent& e);
	void OnAmplitudeSpinChanged(wxSpinEvent& e);
	void OnAmplitudeChanged(wxCommandEvent &e);
	void OnGainSpinChanged(wxSpinEvent& e);
	void OnGainChanged(wxCommandEvent &e);
	void OnTuningSpinChanged(wxSpinEvent& e);
	void OnTuningChanged(wxCommandEvent &e);
	void OnDelaySpinChanged(wxSpinEvent& e);
	void OnDelayChanged(wxCommandEvent &e);
	void OnAudioGroupChanged(wxCommandEvent &e);
	void OnBitsPerSampleChanged(wxCommandEvent &e);
	void OnCompressChanged(wxCommandEvent &e);
	void OnChannelsChanged(wxCommandEvent &e);
	void OnLoopLoadChanged(wxCommandEvent &e);
	void OnAttackLoadChanged(wxCommandEvent &e);
	void OnReleaseLoadChanged(wxCommandEvent &e);
	void OnEventApply(wxCommandEvent &e);
	void OnEventReset(wxCommandEvent &e);
	void OnEventDefault(wxCommandEvent &e);
	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnAudioGroupAssitant(wxCommandEvent &e);
	void OnCollapse(wxCommandEvent &e);

protected:

	enum {
		ID_EVENT_TREE = 200,
		ID_EVENT_APPLY,
		ID_EVENT_RESET,
		ID_EVENT_AUDIO_GROUP_ASSISTANT,
		ID_EVENT_COLLAPSE,
		ID_EVENT_DEFAULT,
		ID_EVENT_AMPLITUDE,
		ID_EVENT_AMPLITUDE_SPIN,
		ID_EVENT_GAIN,
		ID_EVENT_GAIN_SPIN,
		ID_EVENT_TUNING,
		ID_EVENT_TUNING_SPIN,
		ID_EVENT_DELAY,
		ID_EVENT_DELAY_SPIN,
		ID_EVENT_AUDIO_GROUP,
		ID_EVENT_IGNORE_PITCH,
		ID_EVENT_LOOP_LOAD,
		ID_EVENT_ATTACK_LOAD,
		ID_EVENT_RELEASE_LOAD,
		ID_EVENT_BITS_PER_SAMPLE,
		ID_EVENT_CHANNELS,
		ID_EVENT_COMPRESS
	};

public:
	OrganDialog (GOrgueDocument* doc, wxWindow* parent, GrandOrgueFile* organfile);
	~OrganDialog();

	bool Destroy();

	DECLARE_EVENT_TABLE()
};

#endif
