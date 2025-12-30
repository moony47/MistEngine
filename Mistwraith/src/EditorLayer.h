#pragma once

#include <Mist.h>
#include <Mist/Cameras/EditorCamera.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

// using namespace Mist;
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
    void SaveScene() const;

    void RenderEditorDockspace();
    void RenderDebugPanel(DeltaTime deltaTime);
    void RenderViewportGizmos(DeltaTime deltaTime);
    void RenderViewportPanel(DeltaTime deltaTime);

    void OnScenePlay();
    void OnSceneStop();

    void UI_Toolbar();

private:
    Ref<Scene> m_ActiveScene, m_EditorScene;

    EditorCamera m_EditorCamera;

    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize{0.0f, 0.0f};
    glm::vec2 m_ViewportBounds[2];

    bool m_ViewportFocussed = false, m_ViewportHovered = false;

    int m_GizmoType = 7;

    Entity m_HoveredEntity;

    enum class SceneState {
        Edit = 0, Play = 1
    };
    SceneState m_SceneState = SceneState::Edit;

    // Panels
    SceneHierarchyPanel m_SceneHierarchyPanel;
    ContentBrowserPanel m_ContentBrowserPanel;

    Ref<Texture2D> m_IconPlay, m_IconStop;
};

} // namespace Mist