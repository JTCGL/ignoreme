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


/*
GL already stores all the shaders created, all i actually need is the shader it returns when a shader is created...
Though adding additional data like a shader name or semantics, might be beneficial...
*/
struct simpleglid {
	simpleglid() {}
	simpleglid(const char* namein) :name(namein) {};
	simpleglid(std::string strnamein) :name(strnamein) {};
	~simpleglid() {
		Release();
		name.clear();
	}
	void Release() {
		glDeleteShader(id);
	}
	std::string name = "";//human name of shader
	GLuint id = 0;//gl-owned id, 0 if not valid
};

std::vector<simpleglid>shaderids;
std::list<simpleglid>shaderids_ll;
std::vector<GLuint>shadersgl;

/*
* this is for those who know
*/

const char* namefromglid(const GLuint id) {
	//for (auto& sh : shaderids_ll) {	}
	auto fi = std::find_if(
		shaderids_ll.begin(), shaderids_ll.end(),
		[&id](auto& elem) {return elem.id == id; }
	);
	if (fi != shaderids_ll.end()) {
		return fi->name.c_str();
	}
	return nullptr;
}

GLuint glidfromname(const std::string namein) {
	auto fi = std::find_if(
		shaderids_ll.begin(), shaderids_ll.end(),
		[&namein](auto& elem) {return elem.name == namein; }
	);
	if (fi != shaderids_ll.end()) {
		return fi->id;
	}
	return 0;
}

void createglshader() {
	auto temp = glCreateShader(GL_VERTEX_SHADER);
	if (!shaderids_ll.empty()) {
		//not empty! is this shader already in one of the entries? Look through all of them and check...
		for (auto& sl : shaderids_ll) {
			if (sl.id == temp) {
				//this shader id was already stored! now what?
				break;
			}
		}
		//got this far, so no matching shader id was found - create a new entry
		//shaderids_ll.emplace_back("shaderll", temp);
	}
	else {
		//it is empty! add a simpleid, and assign this shaderid to it
		//shaderids_ll.emplace_back("shaderll", temp);
	}
}
void removeglshader(const GLuint shader) {
	//glDeleteShader(shader);
}
void deleteglshader(const GLuint shader) {
	glDeleteShader(shader);
}

void simpleidtest() {
	shaderids.resize(10);
	//add some ids
	GLuint id_to_find = 4;
	std::string name_to_find = "myname";

	//find shader when only id is known (glGetAttachedShaders)
	for (auto& s : shaderids) {
		if (s.id == id_to_find) {
			//do something with simpleid struct
		}
	}

	//find shader when only name is known
	for (auto& s : shaderids) {
		if (s.name == name_to_find) {
			//do something with simpleid struct
		}
	}
}

std::vector<glshader>shaders_w;
std::vector<programwrap>programs_w;
int lasprgindex = 0;
int lasshaderindex = 0;
const char* progbasename = "program";

std::string calciteratedname(const char* base, const int inc) {
	return base + std::to_string(inc);
}

template<class T>
int combofromvector(const std::vector<T>& vec, const char* emptystr, const char* comboname, int first = 0) {
	ImGuiComboFlags comboflags = ImGuiComboFlags_None;
	comboflags |= ImGuiComboFlags_HeightLarge;
	auto item_current_idx = first;
	auto isEmpty = vec.empty();
	ImGui::BeginDisabled(isEmpty);
	if (isEmpty) { comboflags |= ImGuiComboFlags_NoArrowButton; if (ImGui::BeginCombo(comboname, emptystr, comboflags)) { ImGui::EndCombo(); } }
	else {
		const char* combo_preview_value = vec.at(item_current_idx).Name();
		if (ImGui::BeginCombo(comboname, combo_preview_value, comboflags)) {
			for (auto n = 0; n < vec.size(); n++) {
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(vec.at(n).Name(), is_selected)) {
					item_current_idx = n;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::EndDisabled();
	return item_current_idx;
}

template<class T>
int combofromlinkedlist(const std::list<T>& ll, const char* emptystr, const char* comboname, int first = 0) {
	auto IsEmpty = ll.empty();
	for (auto& t : ll) {
		
	}
	return 0;
}

void programui() {
	static int progadded = 0;
	static std::string pnamez = calciteratedname(progbasename, progadded);
	if (ImGui::Button("Add Program")) {
		auto& prgadded = programs_w.emplace_back(pnamez);
		progadded++;
		pnamez = calciteratedname(progbasename, progadded);//Increment default name this AFTER the shader is added!
	}

	ImGui::SameLine();
	ImGuiInputTextFlags readedtflgs = ImGuiInputTextFlags_None;
	//readedtflgs |= ImGuiInputTextFlags_ReadOnly;
	ImGui::InputText("##ProgramName", &pnamez, readedtflgs);

	//ImGui::SameLine();

	static auto lastprgidx = 0;
	lastprgidx = combofromvector(programs_w,"Empty vector, add a program","Programs", lastprgidx);

	if (ImGui::Button("Delete Program")) {
		if (programs_w.size()) {
			auto itb = programs_w.begin() + lastprgidx;
			if (itb != programs_w.end()) {
				programs_w.erase(itb);
				lastprgidx -= 1;
				if (lastprgidx < 0) lastprgidx = 0;
			}
		}
	}

	if (ImGui::Button("Clear All Programs")) {
		programs_w.clear();
		progadded = 0;
		lastprgidx = 0;
		pnamez = calciteratedname(progbasename, progadded);
	}

	static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	if (ImGui::BeginTable("programtable", 4, flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Vertex Shader", ImGuiTableColumnFlags_WidthFixed);//ImGuiTableColumnFlags_WidthStretch
		ImGui::TableSetupColumn("Fragment Shader", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();
		for (int row = 0; row < programs_w.size(); row++)
		{
			ImGui::TableNextRow();
			//for (int column = 0; column < 3; column++)
			{
				std::string vsname = "Unassigned", fsname = "Unassigned";
				auto vsid = 0, fsid = 0;
				auto vsidx = programs_w.at(row).vsIndex;
				auto fsidx = programs_w.at(row).fsindex;
				try {
					if (vsidx >= 0) {
						vsname = shaders_w.at(vsidx).Name();
						vsid = shaders_w.at(vsidx).Id();
					}
				}
				catch (std::out_of_range const& exc) {
					//std::cout << exc.what() << '\n';
					SDL_Log("vert shader out of range: %s\n",exc.what());
					programs_w.at(row).vsIndex = -1;
				}

				try {
					if (fsidx >= 0) {
						fsname = shaders_w.at(fsidx).Name();
						fsid = shaders_w.at(fsidx).Id();
					}
				}
				catch (std::out_of_range const& exc) {
					//std::cout << exc.what() << '\n';
					SDL_Log("frag shader out of range: %s\n", exc.what());
					programs_w.at(row).fsindex = -1;
				}


				//ImGui::TableSetColumnIndex(column);
				//ImGui::Text("%s %d,%d", (column == 2) ? "Stretch" : "Fixed", column, row);
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", programs_w.at(row).programname.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", programs_w.at(row).programid);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%d %s(gl id:%u)", vsidx, vsname.c_str(), vsid);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%d %s(gl id:%u)", fsidx, fsname.c_str(), fsid);
			}
		}
		ImGui::EndTable();
	}
}

void addremoveui() {

}

void shadermanageui() {
	const char* shaderbasename = "shader";
	static GLenum stype = GL_VERTEX_SHADER;
	//static std::string snamez = "shader" + std::to_string(0);//"<Shader Name>";
	static std::string snamez = calciteratedname(shaderbasename, 0);

	ImGui::Text("Add or remove shaders or programs");

	//Shaders
	static int added = 0;
	if (ImGui::Button("Add Shader", ImVec2{0,0})) {
		auto& shader = shaders_w.emplace_back( snamez, stype);
		added++;
		snamez = calciteratedname(shaderbasename, added);//Increment default name this AFTER the shader is added!
	}
	ImGui::SameLine();
	ImGuiInputTextFlags readedtflgs = ImGuiInputTextFlags_None;
	//readedtflgs |= ImGuiInputTextFlags_ReadOnly;
	ImGui::InputText("##ShaderName", &snamez, readedtflgs);

	ImGui::SameLine();
	
	static int item_current_2 = 0;
	ImGui::Combo("##ShaderType:", &item_current_2, "GL_VERTEX_SHADER\0GL_FRAGMENT_SHADER\0\0");
	if (item_current_2 == 0) {
		//ImGui::Text("Vertex shader selected:\t%d", item_current_2);
		stype = GL_VERTEX_SHADER;
	}
	else {
		stype = GL_FRAGMENT_SHADER;
	}

	const char* emptystr = "Empty";
	static ImGuiComboFlags comboflags = 0;
	static auto item_current_idx = 0; // Here we store our selection data as an index.
	static std::string item_current_name = emptystr;
	if (!shaders_w.empty()) {
		const char* combo_preview_value = shaders_w.at(item_current_idx).Name();  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Shaders", combo_preview_value, comboflags))
		{
			for (auto n = 0; n < shaders_w.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				//bool dis = (shaders_w.at(n).shadertype == GL_VERTEX_SHADER);
				//ImGui::BeginDisabled(dis);
				if (ImGui::Selectable(shaders_w.at(n).Name(), is_selected)) {
					item_current_idx = n;
					//item_current_name = shaders_w.at(n).shadername;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
				//ImGui::EndDisabled();
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

	ImGui::BeginDisabled(shaders_w.empty());

	ImGui::Text("Shader %s selected, index: %d", item_current_name.c_str(), item_current_idx);

	if (ImGui::Button("Delete Shader")) {
		if (shaders_w.size()) {
			auto itb = shaders_w.begin() + item_current_idx;
			if (itb != shaders_w.end()) {

				/*
				Go through all the programs(!!) and see if this shader is in use, matching by shader name
				if it is in use, have the program remove it accordingly, and only delete it from the vector afterwards. Do NOT remove a shader that is still attached/in use!
				*/
				for (auto& p : programs_w) {
					//see if this program has an attached shader with the same name as the one being deleted
				}

				//store a var in the shader struct to indicate ownership, any ownership, by a program, and only delete it if no owners
				shaders_w.erase(itb);
				item_current_idx -= 1;
				if (item_current_idx < 0) item_current_idx = 0;
			}
		}
	}

	if (ImGui::Button("Clear All Shaders")) {
		shaders_w.clear();
		added = 0;
		item_current_idx = 0;
		item_current_name = emptystr;
	}

	//ImGui::Text(__FUNCTION__);
	ImGui::Text(item_current_name.c_str());

	ImGui::EndDisabled();

	static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	if (ImGui::BeginTable("shadertable", 3, flags))
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
				ImGui::Text("%s",shaders_w.at(row).Name());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", shaders_w.at(row).Id());
				ImGui::TableSetColumnIndex(2);
				std::string shadercolumnlabel = "INVALID";
				switch (shaders_w.at(row).Type()) {
					case GL_VERTEX_SHADER: shadercolumnlabel = "Vertex Shader"; break;
					case GL_FRAGMENT_SHADER: shadercolumnlabel = "Fragment Shader"; break;
				}
				ImGui::Text("%s", shadercolumnlabel.c_str());
			}
		}
		ImGui::EndTable();
	}

	programui();
}

void shadertabui() {
	static auto lastvs = 0;
	static ImGuiInputTextFlags txtflags = ImGuiInputTextFlags_None;
	static std::string shadsrc = "";
	GLint len = 1;
	GLsizei logsize;
	static char log[1024] = { "Compile the text into shader source" };
	static auto lastshaderid = 0;
	bool empty = shaders_w.empty();
	auto shader = 0;
	ImGui::BeginDisabled(shaders_w.empty());
	lastvs = combofromvector(shaders_w, "Empty", "Shader", lastvs);
	if (!empty)
	//if(!shaders_w.empty())
	{
		shader = shaders_w.at(lastvs).Id();
		if (shader != lastshaderid) {
			lastshaderid = shader;
			GLint srclen = 0;
			glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &srclen);
			if (srclen) {
				GLsizei rtnlen = 0;
				//GLchar rtnsrc[srclen];
				shadsrc.reserve(srclen);
				glGetShaderSource(shader, srclen, &rtnlen, &shadsrc[0]);
			}
			else {
				shadsrc = "";
			}
		}
	}

	ImGui::InputTextMultiline("##source", &shadsrc, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), txtflags);

	/*
	GL shader creation needs to be separated from UI code! Make the following standalone, so that it may be called from anywhere. Fundamentally, glshadersource just needs the text string for the shader, whether its from the UI or a file on disk
	*/

	if (ImGui::Button("Compile shader")) {
		assert(shader);
		auto& ts = shaders_w.at(lastvs);
		ts.Compile(shadsrc);
		/*
		assert(shader);
		//if (shader) {
			const char* c_str = shadsrc.c_str();
			GLint loglen = 0;
			glShaderSource(shader, 1, &c_str, NULL);
			glCompileShader(shader);
			glGetShaderInfoLog(shader, 1024, NULL, log);
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglen);
			if (log[0] == '\0') {
				sprintf(log, "Empty shader info log, reported: %d", loglen);
				SDL_Log("%s", log);
			}
		//}
		*/
	}
	ImGui::EndDisabled();

	/*
	* have 2 code views; one for "input" shader code from an author, and another to display the 'compiled' source code stored with the shader accessed via glGet**
	*/
}

void programeditui() {
	//ImGui::Text("This is the Program edit tab!\nblah blah blah blah blah");
	bool emptyprg = programs_w.empty();
	bool emptyshaders = shaders_w.empty();
	static auto lastprg = 0, lastvs = 0, lastfs = 0;
	static std::string lastlink="Link a program to see link status";
	ImGui::BeginDisabled(emptyprg);
	lastprg = combofromvector(programs_w, "Empty", "Programz", lastprg);
	lastvs = combofromvector(shaders_w, "Empty", "Vert", lastvs);
	lastfs = combofromvector(shaders_w, "Empty", "Frag", lastfs);	
	if (ImGui::Button("Link Program")) {
		programs_w.at(lastprg).vsIndex = lastvs;
		programs_w.at(lastprg).fsindex = lastfs;

		if (!emptyshaders) {
			try {
				CLEARGLERR();
				glAttachShader(programs_w.at(lastprg).programid, shaders_w.at(lastvs).Id());
				CHECKGLERR();
			}
			catch (const std::out_of_range& e) {
				SDL_Log("shaders_w VS: %s", e.what());
			}
			try {
				CLEARGLERR();
				//glAttachShader(programs_w.at(lastprg).programid, shaders_w.at(lastvs).Id());
				glAttachShader(programs_w.at(lastprg).programid, shaders_w.at(lastfs).Id());
				//glLinkProgram(programs_w.at(lastprg).programid);
				//CLEARGLERR();
				CHECKGLERR();
				//lastlink = lastglerrstr;
			}
			catch (const std::out_of_range& e) {
				SDL_Log("shaders_w: FS %s", e.what());
			}

			CLEARGLERR();
			glLinkProgram(programs_w.at(lastprg).programid);
			CHECKGLERR();
			lastlink = lastglerrstr;
		}
	}
	GLsizei bs = 0;
	GLchar buf[1024] = { 0 };
	if (!emptyprg)
	{
		glGetProgramInfoLog(programs_w.at(lastprg).programid, 1024, &bs, (GLchar*)&buf);
	}
	//SDL_Log("len: %d %s", bs, buf);
	ImGui::Text("Link status: %s", lastlink.c_str());
	ImGui::Text("Log len: %d\n%s", bs, buf);
	ImGui::EndDisabled();
}

void programvisui() {
	bool emptyprg = programs_w.empty();
	static auto lastprg = 0;
	ImGui::BeginDisabled(emptyprg);
	lastprg = combofromvector(programs_w, "Empty", "Programz", lastprg);
	if (!emptyprg) {
		auto prgid = programs_w.at(lastprg).programid;
		assert(prgid);
		GLint linked = GL_FALSE;
		GLint numactiveuniforms = 0;
		GLint activeuniformmaxlen = 0;
		glGetProgramiv(prgid, GL_LINK_STATUS, &linked);
		if (linked) {
			glGetProgramiv(prgid, GL_ACTIVE_UNIFORMS, &numactiveuniforms);
			glGetProgramiv(prgid, GL_ACTIVE_UNIFORM_MAX_LENGTH, &activeuniformmaxlen);
			for (auto u = 0; u < numactiveuniforms; u++) {
				GLsizei written = 0;
				GLint size = 0;
				GLenum type;
				std::string name;
				name.reserve(activeuniformmaxlen);
				glGetActiveUniform(prgid, u, activeuniformmaxlen, &written, &size, &type, &name[0]);
				ImGui::Text("Uniform %d: Type: %s %s", u, gl_enum_to_char(type), name.c_str());
			}
		}
		//ImGui::Text("Active uniforms for program %d : %d", prgid, numactiveuniforms);
		ImGui::Text("Active uniforms for program %s : %d", programs_w.at(lastprg).Name(), numactiveuniforms);
	}
	ImGui::EndDisabled();
}

void showshaderdlg(bool* p_open) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_None;
	//flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;
	//flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

	ImGui::SetNextWindowBgAlpha(1.f);

	ImGui::Begin("Programs and Shaders", p_open, flags);

	CLEARGLERR();
	CHECKGLERR();

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	tab_bar_flags |= ImGuiTabBarFlags_Reorderable;
	if (ImGui::BeginTabBar("ShadersTabBar", tab_bar_flags)) {
		if (ImGui::BeginTabItem("Manage")) {
			shadermanageui();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Shader")) {
			shadertabui();
			ImGui::EndTabItem();
		}
		/*
		if (ImGui::BeginTabItem("Fragment")) {
			shadertabui();
			ImGui::EndTabItem();
		}
		*/
		if (ImGui::BeginTabItem("Program")) {
			programeditui();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Vis Params")) {
			//enumerate and layout all uniforms for a program as color/slider/UI widgets - the fun stuff!
			programvisui();
			ImGui::EndTabItem();
		}
		/*
		if (ImGui::BeginTabItem("Old UI")) {
			old_shadertab_ui();
			ImGui::EndTabItem();
		}
		*/
		ImGui::EndTabBar();
	}
	ImGui::End();
}
