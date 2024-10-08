#include "ImguiDemoWindow.h"
#include "misc/cpp/imgui_stdlib.h" // Necessary for using std::string with ImGui::InputText
#include "Texture.h"
#include <vector>
nzgdc_demo::ImguiDemoWindow::ImguiDemoWindow()
{
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::LayoutDemo, std::make_pair(false, []() { return DrawLayoutHelperDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::ColorPickerDemo, std::make_pair(false, []() { return DrawColorPickerDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::InputTextDemo, std::make_pair(false, []() { return DrawInputTextDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::TreeDemo, std::make_pair(false, []() { return DrawTreeDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::ComboDemo, std::make_pair(false, []() { return DrawComboDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::PopupDemo, std::make_pair(false, []() { return DrawPopupDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::DuplicateIdTable, std::make_pair(false, []() { return DrawTableDuplicateIdDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::MismatchIDTable, std::make_pair(false, []() { return DrawTablePushIdMismatchDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::TableDemo, std::make_pair(false, []() { return DrawTableDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::CoordinatesDemo, std::make_pair(false, []() { return DrawCoordinatesDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::StickManDemo, std::make_pair(false, []() { return DrawStickManDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::ImageDemo, std::make_pair(false, []() { return DrawImageDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::ItemStateDemo, std::make_pair(false, []() { return DrawItemStateDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::UserInputDemo, std::make_pair(false, []() { return DrawUserInputDemo(); })));
	m_buttonsData.emplace_back(std::make_pair(DemoIDs::DebugLogDemo, std::make_pair(false, []() { return DrawDebugLog(); })));
}

void nzgdc_demo::ImguiDemoWindow::RenderContent()
{
	for (auto& button_data : m_buttonsData)
	{
		bool window_state_change{false};
		if (ImGui::Button((button_data.first + (button_data.second.first ? " - On" : " - Off")).c_str()))
		{
			window_state_change = true;
			button_data.second.first = !button_data.second.first;
			if (button_data.second.first)
			{
				ImGui::SetNextWindowFocus();
				ImGui::SetNextWindowSize(ImVec2(350.0f, 350.0f), ImGuiCond_Once);
			}
		}
		if (buttonToolTips.contains(button_data.first))
		{
			if (ImGui::IsItemHovered() && ImGui::BeginTooltip())
			{
				ImGui::TextUnformatted(buttonToolTips.at(button_data.first).c_str());
				ImGui::EndTooltip();
			}
		}
		if (button_data.second.first)
		{
			if (!button_data.second.second())
			{
				button_data.second.first = false;
				window_state_change = true;
			}
		}
		if (window_state_change)
		{
			IMGUI_DEBUG_LOG("Demo window %s %s\n", button_data.first.c_str(), button_data.second.first ? "opened" : "closed");
		}
	}
}

std::string nzgdc_demo::ImguiDemoWindow::GetWindowId() const
{
	return "ImGui Demo Window (nzgdc)";
}

bool nzgdc_demo::ImguiDemoWindow::DrawLayoutHelperDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::LayoutDemo.c_str(), &is_open);
	{
		ImGui::SeparatorText("Text1");
		ImGui::Separator();
		ImGui::TextUnformatted("Text2");
		ImGui::SameLine();
		ImGui::TextUnformatted("Text3");
		ImGui::Spacing(); // Add a small vertical spacing
		ImGui::TextUnformatted("Text4");
		ImGui::Dummy(ImVec2(15.0f, 30.0f));
		ImGui::SameLine();
		ImGui::TextUnformatted("Text5");
		ImGui::TextUnformatted("Text6");
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawColorPickerDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::ColorPickerDemo.c_str(), &is_open);
	{
		static int int_val{ 5 };
		ImGui::DragInt("Draggable Int", &int_val, -1, -100, 100, "%d", ImGuiSliderFlags_Logarithmic);

		static float col_val[3] = { 0.0f,0.5f,0.7f };
		ImGui::DragFloat3("Draggable Float 3", col_val, 0.01f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::InputFloat("Input Float", &col_val[0], 0.01f, 0.1f, "%.2f");
		ImGui::ColorPicker3("Color picker 3", col_val, ImGuiColorEditFlags_NoAlpha);
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawInputTextDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::InputTextDemo.c_str(), &is_open);
	{
		static std::string password{ "1234" }, username{ "0000" }, content{ "Ohayo Gozaimasu\nBom dia\nBonjour\nGuten Morgen\nBuongiorno" };

		static auto FilterImGuiLettersLambda = [](ImGuiInputTextCallbackData* data)
		{
			IMGUI_DEBUG_LOG("input: %c from lambda\n", data->EventChar);
			static std::string strFilter = "imgui"; // Filter out i, m, g, u characters from user input
			if (data->EventChar < 256 && strFilter.find(static_cast<char>(data->EventChar)) == std::string::npos)
			{
				return 0;
			}
			return 1;
		};

		ImGui::InputText("Input text1", &username, ImGuiInputTextFlags_CallbackCharFilter, FilterImGuiLetters);
		ImGui::InputText("Input text2", &username, ImGuiInputTextFlags_CallbackCharFilter, FilterImGuiLettersLambda);
		ImGui::InputText("Password", &password, ImGuiInputTextFlags_Password);
		ImGui::InputTextMultiline("Content", &content, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 5), ImGuiInputTextFlags_ReadOnly);
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawComboDemo()
{
	bool is_open;

	ImGui::Begin(DemoIDs::ComboDemo.c_str(), &is_open);
	{
		static std::vector<std::string> fruits{ "apple","banana","pear" };
		static std::string selected_fruit{ fruits[0] };
		if (ImGui::BeginCombo("fruit", selected_fruit.c_str()))
		{
			for (const auto& fruit : fruits)
			{
				if (ImGui::Selectable(fruit.c_str(), selected_fruit == fruit))
					selected_fruit = fruit;
			}
			if (ImGui::BeginCombo("nested combo", ""))
			{
				ImGui::TextUnformatted("Text 1");
				ImGui::Button("Button 1");
				ImGui::EndCombo();
			}
			ImGui::EndCombo();
		}
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawTreeDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::TreeDemo.c_str(), &is_open);
	{
		static std::vector<std::string> fruits{ "apple","banana","pear" };
		static std::string selected_fruit{ fruits[0] };
		if (ImGui::TreeNode("treeNode1", "Selected fruit: %s", selected_fruit.c_str()))
		{
			for (const auto& fruit : fruits)
			{
				if (ImGui::Selectable(fruit.c_str(), selected_fruit == fruit))
				{
					selected_fruit = fruit;
				}
			}

			if (ImGui::TreeNode("treeNode1_1", "Nested tree"))
			{
				ImGui::TextUnformatted("Text");
				ImGui::Button("Button1");
				ImGui::TreePop();
			}

			if (ImGui::CollapsingHeader("Collapsing Header"))  // Does not push ID to ID stack
			{
				//ImGui::PushID("CollapsingHeader");
				for (const auto& fruit : fruits)
				{
					if (ImGui::Selectable(fruit.c_str(), selected_fruit == fruit)) // Does not work, because ID collides with the for loop above!
						selected_fruit = fruit;

					if (ImGui::IsItemHovered() && ImGui::BeginTooltip())
					{
						ImGui::TextUnformatted("Does not work, as ID are duplicated");
						ImGui::EndTooltip();
					}
				}
				//ImGui::PopID();
			}
			ImGui::TextUnformatted("Text2");
			ImGui::TreePop();
		}
	}
	ImGui::End();

	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawPopupDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::PopupDemo.c_str(), &is_open, ImGuiWindowFlags_MenuBar);
	{
		std::string open_popup_id;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Open"))
			{
				if (ImGui::MenuItem("Popup 1 (wrong)")) 
				{
					ImGui::OpenPopup("Popup1"); // BeginPopup() not in the same ID layer
				}
				if (ImGui::MenuItem("Popup 1"))
				{
					open_popup_id = "Popup1";
				}
				if (ImGui::MenuItem("Popup modal 1(wrong)"))
				{
					ImGui::OpenPopup("PopupModal1");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::MenuItem("Popup modal 1"))
		{
			ImGui::OpenPopup("PopupModal1");
		}

		if (ImGui::BeginCombo("combo", "combo"))
		{
			if (ImGui::Selectable("Popup 1 (wrong)"))
			{
				ImGui::OpenPopup("Popup1"); // BeginPopup() not in the same ID layer
			}
			if (ImGui::Selectable("Popup 1"))
			{
				open_popup_id = "Popup1";
			}
			ImGui::EndCombo();
		}

		if (!open_popup_id.empty())
		{
			ImGui::OpenPopup(open_popup_id.c_str());
		}

		if (ImGui::BeginPopup("Popup1"))
		{
			ImGui::TextUnformatted("Text1");
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("PopupModal1"))
		{
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawTableDuplicateIdDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::DuplicateIdTable.c_str(), &is_open);
	{
		static std::unordered_map<std::string, int> fruit_map{
			{ "Orange",1 },{ "Apple",5 },{ "Grape",10 },{ "Watermelon",99 },{ "Lemon",-5 }
		};
		if (ImGui::BeginTable("fruit_data_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Fruit", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Number", ImGuiTableColumnFlags_WidthStretch, 125.0f);
			ImGui::TableSetupColumn("Option", ImGuiTableColumnFlags_WidthFixed, 150.0f);
			ImGui::TableHeadersRow();
			for (auto fruit_iterator = fruit_map.begin(); fruit_iterator != fruit_map.end();)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); // Same as ImGui::TableNextColumn()
				{
					ImGui::TextUnformatted(fruit_iterator->first.c_str());
				}
				ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(1)
				{
					ImGui::InputInt("##fruit_num", &fruit_iterator->second); // Error: ID will be the same for all items in the map
				}
				ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(2)
				{
					if (ImGui::Button("Remove")) // Error: ID will be the same for all items in the map
					{
						fruit_iterator = fruit_map.erase(fruit_iterator);
						continue;
					}
				}
				++fruit_iterator;
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawTablePushIdMismatchDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::MismatchIDTable.c_str(), &is_open);
	{
		static std::unordered_map<std::string, int> fruit_map{ { "Orange",1 },{ "Apple",5 },{ "Grape",10 },{ "Watermelon",99 },{ "Lemon",-5 } };
		if (ImGui::BeginTable("fruit_data_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Fruit", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Number", ImGuiTableColumnFlags_WidthStretch, 125.0f);
			ImGui::TableSetupColumn("Option", ImGuiTableColumnFlags_WidthFixed, 150.0f);
			ImGui::TableHeadersRow();
			for (auto fruit_iterator = fruit_map.begin(); fruit_iterator != fruit_map.end();)
			{
				ImGui::PushID(("table_row_" + fruit_iterator->first).c_str());
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); // Same as ImGui::TableNextColumn()
					{
						ImGui::TextUnformatted(fruit_iterator->first.c_str());
					}
					ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(1)
					{
						ImGui::InputInt("##fruit_num", &fruit_iterator->second);
					}
					ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(2)
					{
						if (ImGui::Button("Remove"))
						{
							fruit_iterator = fruit_map.erase(fruit_iterator);
							continue; // Error: continue will create PushID() / PopID() mismatch! Call PopID() above this line
						}
						if (ImGui::IsItemHovered() && ImGui::BeginTooltip())
						{
							ImGui::TextUnformatted("Pressing will trigger runtime error");
							ImGui::EndTooltip();
						}
					}
					++fruit_iterator;
				}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawTableDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::TableDemo.c_str(), &is_open);
	{
		static std::unordered_map<std::string, int> fruit_map{
			{ "Orange",1 },{ "Apple",5 },{ "Grape",10 },{ "Watermelon",99 },{ "Lemon",-5 }
		};
		std::string fruit_to_remove;
		if (ImGui::BeginTable("fruit_data_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Fruit", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Number", ImGuiTableColumnFlags_WidthStretch, 125.0f);
			ImGui::TableSetupColumn("Option", ImGuiTableColumnFlags_WidthFixed, 150.0f);
			ImGui::TableHeadersRow();
			for (auto& fruit : fruit_map)
			{
				ImGui::PushID(("table_row_" + fruit.first).c_str());
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0); // Same as ImGui::TableNextColumn()
					{
						ImGui::TextUnformatted(fruit.first.c_str());
					}
					ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(1)
					{
						ImGui::InputInt("##fruit_num", &fruit.second);
					}
					ImGui::TableNextColumn(); // Same as ImGui::TableSetColumnIndex(2)
					{
						if (ImGui::Button("Remove"))
						{
							fruit_to_remove = fruit.first;
						}
					}
				}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
		if (!fruit_to_remove.empty())
			fruit_map.erase(fruit_to_remove);
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawCoordinatesDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::CoordinatesDemo.c_str(), &is_open);
	{
		constexpr static ImVec4 red = ImVec4(1.0f, 0.25f, 0.25f, 1.0f);
		constexpr static ImVec4 green = ImVec4(0.25f, 1.0f, 0.25f, 1.0f);
		constexpr static ImVec4 blue = ImVec4(0.5f, 0.5f, 1.0f, 1.0f);
		static bool add_cursor_extra_spacing{ false };
		auto show_content_region = []()
		{
			const auto cursor_pos = ImGui::GetCursorPos();
			const auto available_region = ImGui::GetContentRegionAvail();
			const auto region_max = ImGui::GetContentRegionMax();
			const auto window_region_min = ImGui::GetWindowContentRegionMin();
			const auto window_region_max = ImGui::GetWindowContentRegionMax();
			ImGui::Text("GetCursorPos(): x: %.2f, y: %.2f", cursor_pos.x, cursor_pos.y);
			ImGui::TextColored(blue, "GetContentRegionAvail(): x: %.2f, y: %.2f", available_region.x, available_region.y);
			ImGui::TextColored(green, "GetContentRegionMax(): x: %.2f, y: %.2f", region_max.x, region_max.y);
			ImGui::TextColored(red, "GetWindowContentRegionMin(): x: %.2f, y: %.2f", window_region_min.x, window_region_min.y);
			ImGui::TextColored(red, "GetWindowContentRegionMax(): x: %.2f, y: %.2f", window_region_max.x, window_region_max.y);
		};
		if (add_cursor_extra_spacing)
		{
			const auto cursor_pos = ImGui::GetCursorPos();
			ImGui::SetCursorPosY(cursor_pos.y + 25.0f);
		}

		const auto window_pos = ImGui::GetWindowPos();
		const auto cursor_screen_pos = ImGui::GetCursorScreenPos();
		const auto mouse_pos = ImGui::GetMousePos();
		ImGui::Text("GetWindowPos(): x: %.2f, y: %.2f", window_pos.x, window_pos.y);
		ImGui::TextColored(blue, "GetCursorScreenPos(): x: %.2f, y: %.2f", cursor_screen_pos.x, cursor_screen_pos.y);
		ImGui::TextColored(green, "GetMousePos(): x: %.2f, y: %.2f", mouse_pos.x, mouse_pos.y);

		show_content_region();
		ImGui::Separator();
		ImGui::Columns(2);
		show_content_region();
		ImGui::NextColumn();
		ImGui::Dummy(ImVec2(50.0f, 0.0f));
		ImGui::SameLine();
		show_content_region();

		ImGui::Columns(1);
		ImGui::NewLine();
		if (ImGui::Button("WindowPos.x + 50.0f"))
		{
			const ImVec2 window_pos = ImGui::GetWindowPos();
			ImGui::SetWindowPos(ImVec2(window_pos.x + 50.0f, window_pos.y));
		}
		if (ImGui::Button("CursorPos.y + 25.0f"))
		{
			add_cursor_extra_spacing = !add_cursor_extra_spacing;
		}
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawStickManDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::StickManDemo.c_str(), &is_open);
	{
		constexpr float scale = 1.0f;
		constexpr ImU32 color = IM_COL32(255, 255, 255, 255);
		constexpr float thickness = 2.0f * scale;
		const ImVec2 cursor_screen_pos = ImGui::GetCursorScreenPos();
		const ImVec2 avail_region = ImGui::GetContentRegionAvail();
		const ImVec2 center = ImVec2(cursor_screen_pos.x + avail_region.x / 2, cursor_screen_pos.y + avail_region.y / 2);

		constexpr float head_radius = 10.0f * scale;
		const ImVec2 head_pos = ImVec2(center.x, center.y - 30.0f * scale);  // Head position
		const ImVec2 body_top = ImVec2(center.x, center.y - 20.0f * scale);  // Top of the body
		const ImVec2 body_bottom = ImVec2(center.x, center.y + 30.0f * scale);  // Bottom of the body
		const ImVec2 left_arm = ImVec2(center.x - 20.0f * scale, center.y);  // Left arm
		const ImVec2 right_arm = ImVec2(center.x + 20.0f * scale, center.y);  // Right arm
		const ImVec2 left_leg = ImVec2(center.x - 15.0f * scale, center.y + 50.0f * scale);  // Left leg
		const ImVec2 right_leg = ImVec2(center.x + 15.0f * scale, center.y + 50.0f * scale);  // Right leg

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddCircle(head_pos, head_radius, color, 0, thickness); // Draw head
		draw_list->AddLine(body_top, body_bottom, color, thickness); // Draw body
		draw_list->AddLine(body_top, left_arm, color, thickness); // Draw left arm
		draw_list->AddLine(body_top, right_arm, color, thickness); // Draw right arms
		draw_list->AddLine(body_bottom, left_leg, color, thickness); // Draw left leg
		draw_list->AddLine(body_bottom, right_leg, color, thickness); // Draw right leg

		// This is to show coordinates for drawing are in screen space
		const std::string text = "Center x: " + std::to_string(center.x) + ", y: " + std::to_string(center.y);
		draw_list->AddText(cursor_screen_pos, color, text.c_str());
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawImageDemo()
{
	bool is_open;
	ImGui::Begin(DemoIDs::ImageDemo.c_str(), &is_open);
	{
		static nzgdc_demo::Texture texture("res/textures/jack.jpg");
		ImGui::Image(reinterpret_cast<void*>(texture.GetTextureId()), ImVec2(256.0f, 256.0f),
			ImVec2(0, 1), ImVec2(1, 0),
			ImVec4(0.3f, 1.0f, 0.3f, 0.75f), ImVec4(0.3f, 0.3f, 1.0f, 1.0f));
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawItemStateDemo()
{
	bool is_open;
	static std::string clipboard_text;

	ImGui::Begin(DemoIDs::ItemStateDemo.c_str(), &is_open);
	{
		if (ImGui::Button("Copy"))
		{
			const std::string new_clipboard_text = clipboard_text + " copy";
			ImGui::SetClipboardText(new_clipboard_text.c_str());
		}
		ImGui::Text(
			"Button hovered: %d\n"
			"Button active: %d\n"
			, ImGui::IsItemHovered(), ImGui::IsItemActive());

		static bool paste_button_hovered{ false }, paste_button_active{ false };
		static ImVec4 green = ImVec4(0.25f, 1.0f, 0.25f, 1.0f);

		ImGui::TextColored(green, "Clipboard text: %s", clipboard_text.c_str());
		if (ImGui::Button("Paste"))
		{
			clipboard_text = ImGui::GetClipboardText();
		}

		paste_button_hovered = ImGui::IsItemHovered();
		paste_button_active = ImGui::IsItemActive();

		ImGui::Text("1. Paste Button hovered: %d", ImGui::IsItemHovered());
		ImGui::TextWrapped("1. Paste Button active: %d (Incorrect, as ImGui::IsItemActive() is pointing to the text item above)", ImGui::IsItemActive());
		// Error: (1. Paste Button active ...) value is incorrect / not intended, because we have created another item above (1. Paste Button hovered ... text)
		// IsItemXXX works on the previously drawn item

		ImGui::Text("2. Paste Button hovered: %d", paste_button_hovered);
		ImGui::Text("2. Paste Button active: %d", paste_button_active); // Correct, as we have cached the item active value earlier. 
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawUserInputDemo()
{
	bool is_open;
	static std::vector<ImGuiKey> released_keys, pressed_keys;
	static bool mouse_double_clicked{ false };

	ImGui::Begin(DemoIDs::UserInputDemo.c_str(), &is_open);
	{
		ImGui::Text("Keys down:");
		for (ImGuiKey key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1))
		{
			if (ImGui::IsKeyDown(key))
			{
				ImGui::SameLine();
				ImGui::Text("(\"%s\")", ImGui::GetKeyName(key));
			}
			if (ImGui::IsKeyPressed(key, false))
				pressed_keys.emplace_back(key);
			if (ImGui::IsKeyReleased(key))
				released_keys.emplace_back(key);
		}
		ImGui::Text("Pressed keys:");
		std::string pressedKeysStr;
		for (const auto& pressed_key : pressed_keys)
		{
			pressedKeysStr += ("(" + std::string(ImGui::GetKeyName(pressed_key)) + ")");
		}
		ImGui::SameLine();
		ImGui::TextWrapped(pressedKeysStr.c_str());

		std::string releasedKeysStr;
		ImGui::Text("Released keys:");
		for (const auto& released_key : released_keys)
		{
			releasedKeysStr += ("(" + std::string(ImGui::GetKeyName(released_key)) + ")");
		}
		ImGui::SameLine();
		ImGui::TextWrapped(releasedKeysStr.c_str());

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			mouse_double_clicked = true;
		}
		ImGui::Text("Left mouse button double clicked: %d", mouse_double_clicked);

		if (ImGui::Button("Reset released and pressed keys"))
		{
			released_keys.clear();
			pressed_keys.clear();
			mouse_double_clicked = false;
		}
	}
	ImGui::End();
	return is_open;
}

bool nzgdc_demo::ImguiDemoWindow::DrawDebugLog()
{
	static int log_counter{0};
	ImGui::SameLine();
	if (ImGui::Button("Add sample log"))
	{
		IMGUI_DEBUG_LOG("This is a sample log: %d\n", log_counter++);
	}
	bool is_open;
	ImGui::ShowDebugLogWindow(&is_open);
	return is_open;
}

int nzgdc_demo::ImguiDemoWindow::FilterImGuiLetters(ImGuiInputTextCallbackData* data)
{
	IMGUI_DEBUG_LOG("input: %c from static function\n", data->EventChar);
	static std::string strFilter = "imgui"; // Filter out i, m, g, u, i characters from user input
	if (data->EventChar < 256 &&  strFilter.find(static_cast<char>(data->EventChar)) == std::string::npos)
	{
		return 0;
	}
	return 1;
}
