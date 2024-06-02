#include "GUIFileBrowser.hpp"
#include "GUIMessageDlg.hpp"
#include <filesystem>
#include "imgui_stdlib.h"
#include <fstream>

namespace fs = std::filesystem;
std::string currentfileselection = "";
std::string localfilename = "";
bool fileoverwritedlg = false;

void recurfile(const char* path) {
	assert(path);
	ImGuiTreeNodeFlags treeflags = ImGuiTreeNodeFlags_None;
	bool recursive = true;
	std::string treelabel;
	fs::path strpath(path);
	ImVec4 treelblcolor = { 255,255,255,255 };
	if (fs::is_directory(strpath)) {
		for (const auto& entry : fs::directory_iterator(strpath, fs::directory_options::skip_permission_denied)) {
			if (fs::is_directory(entry.status())) {
				treelabel = entry.path().stem().generic_string();
				treeflags = ImGuiTreeNodeFlags_FramePadding;
				//treeflags |= ImGuiTreeNodeFlags_FramePadding;
				recursive = true;
				treelblcolor = { 0,255,0,255 };
			}
			if (fs::is_regular_file(entry.status())) {
				treelabel = entry.path().filename().generic_string();
				treeflags |= ImGuiTreeNodeFlags_Leaf;
				recursive = false;
				treelblcolor = {255,0,0,255};
			}
			ImGui::PushStyleColor(ImGuiCol_Text, treelblcolor);//ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(255, 0, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::ImColor(128, 0, 128, 128));
			if (ImGui::TreeNodeEx(treelabel.c_str(), treeflags, treelabel.c_str())) {
				//Only works when clicking on the directory name text itself - not the arrows! 
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					currentfileselection = entry.path().filename().generic_string();
				}
				if(recursive) recurfile(entry.path().generic_string().c_str());
				ImGui::TreePop();					
			}
			ImGui::PopStyleColor(2);
		}
	}
}

void showfilepickerdlg(bool* p_reopen, std::filesystem::path* pathout, bool open = true) {
	std::string filewindowname;
	if (open) { filewindowname = "Open file"; }
	else filewindowname = "Save file";
	const char* filebrowserdialogname = "Select File";
	static std::string custompath = fs::current_path().generic_string();
	ImGuiInputTextFlags edtflgs = ImGuiInputTextFlags_EnterReturnsTrue;
	bool unused = false;

	if (*p_reopen) {
		ImGui::OpenPopup(filewindowname.c_str());
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowBgAlpha(1.f);

	ImGui::SetNextWindowBgAlpha(1.f);
	if (ImGui::BeginPopupModal(filewindowname.c_str(), p_reopen, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::InputText("Current Path", &custompath, edtflgs)) {
		};

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, 260), true, window_flags);
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		ImGuiTreeNodeFlags treeflags = ImGuiTreeNodeFlags_OpenOnArrow;
		std::string rootlabel = custompath.c_str();
		//if (ImGui::TreeNodeEx(rootlabel.c_str(), treeflags))
		if (ImGui::TreeNodeEx("Current:", treeflags))
		{
			recurfile(custompath.c_str());
			ImGui::TreePop();
		}
		ImGui::EndChild();

		ImGui::Separator();

		/*
		ImGuiInputTextFlags readedtflgs = 0;
		readedtflgs |= ImGuiInputTextFlags_ReadOnly;
		ImGui::InputText("Selected:", &currentfileselection, readedtflgs);
		*/
	
		ImGuiInputTextFlags readedtflgs = 0;
		//readedtflgs |= ImGuiInputTextFlags_ReadOnly;
		ImGui::InputText("Filename:", &currentfileselection, readedtflgs);
		
		//if (ImGui::Button("OK", ImVec2(120, 0)))
		if (ImGui::Button("OK"))
		{
			//fileoverwritedlg ? (fileoverwritedlg = false) : (fileoverwritedlg = true);
			*pathout = custompath;
			*pathout /= currentfileselection;
			//*pathout /= localfilename;
			ImGui::CloseCurrentPopup();  *p_reopen = false;
		}
		
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); *p_reopen = false; }

		ImGui::EndPopup();
	}
}