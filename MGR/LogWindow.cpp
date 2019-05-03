#pragma once
#include "LogWindow.h"
#include "GlobalVariables.h"

namespace AppLog {
	//-----------------------------------------------------------------------------
	// [SECTION] Example App: Debug Log / ShowExampleAppLog()
	//-----------------------------------------------------------------------------

	// Usage:
	//  static ExampleAppLog my_log;
	//  my_log.AddLog("Hello %d world\n", 123);
	//  my_log.Draw("title");
	struct ExampleAppLog
	{
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool                ScrollToBottom;

		ExampleAppLog()
		{
			ScrollToBottom = false;
			Clear();
		}

		void    Clear()
		{
			Buf.clear();
			LineOffsets.clear();
			LineOffsets.push_back(0);
		}

		void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			int old_size = Buf.size();
			va_list args;
			va_start(args, fmt);
			Buf.appendfv(fmt, args);
			va_end(args);
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
				if (Buf[old_size] == '\n')
					LineOffsets.push_back(old_size + 1);
			ScrollToBottom = true;
		}

		void    Draw(const char* title, bool* p_open = NULL)
		{
			if (!ImGui::Begin(title, p_open))
			{
				ImGui::End();
				return;
			}
			if (ImGui::Button("Clear")) Clear();
			ImGui::SameLine();
			bool copy = ImGui::Button("Copy");
			ImGui::SameLine();
			Filter.Draw("Filter", -100.0f);
			ImGui::Separator();
			ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
			if (copy)
				ImGui::LogToClipboard();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			const char* buf = Buf.begin();
			const char* buf_end = Buf.end();
			if (Filter.IsActive())
			{
				for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					if (Filter.PassFilter(line_start, line_end))
						ImGui::TextUnformatted(line_start, line_end);
				}
			}
			else
			{
				// The simplest and easy way to display the entire buffer:
				//   ImGui::TextUnformatted(buf_begin, buf_end);
				// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
				// Here we instead demonstrate using the clipper to only process lines that are within the visible area.
				// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
				// Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
				// both of which we can handle since we an array pointing to the beginning of each line of text.
				// When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
				// Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
				ImGuiListClipper clipper;
				clipper.Begin(LineOffsets.Size);
				while (clipper.Step())
				{
					for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
					{
						const char* line_start = buf + LineOffsets[line_no];
						const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
						ImGui::TextUnformatted(line_start, line_end);
					}
				}
				clipper.End();
			}
			ImGui::PopStyleVar();

			if (ScrollToBottom)
				ImGui::SetScrollHereY(1.0f);
			ScrollToBottom = false;
			ImGui::EndChild();
			ImGui::End();
		}
	};

	// Demonstrate creating a simple log window with basic filtering.
	void ShowAppLog(bool* p_open)
	{
		static ExampleAppLog log;

		// For the demo: add a debug button before the normal log window contents
		// We take advantage of the fact that multiple calls to Begin()/End() are appending to the same window.
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("Log:", p_open);
		if (ImGui::SmallButton("Show Mesh Names"))
		{
			if (P10000 && P10000->MeshesCount > 0) {
				for (int i = 0; i < P10000->MeshesCount; ++i) {
					log.AddLog("[PLAYER]: %s %s", P10000->Meshes[i].MeshInfo->MeshTarget, "\n");
				}
			}
			if (PlSwordSheath && PlSwordSheath->MeshesCount > 0) {
				for (int i = 0; i < PlSwordSheath->MeshesCount; ++i) {
					log.AddLog("[SHEATH]: %s %s", PlSwordSheath->Meshes[i].MeshInfo->MeshTarget, "\n");
				}
			}
			if (PlWig && PlWig->MeshesCount > 0) {
				for (int i = 0; i < PlWig->MeshesCount; ++i) {
					log.AddLog("[HAIR/WIG]: %s %s", PlWig->Meshes[i].MeshInfo->MeshTarget, "\n");
				}
			}
		}
		ImGui::End();

		log.Draw("Log:", p_open);
	}
}