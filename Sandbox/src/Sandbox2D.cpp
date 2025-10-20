#include "Sandbox2D.h"

static const size_t s_MapWidth = 24;
static const char* s_MapTiles = "WWWWWWWWWWWWWWWWWWWWWWWW"
                                "WWWWWWGGGGGGGGGGGGWWWWWW"
                                "WWWWWGGGGGGGGGGGGGGWWWWW"
                                "WWWWGGGGGGGWWWWGGGGGWWWW"
                                "WWWGGGGGWWWWWWGGGGGGWWWW"
                                "WWGGGGGGWWWWWGGGGGGGWWWW"
                                "WWGGGGGGWWWWWWGGGGGWWWWW"
                                "WWGGGGGGGGWWWGGGGGWWWWWW"
                                "WWGGGGGGGGGGGGGGGWWWGGWW"
                                "WWWGGGGGGGGGGGGGWWWGGGWW"
                                "WWWWGGGGGGGGGGWWWWGGGWWW"
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

    if (pos != strlen(s_MapTiles) && s_MapTiles[pos + 1] != chr) // E
        *str += 'E';
    else if (pos != 0 && s_MapTiles[pos - 1] != chr) // W
        *str += 'W';

    if (str->length() == 1) {
        // NE
        if (pos - s_MapWidth + 1 >= 0 && s_MapTiles[pos - s_MapWidth + 1] != chr)
            *str += '1';
        // NW
        else if (pos - s_MapWidth - 1 >= 0 && s_MapTiles[pos - s_MapWidth - 1] != chr)
            *str += '2';
        // SE
        else if (pos + s_MapWidth + 1 < strlen(s_MapTiles) && s_MapTiles[pos + s_MapWidth + 1] != chr)
            *str += '3';
        // SW
        else if (pos + s_MapWidth - 1 < strlen(s_MapTiles) && s_MapTiles[pos + s_MapWidth - 1] != chr)
            *str += '4';
    }
    return str;
}

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"),
    m_CameraController(0.0f, 0.0f, 0.0f, 16.0f / 9.0f, true) {
}

Sandbox2D::~Sandbox2D() {
}

void Sandbox2D::OnAttach() {
    MIST_PROFILE_FUNCTION();

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

void Sandbox2D::OnDetach() {
    MIST_PROFILE_FUNCTION();

    MIST_TEXLIB->Remove("SpriteSheet", true);
}

void Sandbox2D::OnUpdate(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(deltaTime);

    Mist::Renderer2D::ResetStats();

    m_Framebuffer->Bind();

    RenderCommand::SetClearColour(glm::vec4{0.1f, 0.1f, 0.1f, 1.0f});
    Renderer2D::BeginScene(m_CameraController.GetCamera());

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

void Sandbox2D::OnImGuiRender(DeltaTime deltaTime) {
    MIST_PROFILE_FUNCTION();

    ImGui::Begin("Sandbox2D");
    ImGui::ColorEdit4("Start Colour", glm::value_ptr(m_SpriteColour), ImGuiColorEditFlags_Float);
    ImGui::End();
}

void Sandbox2D::OnEvent(Event& e) {
    MIST_PROFILE_FUNCTION();

    m_CameraController.OnEvent(e);
}
