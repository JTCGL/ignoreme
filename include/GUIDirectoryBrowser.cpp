#include "GUIDirectoryBrowser.hpp"
#include <filesystem>
#include "imgui_stdlib.h"

namespace fs = std::filesystem;
std::string currentselection = "";
//std::string custompath = "";

void enumdircontents(const char* dirpath) {
	//fs::path pathToShow("C:\\Windows\\system.ini");
	//std::string mypath("C:\Windows\system.ini");
	fs::path strpath(dirpath);
	//strpath = fs::current_path();

	/*
	std::string rootname = strpath.root_name().generic_string();
	std::string root_path = strpath.root_path().generic_string();
	std::string relative_path = strpath.relative_path().generic_string();
	std::string parent_path = strpath.parent_path().generic_string();
	std::string filename = strpath.filename().generic_string();
	std::string stem = strpath.stem().generic_string();
	std::string extension = strpath.extension().generic_string();
	ImGui::Text("Root Name: %s", rootname.c_str());
	ImGui::Text("Root path: %s", root_path.c_str());
	ImGui::Text("relative_path: %s", relative_path.c_str());
	ImGui::Text("parent_path: %s", parent_path.c_str());
	ImGui::Text("filename: %s", filename.c_str());
	ImGui::Text("stem: %s", stem.c_str());
	ImGui::Text("extension: %s", extension.c_str());
	*/

	if (fs::is_directory(strpath)) {
		for (const auto& entry : fs::directory_iterator(strpath)) {
			//std::string entry_name = entry.path().generic_string();
			//ImGui::Text("entry: %s", entry_name.c_str());

			//Can filter out hidden/os/system files?
			if (fs::is_directory(entry.status()))
			{
				if (!fs::is_empty(entry)) {
					//ImGui::Text("entry: %s", entry.path().generic_string().c_str());
					if (ImGui::TreeNode(entry.path().stem().generic_string().c_str()))
					{
						if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
							//ImGui::Text("fucker");
							//if open, recursively repeat this process for each subdir, their subdir etc...
						}
						ImGui::TreePop();
					}
				}
				else {
					ImGui::Text(entry.path().stem().generic_string().c_str());//Only for testing - not selectable!
				}
			}
		}
	}
	else {
		ImGui::Text("Path is not a valid directory");
	}
}

void recurdir(const char* path) {
	assert(path);
	fs::path strpath(path);
	if (fs::is_directory(strpath)) {
		//for (const auto& entry : fs::directory_iterator(strpath))
		for (const auto& entry : fs::directory_iterator(strpath, fs::directory_options::skip_permission_denied))
		{
			if (fs::is_directory(entry.status())) {
				if (ImGui::TreeNode(entry.path().stem().generic_string().c_str())) {

					//currentselection
					//if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					//if (ImGui::IsItemHovered())
					if (ImGui::IsItemFocused())
					{
						//currentselection = entry.path().stem().generic_string();
						currentselection = entry.path().generic_string();
						//std::wstring np = entry.path().native();
						//currentselection = std::string(np.c_str());
					}
					recurdir(entry.path().generic_string().c_str());
					ImGui::TreePop();
				}
				//if (!fs::is_empty(entry)) {	}
			}
		}
	}
}

void showdirbrowser(bool* p_open) {
	ImGuiWindowFlags flags = 0;
	ImGui::SetNextWindowBgAlpha(1.f);
	flags |= ImGuiWindowFlags_NoCollapse;
	if (!ImGui::Begin("Directory", p_open, flags)) {
		ImGui::End();
		return;
	}

	///*
	static std::string custompath = fs::current_path().generic_string();
	//custompath.resize(2048);
	//This is* always* sending the string as a path, even when the user is typing!that means it sending incomplete / invalid path names!
	//GRACEFULLY handle path names that are incomplete, and ONLY update the path when the user hits enter/control loses focus/
	//ImGuiInputTextFlags, ImGuiInputTextFlags_EnterReturnsTrue
	ImGuiInputTextFlags edtflgs = ImGuiInputTextFlags_EnterReturnsTrue;
	if (ImGui::InputText("Current Path", &custompath, edtflgs)) {
	};
	bool foc = ImGui::IsItemFocused();//Need more than one widget, otherwise this is always true
	//ImGui::Text("Focused: %u", foc);
	//ImGui::Text("entry: %s", custompath.c_str());
	// 
	//enumdircontents(custompath.c_str());
	fs::path strpath(custompath);
	//if (ImGui::TreeNode(strpath.stem().generic_string().c_str())) //Something here is triggering the window ID assert!
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, 260), true, window_flags);
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	ImGuiTreeNodeFlags treeflags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (ImGui::TreeNodeEx("Current:", treeflags))
	{
		recurdir(custompath.c_str());
		ImGui::TreePop();
	}
	ImGui::EndChild();

	/*
	for (auto const& dir_entry : fs::recursive_directory_iterator(custompath.c_str()))
	{
		//std::cout << dir_entry << '\n';
		if (fs::is_directory(dir_entry.status()))
		{
			ImGui::Text("wut: %s", dir_entry.path().generic_string().c_str());
		}
	}
	*/
	ImGui::Separator();
	//if (ImGui::InputText("Ignoreme", &custompath)) {};
	//ImGui::Text("Selected: %s", currentselection.c_str());
	//ImGuiInputTextFlags_ReadOnly
	ImGuiInputTextFlags readedtflgs = 0;
	readedtflgs |= ImGuiInputTextFlags_ReadOnly;
	ImGui::InputText("Selected:", &currentselection, readedtflgs);
	//bool select = false;
	//ImGui::Selectable(currentselection.c_str(), &select);
	//*/

	/*
	ImGui::OpenPopup("Delete?");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Delete?", p_open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static std::string custompath = fs::current_path().generic_string();
		//This is* always* sending the string as a path, even when the user is typing!that means it sending incomplete / invalid path names!
		//GRACEFULLY handle path names that are incomplete, and ONLY update the path when the user hits enter/control loses focus/
		//ImGuiInputTextFlags, ImGuiInputTextFlags_EnterReturnsTrue
		ImGuiInputTextFlags edtflgs = ImGuiInputTextFlags_EnterReturnsTrue;
		if (ImGui::InputText("Current", &custompath, edtflgs)) {
		};
		bool foc = ImGui::IsItemFocused();//Need more than one widget, otherwise this is always true
		ImGui::Text("Focused: %u", foc);
		//ImGui::Text("entry: %s", custompath.c_str());
		enumdircontents(custompath.c_str());
		ImGui::Separator();
		if (ImGui::InputText("Ignoreme", &custompath)) {};

		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }

		ImGui::EndPopup();
	}
	*/
	
	//end window
	ImGui::End();
}

void showdirbrowsermodal(bool* p_reopen, fs::path* pathout) {
	ImGuiPopupFlags popflags = 0;
	static std::string custompath = fs::current_path().generic_string();
	ImGuiInputTextFlags edtflgs = ImGuiInputTextFlags_EnterReturnsTrue;
	const char* browserdialogname = "Select Directory";

	fs::path strpath(custompath);
	if (*p_reopen) {
		ImGui::OpenPopup(browserdialogname);
	}

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	ImGui::SetNextWindowBgAlpha(1.f);
	if (ImGui::BeginPopupModal(browserdialogname, p_reopen, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::InputText("Current Path", &custompath, edtflgs)) {
		};

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, 260), true, window_flags);
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		ImGuiTreeNodeFlags treeflags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (ImGui::TreeNodeEx("Current:", treeflags))
		{
			recurdir(custompath.c_str());
			ImGui::TreePop();
		}
		ImGui::EndChild();

		ImGui::Separator();

		ImGuiInputTextFlags readedtflgs = 0;
		readedtflgs |= ImGuiInputTextFlags_ReadOnly;
		ImGui::InputText("Selected:", &currentselection, readedtflgs);

		if (ImGui::Button("OK", ImVec2(120, 0))) {
			pathout->make_preferred();
			*pathout = currentselection;
			ImGui::CloseCurrentPopup();  *p_reopen = false;
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); *p_reopen = false; }
		ImGui::EndPopup();
	}
}