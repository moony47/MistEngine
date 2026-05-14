#include "EditorLayer.h"
#include "CameraController.h"

#include "Mist/Scene/SceneSerialiser.h"
#include "Mist/Utils/PlatformUtils.h"

#include "Mist/Scripting/DotNetRuntime.h"

#include <ImGuizmo.h>

namespace Mist {

extern const std::filesystem::path g_AssetPath;

EditorLayer::EditorLayer() :
    Layer("EditorLayer") {};

EditorLayer::~EditorLayer() {};

void EditorLayer::OnAttach() {
    MIST_PROFILE_FUNCTION();

    // Create textures
    m_IconPlay = MIST_TEXLIB->Create("PlayIcon", "res/icons/PlayButton.png", false);
    m_IconStop = MIST_TEXLIB->Create("StopIcon", "res/icons/StopButton.png", false);

    // Create framebuffer
    Mist::FramebufferSpecification fbSpec(
        1280, 720,
        {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth});
    m_Framebuffer = Mist::Framebuffer::Create(fbSpec);

#ifdef MIST_DEBUG
    m_DotnetRuntime = CreateRef<DotNetRuntime>();
    m_DotnetRuntime->Initialize("D:\\dev\\MistEngine\\ScriptEngine\\bin\\Debug\\net10.0\\ScriptEngine.dll");
#elif MIST_RELEASE
    m_DotnetRuntime = CreateRef<DotNetRuntime>();
    m_DotnetRuntime->Initialize("D:\\dev\\MistEngine\\ScriptEngine\\bin\\Release\\net10.0\\ScriptEngine.dll");
#elif MIST_DIST
    m_DotnetRuntime = CreateRef<DotNetRuntime>();
    m_DotnetRuntime->Initialize("D:\\dev\\MistEngine\\ScriptEngine\\bin\\Dist\\net10.0\\ScriptEngine.dll");
#endif

    // Create scene
    auto cmdLineArgs = MIST_APP.GetCommandLineArgs();
    if (cmdLineArgs.Count > 1) {
        auto sceneFilePath = cmdLineArgs[1];
        OpenScene(sceneFilePath);
    } else
        NewScene();

    Entity en = m_ActiveScene->CreateEntity({}, "MovingEntity");
    ManagedScriptComponent script = en.AddComponent<ManagedScriptComponent>("Mist.Scripting.SimpleMovement");

    m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
    m_ContentBrowserPanel.SetContext(this);
}

void EditorLayer::OnDetach() {
    MIST_PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();
    if (m_SceneState == SceneState::Play)
        m_ActiveScene->OnUpdate(deltaTime);
}

void EditorLayer::OnRender(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    Mist::Renderer2D::ResetStats();

    m_Framebuffer->Bind();

    RenderCommand::SetClearColour(glm::vec4{0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();
    m_Framebuffer->ClearColourAttachment(1, -1);

    // Render Scene from EditorCamera view
    switch (m_SceneState) {
        case SceneState::Edit:
            m_ActiveScene->OnRenderEditor(deltaTime, m_EditorCamera);
            break;
        case SceneState::Play:
            m_ActiveScene->OnRender(deltaTime);
            break;
    }

    // Which entity is being hovered over right now?
    FindHoveredEntity();

    m_Framebuffer->Unbind();
}

void EditorLayer::FindHoveredEntity() {
    auto [mx, my] = ImGui::GetMousePos();
    mx -= m_ViewportBounds[0].x;
    my = (m_ViewportBounds[1].y - m_ViewportBounds[0].y) - (my - m_ViewportBounds[0].y);

    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < m_ViewportSize.x && mouseY < m_ViewportSize.y) {
        int entity = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
        m_HoveredEntity = entity == -1 ? Entity{} : Entity{m_ActiveScene.get(), (entt::entity)entity};
    } else
        m_HoveredEntity = {};
}

void EditorLayer::NewScene() {
    m_EditorScene = CreateRef<Scene>(m_DotnetRuntime);
    m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_EditorScenePath = std::filesystem::path();
    OnSceneStop();
}

void EditorLayer::OpenScene() {
    std::filesystem::path filepath = FileDialogs::OpenFile("Mist Scene (*.mist.yaml)\0*.mist.yaml\0");
    if (!filepath.empty())
        OpenScene(filepath);
}

void EditorLayer::OpenScene(const std::filesystem::path& path) {
    if (m_SceneState != SceneState::Edit)
        OnSceneStop();

    if (path.extension().string() != ".yaml") {
        MIST_WARN("[EditorLayer::OpenScene] Could not load {0} as it is not a scene file", path.filename().string());
        return;
    }

    NewScene();
    m_EditorScenePath = path;
    SceneSerialiser serialiser(m_EditorScene);
    serialiser.Deserialise(path.string());
}

void EditorLayer::SaveScene() {
    if (!m_EditorScenePath.empty())
        SerialiseScene();
    else
        SaveSceneAs();
}

void EditorLayer::SaveSceneAs() {
    std::filesystem::path filepath = FileDialogs::SaveFile("Mist Scene (*.mist.yaml)\0*.mist.yaml\0");
    if (!filepath.empty()) {
        m_EditorScenePath = filepath;
        SerialiseScene();
    }
}

void EditorLayer::SerialiseScene() {
    SceneSerialiser serialiser(m_EditorScene);
    serialiser.Serialise(m_EditorScenePath.string());
}

void EditorLayer::RenderEditorDockspace() {
    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    {
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                            ImGuiWindowFlags_NoNavFocus;
        } else
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        {
            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("EditorDockspace", &dockspaceOpen, window_flags);
            if (!opt_padding)
                ImGui::PopStyleVar();
        }

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Mistwraith")) {
            if (ImGui::MenuItem("Exit"))
                MIST_APP.Close();
            // ImGui::Separator();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                NewScene();
            if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
                OpenScene();
            ImGui::Separator();
            if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                SaveScene();
            if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
                SaveSceneAs();
            // ImGui::Separator();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void EditorLayer::RenderDebugPanel(DeltaTime deltaTime) {
    ImGui::Begin("Debug Info");

#ifdef MIST_PROFILING
    ImGui::Checkbox("Profiling", &MIST_PROFILE_ENABLED);
#endif
    ImGui::Text("Hovered Entity: %s",
                m_HoveredEntity ? m_HoveredEntity.GetComponent<IDComponent>().Name.c_str() : "None");
    ImGui::Text("Application FPS: %.3f ms/frame (%.1f FPS)", deltaTime.GetMilliseconds(),
                1.0f / deltaTime.GetSeconds());
    ImGui::Text("     Quads: %i", Mist::Renderer2D::GetStats().QuadCount);
    ImGui::Text("   Circles: %i", Mist::Renderer2D::GetStats().CircleCount);
    ImGui::Text("  Vertices: %i", Mist::Renderer2D::GetStats().VertexCount());
    ImGui::Text("   Indices: %i", Mist::Renderer2D::GetStats().IndexCount());
    ImGui::Text("Draw Calls: %i", Mist::Renderer2D::GetStats().DrawCalls);

    ImGui::End();
}

void EditorLayer::RenderViewportGizmos(DeltaTime deltaTime) {
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
        bool snapping = Input::IsKeyPressed(Key::LeftControl);
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

        ImGuizmo::Manipulate(value_ptr(cameraView), value_ptr(cameraProj), op, mode, value_ptr(transform), nullptr,
                             snapping ? value_ptr(snapValue) : nullptr);

        if (ImGuizmo::IsUsing()) {
            glm::vec3 position, rotation, scale;
            ImGuizmo::DecomposeMatrixToComponents(value_ptr(transform), value_ptr(position), value_ptr(rotation),
                                                  value_ptr(scale));
            transformComp.SetPosition(position);
            transformComp.SetRotation(rotation);
            transformComp.SetScale(scale);
        } else
            m_EditorCamera.OnUpdate(deltaTime, m_ViewportFocussed && m_ViewportHovered);
    } else
        m_EditorCamera.OnUpdate(deltaTime, m_ViewportFocussed && m_ViewportHovered);
}

void EditorLayer::RenderViewportPanel(DeltaTime deltaTime) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::Begin("Viewport", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
    m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

    m_ViewportFocussed = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    glm::vec2 viewportSizePtr = *(glm::vec2*)&viewportSize;
    if (m_ViewportSize != viewportSizePtr) {
        m_ViewportSize = viewportSizePtr;
        m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
    }

    ImGui::Image((void*)(uint64_t)m_Framebuffer->GetColourAttachment(), viewportSize, {0, 1}, {1, 0});

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const wchar_t* path = (const wchar_t*)payload->Data;
            OpenScene(g_AssetPath / path);
        }
        ImGui::EndDragDropTarget();
    }

    // Gizmos
    if (m_SceneState == SceneState::Edit)
        RenderViewportGizmos(deltaTime);

    RenderUIToolbar();

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void EditorLayer::RenderUIToolbar() {
    bool showPlayBorder = m_SceneState == SceneState::Play;
    if (showPlayBorder)
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.5f, 0.1f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colours = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colours[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto& buttonActive = colours[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    {
        ImGui::SetNextWindowSize({0, 50});
        ImGui::Begin("##toolbar", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        float size = ImGui::GetContentRegionAvail().y - 5.0f;
        Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
        ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton("StartStop", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1),
                               ImVec2(1, 0))) {
            if (m_SceneState == SceneState::Edit)
                OnScenePlay();
            else
                OnSceneStop();
        }

        ImGui::End();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(showPlayBorder ? 4 : 3);
}

void EditorLayer::OnImGuiRender(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    RenderEditorDockspace(); // Begin Dockspace

    m_ContentBrowserPanel.OnImGuiRender();
    m_SceneHierarchyPanel.OnImGuiRender();

    RenderDebugPanel(deltaTime);

    {
        ImGui::Begin("EditorLayer");
        // std::string texName = "diamond";
        // float height = 256;
        // Ref<Texture2D> texture = MIST_TEX(texName);
        // glm::vec2 bl = texture->GetTexCoords(0);
        // glm::vec2 tr = texture->GetTexCoords(2);
        // ImGui::Image((void*)(uint64_t)texture->GetRendererID(), {height * texture->GetAspectRatio(), height},
        //              ImVec2(bl.x, tr.y), ImVec2(tr.x, bl.y));
        ImGui::End();
    }

    RenderViewportPanel(deltaTime);

    ImGui::End(); // End Dockspace
}

void EditorLayer::OnScenePlay() {
    m_SceneState = SceneState::Play;

    m_ActiveScene = Scene::Copy(m_EditorScene);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::OnSceneStop() {
    m_SceneState = SceneState::Edit;

    m_ActiveScene = m_EditorScene;
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::DuplicateEntity() {
    if (m_SceneState != SceneState::Edit)
        return;

    Entity selected = m_SceneHierarchyPanel.GetSelectedEntity();
    if (selected)
        m_EditorScene->DuplicateEntity(selected);
}

void EditorLayer::DeleteEntity() {
    if (m_SceneState != SceneState::Edit)
        return;

    Entity selected = m_SceneHierarchyPanel.GetSelectedEntity();
    if (selected) {
        m_SceneHierarchyPanel.ClearSelectedEntity();
        m_EditorScene->DestroyEntity(selected);
    }
}

void EditorLayer::OnEvent(Event& e) {
    MIST_PROFILE_FUNCTION();

    if (e.Handled)
        return;

    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) {
        if (e.GetRepeatCount() > 0)
            return false;

        bool controlPressed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shiftPressed = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        // Hotkeys
        switch (e.GetKeyCode()) {
            // File Options
            case Key::S:
                if (controlPressed) {
                    if (shiftPressed)
                        SaveSceneAs();
                    else
                        SaveScene();
                }
                break;
            case Key::O:
                if (controlPressed)
                    OpenScene();
                break;
            case Key::N:
                if (controlPressed)
                    NewScene();
                break;

            // Scene Hotkeys
            case Key::D:
                if (controlPressed)
                    DuplicateEntity();
                break;
            case Key::Delete:
                DeleteEntity();
                break;

            // Gizmo Types
            case Key::Q:
                m_GizmoType = -1;
                break;
            case Key::W:
                m_GizmoType = (int)ImGuizmo::OPERATION::TRANSLATE;
                break;
            case Key::E:
                m_GizmoType = (int)ImGuizmo::OPERATION::ROTATE;
                break;
            case Key::R:
                m_GizmoType = (int)ImGuizmo::OPERATION::SCALE;
                break;
        }

        return false;
    });

    dispatcher.Dispatch<MouseButtonReleasedEvent>(
        [&shp = m_SceneHierarchyPanel, h = m_ViewportHovered, he = m_HoveredEntity](const MouseButtonReleasedEvent& e) {
            // If left clicked, select the hovered entity / deselect if nothing is hovered
            if (e.GetPressType() == MousePressedType::Click && e.GetMouseButton() == MouseButton::Left && h) {
                shp.SetSelectedEntity(he);
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