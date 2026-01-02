#pragma once

#include <Mist.h>

#include <filesystem>

namespace Mist {

class EditorLayer;

class ContentBrowserPanel {
public:
    ContentBrowserPanel();

    void RenderBackButton(float thumbnailSize);
    void OnDoubleClick(const std::filesystem::directory_entry& dirEntry);
    void RenderAssets(float thumbnailSize, float padding, ImVec2& panelSize);

    void OnImGuiRender();

    void SetContext(EditorLayer* editorLayer) {
        m_EditorLayer = editorLayer;
    }

private:
    EditorLayer* m_EditorLayer;

    std::filesystem::path m_CurrentDirectory;

    Ref<Texture2D> m_DirectoryIcon, m_FileIcon, m_BackIcon;
};

} // namespace Mist