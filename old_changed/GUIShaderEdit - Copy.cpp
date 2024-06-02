#include "GUIShaderEdit.hpp"

#include "imgui_stdlib.h"
#include "GUIFileBrowser.hpp"
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;
fs::path shaderfilein;
fs::path shaderfileout;
bool openshaderdlg = false;
bool saveshaderdlg = false;
bool writeoutfile = false;

static const char* rtBlitVS_source = R"(
#version 300 es
in vec4 rtVertTexClipPos;
out vec2 vTexCoords;
void main() {
	vTexCoords = rtVertTexClipPos.zw;
	gl_Position = vec4(rtVertTexClipPos.xy, 0.0, 1.0);
}
)";

class shaderobj {
public:
	shaderobj() {}
	~shaderobj() {}
	GLuint shaderid;
};

/*
GL already stores all the shaders created, all i actually need is the shader it returns when a shader is created...
Though adding additional data likea shader name or semantics, might be beneficial...
*/

class programwrap {
public:
	programwrap() {
		SDL_Log("__FUNCTION__");
	}
	~programwrap() { std::cout << __FUNCTION__ << "\n"; }
	//copy
	//move
	//copy assign
	//move assign
	std::string programname = "defprg";
	GLuint programid=0;
	unsigned short vsIndex = 0, fsindex = 0;//indices into shaders_w
};
std::vector<shaderwrap>shaders_w;
std::vector<programwrap>programs_w;

std::vector<shaderobj>shaders;
std::vector<GLint>glshaders;
std::vector<GLint>glprograms;

void programui() {
	static int progadded = 0;
	if (ImGui::Button("Add Program object")) {
		std::string tname = "program" + std::to_string(progadded);
		//auto& shader = shaders_w.emplace_back(tname, GL_VERTEX_SHADER);
		progadded++;
	}
}

void shadertabui() {

}

void addremoveui() {

}

void shadermanageui() {
	ImGui::Text("Add or remove shaders or programs");

	//Shaders
	static int added = 0;
	if (ImGui::Button("Add Shader object")) {
		std::string tname = "managed" + std::to_string(added);		
		auto& shader = shaders_w.emplace_back( tname, GL_VERTEX_SHADER);
		added++;
	}

	const char* emptystr = "Empty";
	static ImGuiComboFlags comboflags = 0;
	static auto item_current_idx = 0; // Here we store our selection data as an index.
	static std::string item_current_name = emptystr;
	if (!shaders_w.empty()) {
		const char* combo_preview_value = shaders_w.at(item_current_idx).shadername.c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Shaders", combo_preview_value, comboflags))
		{
			for (auto n = 0; n < shaders_w.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(shaders_w.at(n).shadername.c_str(), is_selected)) {
					item_current_idx = n;
					//item_current_name = shaders_w.at(n).shadername;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	else {
		ImGui::BeginDisabled(true);
		if (ImGui::BeginCombo("Shaders", "Empty", comboflags))
		{
			ImGui::EndCombo();
		}
		ImGui::EndDisabled();
	}

	//Programs

	ImGui::BeginDisabled(shaders_w.empty());

	ImGui::Text("Shader %s selected, index: %d", item_current_name.c_str(), item_current_idx);

	if (ImGui::Button("Delete Selected")) {
		if (shaders_w.size()) {
			auto itb = shaders_w.begin() + item_current_idx;
			if (itb != shaders_w.end()) {
				shaders_w.erase(itb);
				item_current_idx -= 1;
				if (item_current_idx < 0) item_current_idx = 0;
			}
		}
	}

	if (ImGui::Button("Clear shaders")) {
		shaders_w.clear();
		added = 0;
		item_current_idx = 0;
		item_current_name = emptystr;
	}

	//ImGui::Text(__FUNCTION__);
	ImGui::Text(item_current_name.c_str());

	ImGui::EndDisabled();

	static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	if (ImGui::BeginTable("table1", 3, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();
		for (int row = 0; row < shaders_w.size(); row++)
		{
			ImGui::TableNextRow();
			//for (int column = 0; column < 3; column++)
			{
				//ImGui::TableSetColumnIndex(column);
				//ImGui::Text("%s %d,%d", (column == 2) ? "Stretch" : "Fixed", column, row);
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s",shaders_w.at(row).shadername.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", shaders_w.at(row).shaderid);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%d", shaders_w.at(row).shadertype);
			}
		}
		ImGui::EndTable();
	}

	programui();
}

void shadereditui() {
	static ImGuiComboFlags comboflags = 0;
	static int item_current_idx = 0;
	const char* combo_preview_value = nullptr;
	if(shaders_w.size() >= 1)
		combo_preview_value = shaders_w[item_current_idx].shadername.c_str();
	else
		combo_preview_value = "Load shader";

	if (ImGui::BeginCombo("Shaders", combo_preview_value, comboflags))
	{
		int n = 0;
		const bool is_selected = (item_current_idx == n);
		for (auto& so : shaders_w)
		{
			if (ImGui::Selectable(so.shadername.c_str(), is_selected)) {
				item_current_idx = n;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
			n++;
		}
		ImGui::EndCombo();
	}
	//ImGui::Text("This is the Second shader edit tab!\nblah blah blah blah blah");
	if (ImGui::Button("Add Shader object")) {
		//shaderwrap so("dumbo", GL_VERTEX_SHADER);
		//so.shadername = "dumbo";
		//shaders_w.push_back(so);
		//shaders_w.erase(so);
	}
}

void programeditui() {
	ImGui::Text("This is the Program edit tab!\nblah blah blah blah blah");

	static ImGuiComboFlags comboflags = 0;

	if (ImGui::BeginCombo("Shaders", "preview", comboflags))
	{
		for (auto& so : shaders_w) {
			if (ImGui::Selectable(so.shadername.c_str(), false)) {

			}
		}
		ImGui::EndCombo();
	}
}

void showshaderdlg(bool* p_open) {
	ImGuiWindowFlags flags = 0;
	//flags |= ImGuiWindowFlags_NoResize;
	GLint len = 1;
	GLuint vshader = 0, fshader = 0, vf_program = 0;
	GLsizei logsize;
	static char log[1024] = { "Compile the text into shader source" };
	static ImGuiInputTextFlags txtflags = ImGuiInputTextFlags_None;// ImGuiInputTextFlags_AllowTabInput;
	static std::string shadsrc = "";

	ImGui::SetNextWindowBgAlpha(1.f);

	ImGui::Begin("Shader edit", p_open, flags);

	CLEARGLERR();
	CHECKGLERR();

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		///*
		if (ImGui::BeginTabItem("Manage"))
		{
			shadermanageui();
			ImGui::EndTabItem();
		}
		//*/

		if (ImGui::BeginTabItem("Shader 1"))
		{


	GLenum stype;
	static int item_current_2 = 0;
	ImGui::Combo("Shader Type:", &item_current_2, "GL_VERTEX_SHADER\0GL_FRAGMENT_SHADER\0\0");
	if (item_current_2 == 0) {
		ImGui::Text("Vertex shader selected:\t%d", item_current_2);
		stype = GL_VERTEX_SHADER;
	}
	else if (item_current_2 == 1) {
		ImGui::Text("Fragment shader selected:\t%d", item_current_2);
		stype = GL_FRAGMENT_SHADER;
	}

	if (ImGui::Button("Shader from file...")) {
		openshaderdlg ? (openshaderdlg = false) : (openshaderdlg = true);
	}
	if (openshaderdlg) {
		showfilepickerdlg(&openshaderdlg, &shaderfilein, true);
	}
	if (fs::exists(shaderfilein) && !fs::is_empty(shaderfilein)) {
		//shadsrc = shaderfilein.stem().generic_string();
		//shaderfilein.clear();

		///*
		if (std::ifstream is{shaderfilein, std::ios::binary | std::ios::ate}) {
			auto size = is.tellg();
			std::string str(size, '\0'); // construct string to stream size
			is.seekg(0);
			if (is.read(&str[0], size))
				std::cout << str << '\n';

			shadsrc = str;
		}
		//*/
		shaderfilein.clear();
	}

	//ImGui::Text("GL_ERROR:\t%s", lastglerrstr);
	ImGui::InputTextMultiline("##source", &shadsrc, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), txtflags);
	ImGui::Text(log);

	//ImGui::SameLine();
	if (ImGui::Button("Compile shader")) {
		const char* c_str = shadsrc.c_str();
		GLint loglen = 0;

		vshader = glCreateShader(stype);
		glShaderSource(vshader, 1, &c_str, NULL);
		glCompileShader(vshader);
		glGetShaderInfoLog(vshader, 1024, NULL, log);
		glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &loglen);
		if (log[0] == '\0') {
			sprintf(log, "Empty shader info log, reported: %d",loglen);
		}

		//vf_program = glCreateProgram();
		//glAttachShader(vf_program, vshader);
		//glAttachShader(vf_program, fshader);
		//glLinkProgram(vf_program);
	}

	if (ImGui::Button("Save shader to file")) {
		saveshaderdlg ? (saveshaderdlg = false) : (saveshaderdlg = true);
	}
	if (ImGui::Button("Add Shader object")) {
		//shaderwrap so("othertab", GL_VERTEX_SHADER);
		//shaders_w.push_back(so);
		//shaders_w.erase(so);
	}

	static ImGuiComboFlags comboflags = 0;
	if (ImGui::BeginCombo("Shaders", "preview", comboflags))
	{
		for (auto& so : shaders_w) {
			if (ImGui::Selectable(so.shadername.c_str(), false)) {

			}
		}
		ImGui::EndCombo();
	}

	if (saveshaderdlg) {
		showfilepickerdlg(&saveshaderdlg, &shaderfileout, false);
		if (!saveshaderdlg) {//once the dialog IS CLOSED...
			if (fs::exists(shaderfileout)) writeoutfile = true;
		}
	}
	if (fs::exists(shaderfileout) && writeoutfile) {
		//std::ofstream ostrm("Test.txt", std::ios::binary);
		std::ofstream ostrm(shaderfileout, std::ios::binary);
		//ostrm << "some text";
		ostrm << shadsrc;
		writeoutfile = false;
	}

	//ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
	ImGui::EndTabItem();
		}


	if (ImGui::BeginTabItem("Shader 2"))
	{
		//ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
		shadereditui();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Program"))
	{
		//ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
		programeditui();
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	}

	ImGui::End();
}
