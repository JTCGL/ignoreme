#include "GUITestbed.hpp"

#include "imgui_stdlib.h"
#include "GUIFileBrowser.hpp"
#include <iostream>
#include <fstream>
#include <list>

struct namedentry {
	namedentry(const char* namein): name(namein) {}
	namedentry(std::string namein) : name(namein) {}
	const char* UIName() const { return name.c_str(); }
//private:
	std::string name;
};

std::list<namedentry>labels;

void makedata() {
	for (auto e = 0; e < 10; e++) {
		labels.emplace_back("label"+(std::to_string(e)));
	}
}

void cleandata() {
	labels.clear();
}

template <class T>
void lltest(const std::list<T>& ll) {
	auto tmp = ll.begin();
	for (auto t = 0; t < ll.size(); t++) {
		std::cout << "named: " << tmp->UIName() << "\n";
		std::advance(tmp, 1);
	}
}

template<class T>
int combofromlist(std::list<T>& list, const char* emptystr, const char* comboname, int first = 0) {
	ImGuiComboFlags comboflags = ImGuiComboFlags_None;
	comboflags |= ImGuiComboFlags_HeightLarge;
	auto item_current_idx = first;
	auto isEmpty = list.empty();
	auto lit = list.begin();
	auto lastit = lit;
	//auto fdsf = lit->UIName();

	lltest(list);

	ImGui::BeginDisabled(isEmpty);
	if (isEmpty) { comboflags |= ImGuiComboFlags_NoArrowButton; if (ImGui::BeginCombo(comboname, emptystr, comboflags)) { ImGui::EndCombo(); } }
	else {
		//const char* combo_preview_value = vec.at(item_current_idx).Name();
		//std::advance(lit, item_current_idx);
		const char* combo_preview_value = lastit->UIName();
		if (ImGui::BeginCombo(comboname, combo_preview_value, comboflags)) {
			
			
			//for(auto& e : list) {
			for (auto n = 0; n < list.size(); n++) {
				const bool is_selected = (item_current_idx == n);

				//https://en.cppreference.com/w/cpp/iterator/advance
				//auto lit = list.begin();
				//std::list<T>::iterator litt;
				//std::advance(lit, 0);
				//const char* nnn = (*lit)->UIName();


				if (ImGui::Selectable(lit->UIName(), is_selected)) {
					item_current_idx = n;
					lastit = lit;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

				std::advance(lit, 1);

				/*
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(vec.at(n).Name(), is_selected)) {
					item_current_idx = n;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
				*/
			}
			ImGui::EndCombo();
		}
	}
	ImGui::EndDisabled();
	return item_current_idx;
}

void showtestbeddlg(bool* p_open) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_None;
	//flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;
	//flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

	ImGui::SetNextWindowBgAlpha(1.f);

	ImGui::Begin("Logos", p_open, flags);

	if (ImGui::Button("Make data")) {
		makedata();
	}

	combofromlist(labels, "emptylabel", "combolabelname");

	/*
	for (auto& e : labels) {
		ImGui::Text(e.UIName());
	}
	*/
	
	ImGui::End();
	//cleandata();
}
