#include "ContentBrowserPanel.h"
#include "mistpch.h"

#include <imgui.h>

namespace Mist {

extern const std::filesystem::path g_AssetPath = "assets";

ContentBrowserPanel::ContentBrowserPanel() :
    m_CurrentDirectory(g_AssetPath) {
    m_DirectoryIcon = MIST_TEXLIB->Create("DirectoryIcon", "res/icons/DirectoryIcon.png", false);
    m_FileIcon = MIST_TEXLIB->Create("FileIcon", "res/icons/FileIcon.png", false);
    m_BackIcon = m_DirectoryIcon;
};

void ContentBrowserPanel::OnImGuiRender() {
    static float padding = 8.0f;
    static float thumbnailSize = 96.0f;

    ImGui::Begin("Content Browser");

    ImVec2 panelSize = ImGui::GetWindowSize();

    {
        float cellSize = thumbnailSize + padding;

        int columnCount = (int)(panelSize.x / cellSize);
        if (columnCount < 1)
            columnCount = 1;

        ImGui::Columns(columnCount, 0, false);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        if (m_CurrentDirectory != g_AssetPath) {
            ImGui::ImageButton("..", (ImTextureID)m_BackIcon->GetRendererID(), {thumbnailSize, thumbnailSize}, {0, 1},
                               {1, 0});
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            ImGui::TextWrapped("..");
            ImGui::NextColumn();
        }

        for (auto& dirEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
            auto relativePath = std::filesystem::relative(dirEntry.path(), g_AssetPath);
            std::string filename = relativePath.filename().string();

            Ref<Texture2D> icon = dirEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
            ImGui::ImageButton(filename.c_str(), (ImTextureID)icon->GetRendererID(), {thumbnailSize, thumbnailSize},
                               {0, 1}, {1, 0});

            if (ImGui::BeginDragDropSource()) {
                const wchar_t* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t),
                                          ImGuiCond_Once);
                ImGui::EndDragDropSource();
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                if (dirEntry.is_directory())
                    m_CurrentDirectory /= filename;

            ImGui::TextWrapped(filename.c_str());
            ImGui::NextColumn();
        }

        ImGui::PopStyleColor();
        ImGui::Columns(1);
    }

    {
        static float offset = 25.0f;
        ImGui::SetCursorPosY(panelSize.y - offset);
        if (ImGui::TreeNode("Display Options")) {
            offset = 85.0f;
            ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 8, 256);
            ImGui::SliderFloat("Padding", &padding, 0, 32);
            ImGui::TreePop();
        } else
            offset = 25.0f;
    }

    ImGui::End();
}

} // namespace Mist