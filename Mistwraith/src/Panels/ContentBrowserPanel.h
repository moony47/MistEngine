#pragma once
#include <filesystem>

#include <Mist.h>

namespace Mist {

class ContentBrowserPanel {
public:
    ContentBrowserPanel();

    void OnImGuiRender();

private:
    std::filesystem::path m_CurrentDirectory;

    Ref<Texture2D> m_DirectoryIcon, m_FileIcon, m_BackIcon;
};

} // namespace Mist