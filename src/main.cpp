/*
 * Copyright 2015 Miodrag Milanovic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-3-Clause
 */

#include "common.h"
#include <bgfx.h>
#include <bx/uint32_t.h>
#include "imgui/imgui.h"

void displayMainMenu()
{
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

	bool opened_left = true;
	bool opened_right = true;

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
		);
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
		
		ImGui::SetNextWindowPos(ImVec2(0, 22));
		//ImGuiStyle& style = ImGui::GetStyle(); // this struct has the colors
		//style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
		//style.Colors[ImGuiCol_MenuBarBg] = ImColor(0, 0, 255);
		if (ImGui::Begin("Left Panel", &opened_left, ImVec2(width / 2, height - 22), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowSize().y));
			ImGui::Columns(4);
			ImGui::Text("Name"); ImGui::NextColumn();
			ImGui::Text("Ext"); ImGui::NextColumn();
			ImGui::Text("Size"); ImGui::NextColumn();
			ImGui::Text("Date"); ImGui::NextColumn();
			ImGui::Separator();
			for (int i = 0; i < 15; i++)
			{
				ImGui::Text("%04d: col1", i);
				ImGui::NextColumn();
				ImGui::Text("%04d: col2", i);
				ImGui::NextColumn();
				ImGui::Text("%04d: col3", i);
				ImGui::NextColumn();
				ImGui::Text("%04d: Col4", i);
				ImGui::NextColumn();
			}
			ImGui::EndChild();

			ImGui::End();
		}
		ImGui::SetNextWindowPos(ImVec2(width / 2, 22));
		if (ImGui::Begin("Right Panel", &opened_right, ImVec2(width / 2, height - 22), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		{
			//ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
			ImGui::BeginChild("Sub2", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowSize().y));
			ImGui::Columns(4);
			ImGui::Text("Name"); ImGui::NextColumn();
			ImGui::Text("Ext"); ImGui::NextColumn();
			ImGui::Text("Size"); ImGui::NextColumn();
			ImGui::Text("Date"); ImGui::NextColumn();
			ImGui::Separator();
			for (int i = 0; i < 20; i++)
			{
				ImGui::Text("%04d: col1", i);
				ImGui::NextColumn();
				ImGui::Text("%04d: col2", i);
				ImGui::NextColumn();
				ImGui::Text("%04d: col3", i);
				ImGui::NextColumn();
				ImGui::Text("%04d: Col4", i);
				ImGui::NextColumn();
			}
			ImGui::SetScrollPosHere();
			ImGui::EndChild();
			//ImGui::PopStyleVar();
			ImGui::End();
		}
		imguiEndFrame();

		bgfx::submit(0);

		bgfx::frame();
	}

	// Cleanup.
	imguiDestroy();

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}
