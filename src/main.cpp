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
	while (!entry::processEvents(width, height, debug, reset, &mouseState))
	{
		imguiBeginFrame(mouseState.m_mx
			, mouseState.m_my
			, (mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
			| (mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
			, 0
			, width
			, height
			);
		
		displayMainMenu();

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
