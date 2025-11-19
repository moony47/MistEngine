#include "CameraController.h"
#include "EditorLayer.h"

#include <ImGuizmo.h>

#include "Mist/Scene/SceneSerialiser.h"
#include "Mist/Utils/PlatformUtils.h"

#include "Mist/Maths/Maths.h"

namespace Mist {

static const size_t s_MapWidth = 24;
static const char* s_MapTiles = "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWGGGGGGGGGGGGWWWWWW"
                                "WWWWWGGGGGGGGGGGGGGWWWWW"
                                "WWWWGGGGGGGWWWWGGGGGWWWW"
                                "WWWGGGGGWWWWWWWWGGGGWWWW"
                                "WWGGGGGGWWWWWWWWGGGGWWWW"
                                "WWGGGGGGWWWWWWGGGGGWWWWW"
                                "WWGGGGGGGGWWWGGGGGWWWWWW"
                                "WWGGGGGGGGGGGGGGGWWWWWWW"
                                "WWWGGGGGGGGGGGGGWWWWWWWW"
                                "WWWWGGGGGGGGGGWWWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWWWWWWWWWWGGGWWWWWW"
                                "WWWWWWWWWWWWWGGGGGGWWWWW"
                                "WWWWWWWWWWWWWGGGGGGWWWWW"
                                "WWWWWWWWWWWWGGGGGGWWWWWW"
                                "WWWWWWWWWWWWWWGGWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWWWWWWWWWWWWWWWWWWW";
static const size_t s_MapHeight = strlen(s_MapTiles) / s_MapWidth;

EditorLayer::EditorLayer() :
    Layer("EditorLayer") {
}

EditorLayer::~EditorLayer() {
}

void EditorLayer::OnAttach() {
    MIST_PROFILE_FUNCTION();

    // Create textures
    MIST_TEXLIB->Create("Diamond", "res/textures/diamond.png");
    MIST_TEXLIB->Create("Star", "res/textures/star.png");

    MIST_TEXLIB->Create("SpriteSheet", "res/textures/RPGpack_sheet_2X.png");
    MIST_TEXLIB->CreateSub("W", "SpriteSheet", {11, 11}, {128, 128});
    MIST_TEXLIB->CreateSub("G", "SpriteSheet", {1, 11}, {128, 128});

    // Create framebuffer
    Mist::FramebufferSpecification fbSpec(
        1280, 720,
        {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth});
    m_Framebuffer = Mist::Framebuffer::Create(fbSpec);

    // Create scene
    m_ActiveScene = CreateRef<Scene>();

    m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

    SceneSerialiser serialiser(m_ActiveScene);
    serialiser.Deserialise("res/scenes/Example.mist.yaml");

    // Create some entities in the scene, including primary camera
    // m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
    // m_CameraEntity.AddComponent<CameraComponent>();
    // m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    // m_ActiveScene->SetPrimaryCamera(m_CameraEntity);

    // m_SpriteEntity1 = m_ActiveScene->CreateEntity("Sprite1");
    // m_SpriteEntity1.AddComponent<SpriteComponent>("Diamond", glm::vec4{0.8f, 0.2f, 0.8f, 1.0f});

    // m_SpriteEntity2 = m_ActiveScene->CreateEntity("Sprite2");
    // m_SpriteEntity2.AddComponent<SpriteComponent>("Star", glm::vec4{0.2f, 0.8f, 0.8f, 1.0f});
}

void EditorLayer::OnDetach() {
    MIST_PROFILE_FUNCTION();

    // Delete textures
    MIST_TEXLIB->Remove("Diamond");
    MIST_TEXLIB->Remove("Star");
    MIST_TEXLIB->Remove("SpriteSheet", true);
}

void EditorLayer::OnUpdate(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();
    if (m_ViewportFocussed)
        m_ActiveScene->OnUpdate(deltaTime);
}

void EditorLayer::OnRender(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    Mist::Renderer2D::ResetStats();

    m_Framebuffer->Bind();

    RenderCommand::SetClearColour(glm::vec4{0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();

    m_ActiveScene->OnRenderEditor(deltaTime, m_EditorCamera);

    if (!Input::IsMouseButtonPressed(MouseButtonCode::Left)) {
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;

        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < m_ViewportSize.x && mouseY < m_ViewportSize.y) {
            int entity = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            m_HoveredEntity =
                (entity > -1 && entity < 10000) ? Entity{m_ActiveScene.get(), (entt::entity)entity} : Entity{};
        }
    }

    m_Framebuffer->Unbind();
}

void EditorLayer::NewScene() {
    m_ActiveScene = CreateRef<Scene>();
    m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::OpenScene() {
    std::string filepath = FileDialogs::OpenFile("Mist Scene (*.mist.yaml)\0*.mist.yaml\0");
    if (!filepath.empty()) {
        NewScene();
        SceneSerialiser serialiser(m_ActiveScene);
        serialiser.Deserialise(filepath);
    }
}

void EditorLayer::SaveScene() {
    std::string filepath = FileDialogs::SaveFile("Mist Scene (*.mist.yaml)\0*.mist.yaml\0");
    if (!filepath.empty()) {
        SceneSerialiser serialiser(m_ActiveScene);
        serialiser.Serialise(filepath);
    }
}

void EditorLayer::BeginEditorDockspace() {
    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    } else {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Mistwraith")) {
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
                MIST_APP.Close();
            if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                NewScene();
            if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
                OpenScene();
            if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
                SaveScene();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void EditorLayer::OnImGuiRender(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    BeginEditorDockspace(); // Begin Dockspace

    m_SceneHierarchyPanel.OnImGuiRender();

    {
        ImGui::Begin("Debug Info");

#ifdef MIST_PROFILING
        ImGui::Checkbox("Profiling", &MIST_PROFILE_ENABLED);
#endif
        ImGui::Text("Application FPS: %.3f ms/frame (%.1f FPS)", deltaTime.GetMilliseconds(),
                    1.0f / deltaTime.GetSeconds());
        ImGui::Text("     Quads: %i", Mist::Renderer2D::GetStats().QuadCount);
        ImGui::Text("  Vertices: %i", Mist::Renderer2D::GetStats().GetVertexCount());
        ImGui::Text("   Indices: %i", Mist::Renderer2D::GetStats().GetIndexCount());
        ImGui::Text("Draw Calls: %i", Mist::Renderer2D::GetStats().DrawCalls);

        ImGui::End();
    }

    {
        ImGui::Begin("EditorLayer");

        {
            std::string texName = "Diamond";
            float height = 256;
            Ref<Texture2D> texture = MIST_TEX(texName);
            glm::vec2 bl = texture->GetTexCoords(0);
            glm::vec2 tr = texture->GetTexCoords(2);
            ImGui::Image((void*)(uint64_t)texture->GetRendererID(), {height * texture->GetAspectRatio(), height},
                         ImVec2(bl.x, tr.y), ImVec2(tr.x, bl.y));
        }

        ImGui::End();
    }

    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
        m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

        m_ViewportFocussed = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        // MIST_APP.GetImGuiLayer()->SetPassEvents(m_ViewportFocussed || m_ViewportHovered);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        glm::vec2 viewportSizePtr = *(glm::vec2*)&viewportSize;
        if (m_ViewportSize != viewportSizePtr) {
            m_ViewportSize = viewportSizePtr;
            m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        }

        ImGui::Image((void*)(uint64_t)m_Framebuffer->GetColourAttachment(), viewportSize, {0, 1}, {1, 0});

        {
            // Gizmos
            Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity && m_GizmoType != -1) {
                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

                ImVec2 pos = ImGui::GetWindowPos();
                ImVec2 size = ImGui::GetWindowSize();
                ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

                const glm::mat4& cameraView = m_EditorCamera.GetView();
                const glm::mat4& cameraProj = m_EditorCamera.GetProjection();

                auto& transformComp = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transform = transformComp.GetTransform();

                ImGuizmo::OPERATION op = (ImGuizmo::OPERATION)m_GizmoType;
                bool snapping = !Input::IsKeyPressed(KeyCode::LeftControl);
                glm::vec3 snapValue;
                switch (op) {
                    case ImGuizmo::OPERATION::TRANSLATE:
                        snapValue = {0.5f, 0.5f, 0.5f};
                        break;
                    case ImGuizmo::OPERATION::ROTATE:
                        snapValue = {15.0f, 15.0f, 15.0f};
                        break;
                    case ImGuizmo::OPERATION::SCALE:
                        snapValue = {0.25f, 0.25f, 0.25f};
                        break;
                }
                ImGuizmo::MODE mode = ImGuizmo::WORLD;
                //(op == ImGuizmo::OPERATION::TRANSLATE) ? ImGuizmo::WORLD : ImGuizmo::LOCAL;

                ImGuizmo::Manipulate(value_ptr(cameraView), value_ptr(cameraProj), op, mode, value_ptr(transform),
                                     nullptr, snapping ? value_ptr(snapValue) : nullptr);

                if (ImGuizmo::IsUsing()) {
                    glm::vec3 position, rotation, scale;
                    ImGuizmo::DecomposeMatrixToComponents(value_ptr(transform), value_ptr(position),
                                                          value_ptr(rotation), value_ptr(scale));
                    transformComp.SetPosition(position);
                    transformComp.SetRotation(rotation);
                    transformComp.SetScale(scale);
                } else
                    m_EditorCamera.OnUpdate(deltaTime, m_ViewportFocussed && m_ViewportHovered);
            } else
                m_EditorCamera.OnUpdate(deltaTime, m_ViewportFocussed && m_ViewportHovered);
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

    ImGui::End(); // End Dockspace
}

void EditorLayer::OnEvent(Event& e) {
    MIST_PROFILE_FUNCTION();

    if (e.Handled)
        return;

    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<KeyPressedEvent>([=](KeyPressedEvent& e) {
        if (e.GetRepeatCount() > 0)
            return false;

        bool controlPressed = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
        bool shiftPressed = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

        // Hotkeys
        switch (e.GetKeyCode()) {
            // File Options
            case KeyCode::S:
                if (controlPressed && shiftPressed)
                    SaveScene();
                break;
            case KeyCode::O:
                if (controlPressed)
                    OpenScene();
                break;
            case KeyCode::N:
                if (controlPressed)
                    NewScene();
                break;

            // Gizmo Types
            case KeyCode::Q:
                m_GizmoType = -1;
                break;
            case KeyCode::W:
                m_GizmoType = (int)ImGuizmo::OPERATION::TRANSLATE;
                break;
            case KeyCode::E:
                m_GizmoType = (int)ImGuizmo::OPERATION::ROTATE;
                break;
            case KeyCode::R:
                m_GizmoType = (int)ImGuizmo::OPERATION::SCALE;
                break;
        }

        return false;
    });

    dispatcher.Dispatch<MouseButtonReleasedEvent>([=](MouseButtonReleasedEvent& e) {
        if (e.GetMouseButton() == MouseButtonCode::Left && m_ViewportHovered && !ImGuizmo::IsOver()) {
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
            if (m_HoveredEntity != Entity{})
                return true;
        }
        return false;
    });

    if (m_ViewportFocussed && (m_ViewportHovered || !e.IsInCategory(EventCategoryMouse)))
        m_ActiveScene->OnEvent(e);

    if (m_ViewportHovered || !e.IsInCategory(EventCategoryMouse))
        m_EditorCamera.OnEvent(e);
}

} // namespace Mist