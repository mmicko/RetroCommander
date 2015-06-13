/*
 * Copyright 2015 Miodrag Milanovic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-3-Clause
 */

#include <stdio.h>
#include "common.h"
#include <bgfx.h>
#include <bx/uint32_t.h>
#include "imgui/imgui.h"
#include "cmd.h"
#include "input.h"

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
	bool opened = true;

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
	
	bool border = false;

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
		if (ImGui::Begin("Left Panel", &opened_left, ImVec2(width / 2, height - 19), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
		{
			ImVec2 box0 = ImGui::GetWindowContentRegionMax();
			box0.y = 15;
			box0.x -= 5;
			ImGui::BeginChild("Sub0", box0, border);
			ImGui::Columns(4);
			ImGui::Text("Name");  ImGui::NextColumn();
			ImGui::Text("Ext");   ImGui::NextColumn();
			ImGui::Text("Size");  ImGui::NextColumn();
			ImGui::Text("Date");  ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::EndChild();

			ImVec2 box = ImGui::GetWindowContentRegionMax();
			box.y -= 60 + 35;
			box.x -= 5;
			ImGui::BeginChild("Sub1", box, border);

			for (int i = 0; i < 150; i++)
			{
				static bool selected[3] = { false, false, false };
				char buf[100];
				sprintf(buf, "file_%d.c     ", i);
				ImGui::Selectable(buf, &selected[0]);
			}
			ImGui::EndChild();

			ImVec2 box2 = ImGui::GetWindowContentRegionMax();
			box2.y = 50;
			box2.x -= 5;
			
			ImGui::BeginChild("Sub3", box2, border);
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

			ImGui::End();
		}
		ImGui::SetNextWindowPos(ImVec2(width / 2, 19));
		ImGui::SetNextWindowSize(ImVec2(width / 2, height - 19));
		if (ImGui::Begin("Right Panel", &opened_right, ImVec2(width / 2, height - 19), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
		{
			ImVec2 box0 = ImGui::GetWindowContentRegionMax();
			box0.y = 15;
			box0.x -= 5;
			ImGui::BeginChild("RSub0", box0, border);
			ImGui::Columns(4);
			ImGui::Text("Name");  ImGui::NextColumn();
			ImGui::Text("Ext");   ImGui::NextColumn();
			ImGui::Text("Size");  ImGui::NextColumn();
			ImGui::Text("Date");  ImGui::NextColumn();
			ImGui::Columns(1);
			ImGui::EndChild();

			ImVec2 box = ImGui::GetWindowContentRegionMax();
			box.y -= 60 + 35;
			box.x -= 5;
			ImGui::BeginChild("RSub1", box, border);

			for (int i = 0; i < 150; i++)
			{
				static bool selected[3] = { false, false, false };
				char buf[100];
				sprintf(buf, "file_%d.c     ", i);
				ImGui::Selectable(buf, &selected[0]);
			}
			ImGui::EndChild();

			ImVec2 box2 = ImGui::GetWindowContentRegionMax();
			box2.y = 50;
			box2.x -= 5;

			ImGui::BeginChild("RSub3", box2, border);
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
