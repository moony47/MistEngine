#include "EditorLayer.h"

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

static const std::string* GetTileTexture(size_t x, size_t y) {
    size_t pos = x + y * s_MapWidth;
    char chr = s_MapTiles[pos];

    std::string* str = new std::string(1, chr);

    if (chr == 'G')
        return str;

    if (pos < strlen(s_MapTiles) - s_MapWidth && s_MapTiles[pos + s_MapWidth] != chr) // S
        *str += 'S';
    else if (pos >= s_MapWidth && s_MapTiles[pos - s_MapWidth] != chr) // N
        *str += 'N';

    if (pos != strlen(s_MapTiles) - 1 && s_MapTiles[pos + 1] != chr) // E
        *str += 'E';
    else if (pos != 0 && s_MapTiles[pos - 1] != chr) // W
        *str += 'W';

    if (str->length() == 1) {
        // NE
        if (pos >= s_MapWidth - 1 && s_MapTiles[pos - s_MapWidth + 1] != chr)
            *str += '1';
        // NW
        else if (pos >= s_MapWidth + 1 && s_MapTiles[pos - s_MapWidth - 1] != chr)
            *str += '2';
        // SE
        else if (pos < strlen(s_MapTiles) - s_MapWidth - 1 && s_MapTiles[pos + s_MapWidth + 1] != chr)
            *str += '3';
        // SW
        else if (pos < strlen(s_MapTiles) - s_MapWidth + 1 && s_MapTiles[pos + s_MapWidth - 1] != chr)
            *str += '4';
    }
    return str;
}

EditorLayer::EditorLayer() :
    Layer("EditorLayer"),
    m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
}

EditorLayer::~EditorLayer() {
}

void EditorLayer::OnAttach() {
    MIST_PROFILE_FUNCTION();

    MIST_TEXLIB->Create("Diamond", "res/textures/diamond.png");
    // MIST_TEXLIB->Create("Star", "res/textures/star.png");

    MIST_TEXLIB->Create("SpriteSheet", "res/textures/RPGpack_sheet_2X.png");

    MIST_TEXLIB->CreateSub("WSW", "SpriteSheet", {10, 10}, {128, 128});
    MIST_TEXLIB->CreateSub("WS", "SpriteSheet", {11, 10}, {128, 128});
    MIST_TEXLIB->CreateSub("WSE", "SpriteSheet", {12, 10}, {128, 128});
    MIST_TEXLIB->CreateSub("WW", "SpriteSheet", {10, 11}, {128, 128});
    MIST_TEXLIB->CreateSub("W", "SpriteSheet", {11, 11}, {128, 128});
    MIST_TEXLIB->CreateSub("WE", "SpriteSheet", {12, 11}, {128, 128});
    MIST_TEXLIB->CreateSub("WNW", "SpriteSheet", {10, 12}, {128, 128});
    MIST_TEXLIB->CreateSub("WN", "SpriteSheet", {11, 12}, {128, 128});
    MIST_TEXLIB->CreateSub("WNE", "SpriteSheet", {12, 12}, {128, 128});

    MIST_TEXLIB->CreateSub("W1", "SpriteSheet", {13, 11}, {128, 128});
    MIST_TEXLIB->CreateSub("W2", "SpriteSheet", {14, 11}, {128, 128});
    MIST_TEXLIB->CreateSub("W3", "SpriteSheet", {13, 12}, {128, 128});
    MIST_TEXLIB->CreateSub("W4", "SpriteSheet", {14, 12}, {128, 128});

    MIST_TEXLIB->CreateSub("G", "SpriteSheet", {1, 11}, {128, 128});

    Mist::FramebufferSpecification fbSpec(1280, 720);
    m_Framebuffer = Mist::Framebuffer::Create(fbSpec);
}

void EditorLayer::OnDetach() {
    MIST_PROFILE_FUNCTION();

    MIST_TEXLIB->Remove("Diamond");
    // MIST_TEXLIB->Remove("Star");
    MIST_TEXLIB->Remove("SpriteSheet", true);
}

void EditorLayer::OnUpdate(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(deltaTime);
}

void EditorLayer::OnFrameStart(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    Mist::Renderer2D::ResetStats();

    m_Framebuffer->Bind();

    RenderCommand::SetClearColour(glm::vec4{0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();
    Renderer2D::BeginScene(m_CameraController.GetCamera());
}

void EditorLayer::OnFrameEnd(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    for (size_t y = 0; y < s_MapHeight; y++)
        for (size_t x = 0; x < s_MapWidth; x++) {
            const std::string* texStr = GetTileTexture(x, y);
            Renderer2D::DrawQuad({(float)x - (float)(s_MapWidth / 2), (float)(s_MapHeight / 2) - (float)y},
                                 glm::radians(0.0f), {1.0f, 1.0f}, *texStr);
            delete texStr;
        }

    Renderer2D::EndScene();
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
                Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void EditorLayer::OnImGuiRender(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    BeginEditorDockspace(); // Begin Dockspace

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

    ImGui::Begin("EditorLayer");

    ImGui::ColorEdit4("Start Colour", glm::value_ptr(m_SpriteColour), ImGuiColorEditFlags_Float);

    std::string texName = "Diamond";
    float height = 256;
    Ref<Texture2D> texture = MIST_TEX(texName);
    glm::vec2 bl = texture->GetTexCoords(0);
    glm::vec2 tr = texture->GetTexCoords(2);
    ImGui::Image((void*)texture->GetRendererID(), {height * texture->GetAspectRatio(), height}, ImVec2(bl.x, tr.y),
                 ImVec2(tr.x, bl.y));

    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    glm::vec2* viewportSizePtr = (glm::vec2*)&viewportSize;
    if (m_ViewportSize != *viewportSizePtr) {
        m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        m_ViewportSize = *viewportSizePtr;
        m_CameraController.OnResize(viewportSize.x, viewportSize.y);
    }
    ImGui::Image((void*)m_Framebuffer->GetColourAttachment(), viewportSize, {0, 1}, {1, 0});

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End(); // End Dockspace
}

void EditorLayer::OnEvent(Event& e) {
    MIST_PROFILE_FUNCTION();

    m_CameraController.OnEvent(e);
}
