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
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::PopupDemo, std::make_pair(false, []() { return DrawPopupDemo(); })));
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::DuplicateIdTable, std::make_pair(false, []() { return DrawTableDuplicateIdDemo(); })));
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::MismatchIDTable, std::make_pair(false, []() { return DrawTablePushIdMismatchDemo(); })));
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::TableDemo, std::make_pair(false, []() { return DrawTableDemo(); })));
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::CoordinatesDemo, std::make_pair(false, []() { return DrawCoordinatesDemo(); })));
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::StickManDemo, std::make_pair(false, []() { return DrawStickManDemo(); })));
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::ImageDemo, std::make_pair(false, []() { return DrawImageDemo(); })));
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::ItemStateDemo, std::make_pair(false, []() { return DrawItemStateDemo(); })));
    m_buttonsData.emplace_back(std::make_pair(DemoIDs::UserInputDemo, std::make_pair(false, []() { return DrawUserInputDemo(); })));
}

void nzgdc_demo::ImguiDemoWindow::RenderContent()
{
    for (auto& buttonData : m_buttonsData)
    {
        if (ImGui::Button(buttonData.first.c_str()))
        {
            buttonData.second.first = !buttonData.second.first;
            if (buttonData.second.first)
            {
                ImGui::SetNextWindowFocus();
            }
        }
        if (buttonToolTips.contains(buttonData.first))
        {
            if (ImGui::IsItemHovered() && ImGui::BeginTooltip())
            {
                ImGui::TextUnformatted(buttonToolTips.at(buttonData.first).c_str());
                ImGui::EndTooltip();
            }
        }
        if (buttonData.second.first)
        {
            if (!buttonData.second.second())
            {
                buttonData.second.first = false;
            }
        }
    }
}

std::string nzgdc_demo::ImguiDemoWindow::GetWindowId() const
{
    return "ImGui Demo Window";
}

bool nzgdc_demo::ImguiDemoWindow::DrawLayoutHelperDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::LayoutDemo.c_str(), &isOpen);
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
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawColorPickerDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::ColorPickerDemo.c_str(), &isOpen);
    {
        static int intVal{ 5 };
        ImGui::DragInt("Draggable Int", &intVal, -1, -100, 100, "%d", ImGuiSliderFlags_Logarithmic);

        static float colorVal[3] = { 0.0f,0.5f,0.7f };
        ImGui::DragFloat3("Draggable Float 3", colorVal, 0.01f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::InputFloat("Input Float", &colorVal[0], 0.01f, 0.1f, "%.2f");
        ImGui::ColorPicker3("Color picker 3", colorVal, ImGuiColorEditFlags_NoAlpha);
    }
    ImGui::End();
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawInputTextDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::InputTextDemo.c_str(), &isOpen);
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
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawTreeDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::TreeDemo.c_str(), &isOpen);
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

            if (ImGui::CollapsingHeader("Collapsing Header")) // This does not push ID to ID stack
            {
                for (const auto& fruit : fruits)
                {
                    // Error: This will not work, because ID collides with the for loop above!
                    // Try wrap this for loop with PushID() / PopID()
                    if (ImGui::Selectable(fruit.c_str(), selected_fruit == fruit)) 
                    {
                        selected_fruit = fruit;
                    }
                }
            }
            ImGui::TextUnformatted("Text2");
            ImGui::TreePop();
        }
    }
    ImGui::End();
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawPopupDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::PopupDemo.c_str(), &isOpen);
    {
        std::string openPopupId;
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Open"))
            {
                if (ImGui::MenuItem("Popup 1 (wrong)"))
                {
                    ImGui::OpenPopup("Popup1");
                }
                if (ImGui::MenuItem("Popup 1"))
                {
                    openPopupId = "Popup1";
                }
                if (ImGui::MenuItem("Popup modal 1"))
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
            if (ImGui::Selectable("Popup 1 wrong"))
            {
                ImGui::OpenPopup("Popup1");
            }
            if (ImGui::Selectable("Popup 1"))
            {
                openPopupId = "Popup1";
            }
            ImGui::EndCombo();
        }

        if (!openPopupId.empty())
        {
            ImGui::OpenPopup(openPopupId.c_str());
        }

        if (ImGui::BeginPopup("Popup1"))
        {
            ImGui::TextUnformatted("Text1");
            ImGui::EndPopup();
        }
    }
    ImGui::End();
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawTableDuplicateIdDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::DuplicateIdTable.c_str(), &isOpen);
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
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawTablePushIdMismatchDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::MismatchIDTable.c_str(), &isOpen);
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
                    }
                    ++fruit_iterator;
                }
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawTableDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::TableDemo.c_str(), &isOpen);
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
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawCoordinatesDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::CoordinatesDemo.c_str(), &isOpen);
    {
        constexpr static ImVec4 red = ImVec4(1.0f, 0.25f, 0.25f, 1.0f);
        constexpr static ImVec4 green = ImVec4(0.25f, 1.0f, 0.25f, 1.0f);
        constexpr static ImVec4 blue = ImVec4(0.5f, 0.5f, 1.0f, 1.0f);
        static bool addCursorExtraSpacing{false};
        auto showContentRegion = []()
        {
            const auto cursorPos = ImGui::GetCursorPos();
            const auto availableRegion = ImGui::GetContentRegionAvail();
            const auto regionMax = ImGui::GetContentRegionMax();
            const auto windowRegionMin = ImGui::GetWindowContentRegionMin();
            const auto windowRegionMax = ImGui::GetWindowContentRegionMax();
            ImGui::Text("GetCursorPos(): x: %.2f, y: %.2f", cursorPos.x, cursorPos.y);
            ImGui::TextColored(blue, "GetContentRegionAvail(): x: %.2f, y: %.2f", availableRegion.x, availableRegion.y);
            ImGui::TextColored(green, "GetContentRegionMax(): x: %.2f, y: %.2f", regionMax.x, regionMax.y);
            ImGui::TextColored(red, "GetWindowContentRegionMin(): x: %.2f, y: %.2f", windowRegionMin.x, windowRegionMin.y);
            ImGui::TextColored(red, "GetWindowContentRegionMax(): x: %.2f, y: %.2f", windowRegionMax.x, windowRegionMax.y);
        };
        if (addCursorExtraSpacing)
        {
            const auto cursorPos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(cursorPos.y + 25.0f);
        }
        showContentRegion();
        ImGui::Separator();
        ImGui::Columns(2);
        showContentRegion();
        ImGui::NextColumn();
        ImGui::Dummy(ImVec2(50.0f, 0.0f));
        ImGui::SameLine();
        showContentRegion();

        ImGui::Columns(1);
        ImGui::NewLine();
        if (ImGui::Button("WindowPos.x + 50.0f"))
        {
            const ImVec2 windowPos = ImGui::GetWindowPos();
            ImGui::SetWindowPos(ImVec2(windowPos.x + 50.0f, windowPos.y));
        }
        if (ImGui::Button("CursorPos.y + 25.0f"))
        {
            addCursorExtraSpacing = !addCursorExtraSpacing;
        }
    }
    ImGui::End();
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawStickManDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::StickManDemo.c_str(), &isOpen);
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
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawImageDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::ImageDemo.c_str(), &isOpen);
    {
        static nzgdc_demo::Texture texture("res/textures/jack.jpg");
        ImGui::Image(reinterpret_cast<void*>(texture.GetTextureId()), ImVec2(256.0f, 256.0f),
            ImVec2(0, 1), ImVec2(1, 0),
            ImVec4(0.3f, 1.0f, 0.3f, 0.75f), ImVec4(0.3f, 0.3f, 1.0f, 1.0f));
    }
    ImGui::End();
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawItemStateDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::ItemStateDemo.c_str(), &isOpen);
    {
        static std::string clipboard_text;
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
        ImGui::Text("1. Paste Button active: %d (Incorrect, as ImGui::IsItemActive() is pointing to the text item above)", ImGui::IsItemActive());
        // Error: (1. Paste Button active ...) value is incorrect / not intended, because we have created another item above (1. Paste Button hovered ... text)
        // IsItemXXX works on the previously drawn item

        ImGui::Text("2. Paste Button hovered: %d", paste_button_hovered);
        ImGui::Text("2. Paste Button active: %d", paste_button_active); // Correct, as we have cached the item active value earlier. 
    }
    ImGui::End();
    return isOpen;
}

bool nzgdc_demo::ImguiDemoWindow::DrawUserInputDemo()
{
    bool isOpen;
    ImGui::Begin(DemoIDs::UserInputDemo.c_str(), &isOpen);
    {
        static std::vector<ImGuiKey> released_keys, pressed_keys;
        static bool mouse_double_clicked{ false };
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
    return isOpen;
}

int nzgdc_demo::ImguiDemoWindow::FilterImGuiLetters(ImGuiInputTextCallbackData* data)
{
    IMGUI_DEBUG_LOG("input: %c from static function\n", data->EventChar);
    static std::string strFilter = "imgui"; // Filter out i, m, g, u, i characters from user input
    if (data->EventChar < 256 && strFilter.find(static_cast<char>(data->EventChar)) == std::string::npos)
    {
        return 0;
    }
    return 1;
}
