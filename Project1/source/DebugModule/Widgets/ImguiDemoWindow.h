#pragma once
#include <functional>
#include <map>

#include "DebugWindowBase.h"

namespace nzgdc_demo{
    class ImguiDemoWindow : public DebugWindowBase
    {
    public:
        ImguiDemoWindow();
        void RenderContent() override;
        [[nodiscard]] std::string GetWindowId() const override;

    private:
        struct DemoIDs {
            inline static const std::string LayoutDemo{"Layout helper Demo"};
            inline static const std::string ColorPickerDemo{"Color Picker Demo"};
            inline static const std::string InputTextDemo{"Input Text Demo"};
            inline static const std::string TreeDemo{"Tree Demo"};
            inline static const std::string DuplicateIdTable{"Duplicate ID table"};
            inline static const std::string MismatchIDTable{"Mismatch ID table"};
            inline static const std::string TableDemo{"Table Demo"};
            inline static const std::string CoordinatesDemo{"Coordinates Demo"};
            inline static const std::string StickManDemo{"Stick Man Demo"};
            inline static const std::string ImageDemo{"Image Demo"};
            inline static const std::string ItemStateDemo{"Item States Demo"};
            inline static const std::string UserInputDemo{"User Input Demo"};
            inline static const std::string PopupDemo{"Popup Demo"};
        };

        inline static std::unordered_map<std::string, std::string> buttonToolTips{
            {DemoIDs::DuplicateIdTable, "Remove/quantity buttons won't work properly, as their IDs are repeated"},
            {DemoIDs::MismatchIDTable, "Will crash on remove button clicked, as PushID() / PopID() mismatch!"}
        };

        static bool DrawLayoutHelperDemo();
        static bool DrawColorPickerDemo();
        static bool DrawInputTextDemo();
        static bool DrawTreeDemo();
        static bool DrawPopupDemo();
        static bool DrawTableDuplicateIdDemo();
        static bool DrawTablePushIdMismatchDemo();
        static bool DrawTableDemo();
        static bool DrawCoordinatesDemo();
        static bool DrawStickManDemo();
        static bool DrawImageDemo();
        static bool DrawItemStateDemo();
        static bool DrawUserInputDemo();
        static int FilterImGuiLetters(ImGuiInputTextCallbackData* data);
        std::vector<std::pair<std::string, std::pair<bool, std::function<bool()>>>> m_buttonsData;
        
    };
}
