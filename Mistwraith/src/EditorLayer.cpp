#include "EditorLayer.h"

#include "CameraController.h"

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
    Mist::FramebufferSpecification fbSpec(1280, 720);
    m_Framebuffer = Mist::Framebuffer::Create(fbSpec);

    // Create scene
    m_ActiveScene = CreateRef<Scene>();

    // Create some entities in the scene, including primary camera
    m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
    m_CameraEntity.AddComponent<CameraComponent>();
    m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    m_ActiveScene->SetPrimaryCamera(&m_CameraEntity);

    m_SpriteEntity = m_ActiveScene->CreateEntity("Sprite");
    m_SpriteEntity.AddComponent<SpriteComponent>("Diamond", glm::vec4{0.8f, 0.2f, 0.8f, 1.0f});
    glm::vec3 rot = glm::radians(glm::vec3{0.0f, 0.0f, 0.0f});
    m_SpriteEntity.Transform().ApplyRotation(rot);

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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

    m_ActiveScene->OnRender(deltaTime);

    m_Framebuffer->Unbind();
}

static void BeginEditorDockspace() {
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

        ImGui::ColorEdit4("Start Colour", glm::value_ptr(m_SpriteEntity.GetComponent<SpriteComponent>().Colour),
                          ImGuiColorEditFlags_Float);

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

        m_ViewportFocussed = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        MIST_APP.GetImGuiLayer()->SetPassEvents(m_ViewportFocussed && m_ViewportHovered);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        glm::vec2 viewportSizePtr = *(glm::vec2*)&viewportSize;
        if (m_ViewportSize != viewportSizePtr) {
            m_ViewportSize = viewportSizePtr;
            m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        }

        ImGui::Image((void*)(uint64_t)m_Framebuffer->GetColourAttachment(), viewportSize, {0, 1}, {1, 0});

        ImGui::End();
        ImGui::PopStyleVar();
    }

    ImGui::End(); // End Dockspace
}

void EditorLayer::OnEvent(Event& e) {
    MIST_PROFILE_FUNCTION();

    if (m_ViewportFocussed && m_ViewportHovered)
        m_ActiveScene->OnEvent(e);
    // m_CameraController.OnEvent(e);
}

}