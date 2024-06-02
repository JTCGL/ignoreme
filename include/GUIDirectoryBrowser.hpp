#ifndef GUIDIRECTORYBROWSER_HPP
#define GUIDIRECTORYBROWSER_HPP
#pragma once

#include "platform.hpp"
#include <filesystem>

extern void showdirbrowser(bool* p_open);
extern void showdirbrowsermodal(bool* p_reopen, std::filesystem::path* pathout);

#endif