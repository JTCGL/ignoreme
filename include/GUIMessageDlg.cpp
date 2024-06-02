#include "GUIMessageDlg.hpp"

/*
Dialog types;
Inform - just displays a message to the user, with an "ok" button to confirm, and close the diloag. Does not change accept any state to changes
Confirm - displays a confirmation dialog, with Yes/No/Cancel options; Yes/No is passed on to a bool variable, where Cancel closes the dialog without setting the bool var
MustConfirm - Same as Confirm, but without the Cancel button. the user *must* pick yes or no
Warning - same as inform, but indicates higher sverity
Error - you fucked up now, boy.

Captions;
Confirm: Yes/True/Save
Deny: No/False/Dont Save
*/

std::string msgstatusstr("", 20);
std::string icomessage("", 20);
iconseverity icsv = iconseverity::ICONONE;
bool wantsupressed = false;

void showmessagedlg(bool* p_reopen, const char* title, const char* message) {
	if (*p_reopen) {
		ImGui::OpenPopup(title);
	}
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowBgAlpha(1.f);
	if (ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(message);
		ImGui::Separator();
		ImGui::NewLine();
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); *p_reopen = false; }
		ImGui::EndPopup();
	}
}

void closedlg() {
	ImGui::CloseCurrentPopup();
}

void showmessagedlgex(bool* p_reopen, const char* title, const char* message, const char* explain, const iconseverity ico, bool* suppress, bool showsupress) {
	if (*p_reopen) {
		ImGui::OpenPopup(title);
	}
	switch (ico) {
	case iconseverity::ICOWARNING: icomessage = "[warning]"; break;
	case iconseverity::ICOERROR: icomessage = "[error]"; break;
	case iconseverity::ICOQUERY: icomessage = "[query]"; break;
	case iconseverity::ICOINFORM:
	default: icomessage = "[inform]";  break;
	}
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowBgAlpha(1.f);
	if (ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if(ico != iconseverity::ICONONE) ImGui::Text(icomessage.c_str());
		ImGui::SameLine();
		ImGui::Text(message);
		ImGui::NewLine();
		if (explain) {
			ImGui::Text(explain);
			ImGui::NewLine();
		}
		ImGui::Separator();
		//ImGui::SetItemDefaultFocus();
		//ImGui::SameLine();
		if(showsupress)
		{
			if (ImGui::Checkbox("Suppress this message", &wantsupressed)) {}
			ImGui::SameLine();
		}
		//if (ImGui::Button("Yes", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); *p_reopen = false; }
		if (ImGui::Button("Yes")) { closedlg(); *p_reopen = false; *suppress = wantsupressed; wantsupressed = false; }
		ImGui::SameLine();
		if (ImGui::Button("No" )) { closedlg(); *p_reopen = false; *suppress = wantsupressed; wantsupressed = false; }
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) { closedlg(); *p_reopen = false; wantsupressed = false; }
		ImGui::EndPopup();
		//ImTextStrToUtf8();
	}
}