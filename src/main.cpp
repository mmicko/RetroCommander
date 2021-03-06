/*
 * Copyright 2015 Miodrag Milanovic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-3-Clause
 */

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <bgfx/bgfx.h>
#include <bx/uint32_t.h>
#include "imgui/imgui.h"
#include "cmd.h"
#include "input.h"
#include <dirent.h>
#include <vector>
#include <sys/stat.h>
#include <time.h>
#include <direct.h>

void displayMainMenu()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.56f, 0.56f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.00f, 0.25f, 0.25f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.00f, 0.25f, 0.25f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.00f, 0.25f, 0.25f, 1.00f));

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Left"))
		{
			if (ImGui::MenuItem("Brief", "CTRL+1")) {}
			if (ImGui::MenuItem("Medium", "CTRL+2")) {}
			if (ImGui::MenuItem("Two columns", "CTRL+3")) {}
			if (ImGui::MenuItem("Full (name)", "CTRL+4")) {}
			if (ImGui::MenuItem("Full (size, time)", "CTRL+5")) {}
			if (ImGui::MenuItem("Full (access)", "CTRL+6")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Sort mode"))
			{
				ImGui::MenuItem("Name");
				ImGui::MenuItem("Extension");
				ImGui::MenuItem("Modif. Time");
				ImGui::MenuItem("Size");
				ImGui::MenuItem("Unsorted");
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Change source")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Files"))
		{
			if (ImGui::MenuItem("User menu", "F2")) {}
			if (ImGui::MenuItem("View", "F3")) {}
			if (ImGui::MenuItem("Edit", "F4")) {}
			if (ImGui::MenuItem("Copy", "F5")) {}
			if (ImGui::MenuItem("Rename or move", "F6")) {}
			if (ImGui::MenuItem("Make directory", "F7")) {}
			if (ImGui::MenuItem("Delete", "F8")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("File attributes", "CTRL+A")) {}
			if (ImGui::MenuItem("Apply command", "CTRL+G")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Select group")) {}
			if (ImGui::MenuItem("Unselect group")) {}
			if (ImGui::MenuItem("Invert selection")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Commands"))
		{
			if (ImGui::MenuItem("Find file", "ALT+F7")) {}
			if (ImGui::MenuItem("History", "ALT+F8")) {}
			if (ImGui::MenuItem("Maximize window", "ALT+F9")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Panel on/off", "CTRL+O")) {}
			if (ImGui::MenuItem("Equal panels", "CTRL+=")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Settings")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Right"))
		{
			if (ImGui::MenuItem("Brief", "CTRL+1")) {}
			if (ImGui::MenuItem("Medium", "CTRL+2")) {}
			if (ImGui::MenuItem("Two columns", "CTRL+3")) {}
			if (ImGui::MenuItem("Full (name)", "CTRL+4")) {}
			if (ImGui::MenuItem("Full (size, time)", "CTRL+5")) {}
			if (ImGui::MenuItem("Full (access)", "CTRL+6")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Sort mode"))
			{
				ImGui::MenuItem("Name");
				ImGui::MenuItem("Extension");
				ImGui::MenuItem("Modif. Time");
				ImGui::MenuItem("Size");
				ImGui::MenuItem("Unsorted");
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Change source")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

static const InputBinding s_bindings[] =
{
	{ entry::Key::F10,   entry::Modifier::None, 1, NULL, "exit" },
	{ entry::Key::F9,    entry::Modifier::None, 1, NULL, "menu" },
	INPUT_BINDING_END
};

int cmdMenu(CmdContext* /*_context*/, void* /*_userData*/, int /*_argc*/, char const* const* /*_argv*/)
{
	return 0;
}

struct file_descriptor
{
    std::string name;
	std::string path;
    bool is_dir;
    bool is_reg;
    size_t size;
    std::string datetime;

    // TODO: separate "view data" (selected) from "underlying data" (files)
    bool is_selected;
};


int strreplace(std::string &str, const std::string& search, const std::string& replace)
{
	int searchlen = search.length();
	int replacelen = replace.length();
	int matches = 0;

	for (int curindex = str.find(search, 0); curindex != -1; curindex = str.find(search, curindex + replacelen))
	{
		matches++;
		str.erase(curindex, searchlen).insert(curindex, replace);
	}
	return matches;
}

std::vector<file_descriptor> files;
std::string  fullpath;

void readFiles(const char *path)
{
	files.clear();
	DIR* directory = opendir(path);
	if (directory != NULL)
	{
		struct dirent* dirent;
		struct stat stat_info;
		fullpath = std::string(path) + "\\";
		while ((dirent = readdir(directory)) != NULL){
			file_descriptor file;
			file.size = 0;
			file.is_dir = false;
			file.is_reg = false;
			file.name = std::string(dirent->d_name);

			file.path = fullpath + file.name;
			int s = stat(file.path.c_str(), &stat_info);
			if (s != -1)
			{
				if (S_ISREG(stat_info.st_mode)){
					file.is_reg = true;
				}
				if (S_ISDIR(stat_info.st_mode)){
					file.is_dir = true;
				}
				file.size = stat_info.st_size;
				char datetime[100];
				const struct tm* time = localtime(&stat_info.st_mtime);
				strftime(datetime, 100, "%c", time);
				file.datetime = std::string(datetime);
			}
			file.is_selected = false;
			files.push_back(file);
		}
	}

	closedir(directory);
}

void displayPanel()
{
    ImVec2 box0 = ImGui::GetWindowContentRegionMax();
    box0.y = 15;
    box0.x -= 5;
    float loc[4];
    ImGui::BeginChild("Sub0", box0, false);
    ImGui::Columns(4);
    ImGui::Text("Name"); loc[0] = ImGui::GetColumnOffset(); ImGui::NextColumn();
    ImGui::Text("Ext");  loc[1] = ImGui::GetColumnOffset(); ImGui::NextColumn();
    ImGui::Text("Size"); loc[2] = ImGui::GetColumnOffset(); ImGui::NextColumn();
    ImGui::Text("Date"); loc[3] = ImGui::GetColumnOffset(); ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::EndChild();
    ImGui::Separator();

    ImVec2 box = ImGui::GetWindowContentRegionMax();
    box.y -= 60 + 35;
    box.x -= 5;
    ImGui::BeginChild("Sub1", box, false);
    ImGui::Columns(4);
    ImGui::SetColumnOffset(0,loc[0]);
    ImGui::SetColumnOffset(1,loc[1]);
    ImGui::SetColumnOffset(2,loc[2]);
    ImGui::SetColumnOffset(3,loc[3]);

    std::string open_dir;

	for (auto it = files.begin(); it != files.end(); ++it)
    {
        file_descriptor* fd = &(*it);

        char buf[100];
        sprintf(buf, "%s", fd->name.c_str());

		bool clicked = false;// ImGui::SelectableAllColumns(buf, fd->is_selected);
        bool hovered = ImGui::IsItemHovered();
		if (clicked && ImGui::GetIO().KeyCtrl)
        {
            fd->is_selected ^= 1;
        }
		else if (hovered && ImGui::IsMouseDoubleClicked(0))
		{
			if ((*it).is_dir) {
				char full[_MAX_PATH];
				open_dir = fullpath + fd->name;
				_fullpath(full, open_dir.c_str(), _MAX_PATH);
				open_dir = std::string(full);
			}
		}
        else if (clicked)
        {
            for (std::vector<file_descriptor>::iterator it2 = files.begin(); it2 != files.end(); ++it2)
                (*it2).is_selected = false;
            fd->is_selected = true;			
			/*if ((*it).is_dir) {
				fullpath += fd->name.c_str();
				readFiles(fullpath.c_str());
			}*/
			//break;
        }
		ImGui::Text(buf);
        ImGui::NextColumn();

        ImGui::Text(""); ImGui::NextColumn();
        
        if ((*it).is_dir) {
            ImGui::Text("<DIR>");  ImGui::NextColumn();
        }
        else {
            ImGui::Text("%d", (*it).size);  ImGui::NextColumn();
        }
        ImGui::Text("%s", (*it).datetime.c_str());  ImGui::NextColumn();
    }
    ImGui::EndChild();

    ImVec2 box2 = ImGui::GetWindowContentRegionMax();
    box2.y = 50;
    box2.x -= 5;

    ImGui::BeginChild("Sub3", box2, false);
    ImGui::Columns(4);
    ImGui::Text("Name");  ImGui::NextColumn();
    ImGui::Text("Ext");   ImGui::NextColumn();
    ImGui::Text("Size");  ImGui::NextColumn();
    ImGui::Text("Date");  ImGui::NextColumn();

    ImGui::Text("Name");  ImGui::NextColumn();
    ImGui::Text("Ext");   ImGui::NextColumn();
    ImGui::Text("Size");  ImGui::NextColumn();
    ImGui::Text("Date");  ImGui::NextColumn();
    ImGui::Text("Name");  ImGui::NextColumn();
    ImGui::Text("Ext");   ImGui::NextColumn();
    ImGui::Text("Size");  ImGui::NextColumn();
    ImGui::Text("Date");  ImGui::NextColumn();

    ImGui::Columns(1);
    ImGui::EndChild();
    
    // process open directory after we are done iterating our files for the frame
    if (!open_dir.empty())
        readFiles(open_dir.c_str());
}

int _main_(int /*_argc*/, char** /*_argv*/)
{
	uint32_t width = 1280;
	uint32_t height = 720;
	uint32_t debug = BGFX_DEBUG_TEXT;
	uint32_t reset = BGFX_RESET_VSYNC;

	bgfx::init();
	bgfx::reset(width, height, reset);

	// Enable debug text.
	bgfx::setDebug(debug);

	imguiCreate();

	entry::MouseState mouseState;

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
		);

	ImGuiStyle& style = ImGui::GetStyle(); // this struct has the colors
	style.WindowRounding = 0.0f;
	style.FrameRounding = 0.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);   // Background of checkbox, radio button, plot, slider, text input
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.50f, 0.50f, 1.00f, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.56f, 0.56f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.40f, 0.80f, 0.15f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.67f, 0.40f, 0.40f, 0.60f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.40f, 0.40f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);



	inputRemoveBindings("bindings");
	inputAddBindings("bindings", s_bindings);

	cmdAdd("menu", cmdMenu);
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	readFiles(cwd);

	while (!entry::processEvents(width, height, debug, reset, &mouseState))
	{
		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, width, height);

		imguiBeginFrame(mouseState.m_mx
			, mouseState.m_my
			, (mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
			| (mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
			, mouseState.m_mz
			, width
			, height
			);

		displayMainMenu();
		ImGui::SetNextWindowPos(ImVec2(0, 19));
		ImGui::SetNextWindowSize(ImVec2(width / 2, height - 19));
		if (ImGui::Begin(fullpath.c_str(), NULL, ImVec2(width / 2, height - 19), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
            displayPanel();
        ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(width / 2, 19));
		ImGui::SetNextWindowSize(ImVec2(width / 2, height - 19));
		if (ImGui::Begin("Right Panel", NULL, ImVec2(width / 2, height - 19), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
            displayPanel();
        ImGui::End();

        // imgui test code
        //ImGui::ShowTestWindow(NULL);

		imguiEndFrame();

		//bgfx::submit(0, NULL, 0);

		bgfx::frame();
	}

	// Cleanup.
	imguiDestroy();

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}
