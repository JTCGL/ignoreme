#ifndef GUIMESSAGEDLG_HPP
#define GUIMESSAGEDLG_HPP
#pragma once

#include "platform.hpp"
#include <filesystem>

enum class iconseverity {
	ICONONE = 0,
	ICOINFORM,
	ICOWARNING,
	ICOERROR,
	ICOQUERY
};

extern void showmessagedlg(bool* p_reopen, const char* title, const char* message);
extern void showmessagedlgex(bool* p_reopen, const char* title, const char* message, const char* explain, const iconseverity ico, bool* suppress, bool showsupress);

#endif