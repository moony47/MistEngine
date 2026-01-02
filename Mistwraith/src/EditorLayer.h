#pragma once

#include <Mist.h>
#include <Mist/Cameras/EditorCamera.h>

#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Mist {

class EditorLayer : public Layer {
public:
    EditorLayer();
    ~EditorLayer() override;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(DeltaTime deltaTime) override;
    void OnRender(DeltaTime deltaTime) override;
    void OnImGuiRender(DeltaTime deltaTime) override;

    void OnEvent(Event& e) override;

private:
    void FindHoveredEntity();

    void NewScene();
    void OpenScene();
    void OpenScene(const std::filesystem::path& path);
    void SaveScene();
    void SaveSceneAs();

    void SerialiseScene();

    void RenderEditorDockspace();
    void RenderDebugPanel(DeltaTime deltaTime);
    void RenderViewportGizmos(DeltaTime deltaTime);
    void RenderViewportPanel(DeltaTime deltaTime);

    void OnScenePlay();
    void OnSceneStop();

    void DuplicateEntity();
    void DeleteEntity();

    void RenderUIToolbar();

    friend class ContentBrowserPanel;

private:
    // Edit/Play State
    enum class SceneState {
        Edit = 0,
        Play = 1
    };
    SceneState m_SceneState = SceneState::Edit;

    // Scenes
    Ref<Scene> m_ActiveScene, m_EditorScene;
    EditorCamera m_EditorCamera;
    std::filesystem::path m_EditorScenePath;

    // Scene Viewport
    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize{0.0f, 0.0f};
    glm::vec2 m_ViewportBounds[2];

    // Controls
    int m_GizmoType = 7;
    Entity m_HoveredEntity;
    bool m_ViewportFocussed = false, m_ViewportHovered = false;

    // Panels
    SceneHierarchyPanel m_SceneHierarchyPanel;
    ContentBrowserPanel m_ContentBrowserPanel;

    // Textures
    Ref<Texture2D> m_IconPlay, m_IconStop;
};

} // namespace Mist