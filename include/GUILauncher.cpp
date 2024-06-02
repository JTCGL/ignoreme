#include "GUILauncher.hpp"
#include "GUIDirectoryBrowser.hpp"
#include "GUIFileBrowser.hpp"
#include "GUIMessageDlg.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
bool showshadereditor = false;
bool showprogramlinker = false;
bool showdirectorybrowser = false;
bool showfilepicker = false;
bool showmsgdlg = false;
bool supressefuturemsgdlg = false;
std::filesystem::path rp = "";
std::filesystem::path filepicked = "";

void demofile(const char* filein) {
	assert(filein);
	std::filesystem::path fp(filein);
	if (std::filesystem::exists(fp)) {
		if (std::ifstream is{ fp, std::ios::binary | std::ios::ate }) {
			auto size = is.tellg();
			std::string str(size, '\0'); // construct string to stream size
			is.seekg(0);
			if (is.read(&str[0], size))
				std::cout << str << '\n';
			ImGui::Text("File contents: %s", str.c_str());
		}
	}
}

void demofilepath(fs::path pathin) {
	if (std::filesystem::exists(pathin)) {
		if (std::ifstream is{ pathin, std::ios::binary | std::ios::ate }) {
			auto size = is.tellg();
			std::string str(size, '\0');
			is.seekg(0);
			if (is.read(&str[0], size))
				std::cout << str << '\n';
			ImGui::Text("File contents: %s", str.c_str());
		}
	}
}

void showlauncher(bool* p_open) {
	ImGuiWindowFlags flags = 0;
	//flags |= ImGuiWindowFlags_NoResize;
	ImGuiStyle gstyle;
	gstyle = ImGui::GetStyle();
	//gstyle.
	ImGui::SetNextWindowBgAlpha(1.f);
	ImGui::Begin("Launcher", p_open, flags);

	//ImGui::Checkbox("Shader edit toggle", &toggleshaderedit);
	//ImGui::Text("grrrr");
	//ImGui::SetNextWindowBgAlpha(0.f);
	if (ImGui::Button("Shader Editor")) {
		//toggleshaderedit = toggleshaderedit ? true : false;
		showshadereditor ? (showshadereditor=false) : (showshadereditor=true);
	}
	if (ImGui::Button("Program Linker")) {
		showprogramlinker ? (showprogramlinker = false) : (showprogramlinker = true);
	}
	if (ImGui::Button("Directory Browser")) {
		showdirectorybrowser ? (showdirectorybrowser = false) : (showdirectorybrowser = true);
	}
	ImGui::Text("Folder picked: %s", rp.generic_string().c_str());
	if (ImGui::Button("File Picker")) {
		showfilepicker ? (showfilepicker = false) : (showfilepicker = true);
	}
	ImGui::Text("File picked: %s", filepicked.generic_string().c_str());

	if (ImGui::Button("Message Dialog")) {
		if (!supressefuturemsgdlg) {
			showmsgdlg ? (showmsgdlg = false) : (showmsgdlg = true);
		}
	}
	ImGui::Checkbox("Suppress Future invocations", &supressefuturemsgdlg);

	if (showshadereditor) {
		shaders_w.reserve(1500);
		showshaderdlg(&showshadereditor);
		if (!showshadereditor) {
			//ImGui::Text("shadererere picked: %d", showshadereditor);
		}
	}

	if (showmsgdlg && !supressefuturemsgdlg) {
		showmessagedlgex(&showmsgdlg, "messagebox title", "message main text",nullptr, iconseverity::ICOWARNING, &supressefuturemsgdlg, false);
	}

	if (showprogramlinker) {
		shaders_w.reserve(15);
		showshaderdlg(&showprogramlinker);
	}

	if (showdirectorybrowser) {
		//showdirbrowser(&showdirectorybrowser);
		showdirbrowsermodal(&showdirectorybrowser,&rp);
	}

	if (showfilepicker) {
		showfilepickerdlg(&showfilepicker, &filepicked,true);
		//only relevant for saving files
		if (!fs::exists(filepicked)) {
			std::ofstream{ filepicked };
		}
	}
	else {
		//demofile(filepicked.generic_string().c_str());
		//demofilepath(filepicked);
	}
	demofilepath(filepicked);

	ImGui::End();
}
