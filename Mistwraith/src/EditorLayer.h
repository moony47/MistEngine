#pragma once

#include <Mist.h>
#include <Mist/Cameras/EditorCamera.h>

#include "Panels/SceneHierarchyPanel.h"

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
    void NewScene();
    void OpenScene();
    void SaveScene();
    void BeginEditorDockspace();

private:
    Ref<Scene> m_ActiveScene;
    EditorCamera m_EditorCamera;

    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize{0.0f, 0.0f};
    glm::vec2 m_ViewportBounds[2];

    bool m_ViewportFocussed = false, m_ViewportHovered = false;

    int m_GizmoType = 7;

    Entity m_HoveredEntity;

    // Panels
    SceneHierarchyPanel m_SceneHierarchyPanel;
};

} // namespace Mist