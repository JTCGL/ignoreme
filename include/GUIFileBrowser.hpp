#ifndef GUIFILEBROWSER_HPP
#define GUIFILEBROWSER_HPP
#pragma once

#include "platform.hpp"
#include <filesystem>

extern void showfilepickerdlg(bool* p_reopen, std::filesystem::path* pathout, bool open);

#endif