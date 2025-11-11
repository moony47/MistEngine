#include "SceneHierarchyPanel.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

namespace Mist {

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
    m_Context = context;
}

void SceneHierarchyPanel::OnImGuiRender() {
    {
        //ImGui::SetNextWindowSizeConstraints({200.0f, 300.0f}, {999999.0f, 999999.0f});
        ImGui::Begin("Scene Hierarchy");

        for (auto entityID : m_Context->m_Registry.view<entt::entity>()) {
            Entity entity(m_Context.get(), entityID);
            DrawEntityNode(entity);
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        // Blank space context menu
        if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
            if (ImGui::MenuItem("New Entity"))
                m_Context->CreateEntity("Empty");

            ImGui::EndPopup();
        }

        ImGui::End();
    }

    {
        //ImGui::SetNextWindowSizeConstraints({370.0f, 400.0f}, {999999.0f, 999999.0f});
        ImGui::Begin("Properties");
        if (m_SelectionContext) {
            DrawComponents(m_SelectionContext);
        }
        ImGui::End();
    }
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags =
        (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : NULL) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    if (ImGui::IsItemClicked())
        m_SelectionContext = entity;

    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem()) {
        entityDeleted = ImGui::MenuItem("Delete Entity", 0, false, *m_Context->m_PrimaryCameraEntity != entity);
        ImGui::EndPopup();
    }

    if (opened) {
        // TODO: Display children
        ImGui::TreePop();
    }

    if (entityDeleted) {
        m_Context->DestroyEntity(entity);
        if (m_SelectionContext == entity)
            m_SelectionContext = {};
    }
}

static bool DrawVec3Control(const std::string& label,
                            glm::vec3& values,
                            float resetValue = 0.0f,
                            float columnWidth = 100.0f) {
    bool modified = false;
    float lineHeight = 24.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);

    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float barWidth = ImGui::CalcItemWidth() / 3;

    ImGui::PushItemWidth(barWidth);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        modified = true;
        values.x = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    modified |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(barWidth);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.65f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.75f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.65f, 0.3f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        modified = true;
        values.y = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    modified |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(barWidth);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        modified = true;
        values.z = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    modified |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");

    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();

    return modified;
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction) {
    static const ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth |
        ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

    if (entity.HasComponent<T>()) {
        auto& component = entity.GetComponent<T>();
        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

        ImGui::PushID(name.c_str());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4.0f, 4.0f});
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());
        ImGui::PopFont();
        ImGui::PopStyleVar();

        float lineHeight = 25.0f;

        ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
        if (ImGui::Button("...", {lineHeight, lineHeight}))
            ImGui::OpenPopup("ComponentSettings");

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (!std::is_same<TransformComponent, T>::value)
                removeComponent = ImGui::MenuItem("Remove Component");
            ImGui::EndPopup();
        }

        if (open) {
            uiFunction(entity, component);
            ImGui::TreePop();
        }

        ImGui::PopID();

        if (removeComponent)
            entity.RemoveComponent<T>();
    }
}

void SceneHierarchyPanel::DrawComponents(Entity entity) {
    if (entity.HasComponent<TagComponent>()) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        static char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());

        if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            tag = std::string(buffer);
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent")) {
        if (ImGui::MenuItem("Camera")) {
            m_SelectionContext.AddComponent<CameraComponent>();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Sprite")) {
            m_SelectionContext.AddComponent<SpriteComponent>();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>("Transform", entity, [](Entity entity, TransformComponent& comp) {
        glm::vec3 position = comp.Position;
        if (DrawVec3Control("Position", position))
            comp.SetPosition(position);

        glm::vec3 rotation = glm::degrees(comp.Rotation);
        if (DrawVec3Control("Rotation", rotation))
            comp.SetRotation(glm::radians(rotation));

        glm::vec3 scale = comp.Scale;
        if (DrawVec3Control("Scale", scale, 1.0f))
            comp.SetScale(scale);
    });

    DrawComponent<CameraComponent>("Camera", entity, [&](Entity entity, CameraComponent& comp) {
        auto& camera = comp.Camera;

        if (ImGui::Button("Make Primary"))
            m_Context->SetPrimaryCamera(entity);

        const static char* typeNames[] = {"Perspective", "Orthographic"};
        int type = (int)camera.GetType();

        if (ImGui::Combo("Camera Type", &type, typeNames, 2))
            camera.SetType((SceneCamera::CameraType)type);

        if ((SceneCamera::CameraType)type == SceneCamera::CameraType::Orthographic) {
            float size = camera.GetOrthographicSize();
            if (ImGui::DragFloat("Orthographic Size", &size, 0.1f, 0.0f, 0.0f, "%.2f"))
                camera.SetOrthographicSize(size);
            float nearClip = camera.GetOrthographicNear();
            if (ImGui::DragFloat("Near Clip", &nearClip, 0.1f, 0.0f, 0.0f, "%.2f"))
                camera.SetOrthographicNear(nearClip);
            float farClip = camera.GetOrthographicFar();
            if (ImGui::DragFloat("Far Clip", &farClip, 0.1f, 0.0f, 0.0f, "%.2f"))
                camera.SetOrthographicFar(farClip);
        } else {
            float fov = glm::degrees(camera.GetPerspectiveFOV());
            if (ImGui::DragFloat("Field of View", &fov, 0.1f, 0.0f, 0.0f, "%.2f"))
                camera.SetPerspectiveFOV(glm::radians(fov));
            float nearClip = camera.GetPerspectiveNear();
            if (ImGui::DragFloat("Near Clip", &nearClip, 0.1f, 0.0f, 0.0f, "%.2f"))
                camera.SetPerspectiveNear(nearClip);
            float farClip = camera.GetPerspectiveFar();
            if (ImGui::DragFloat("Far Clip", &farClip, 0.1f, 0.0f, 0.0f, "%.2f"))
                camera.SetPerspectiveFar(farClip);
        }
    });

    DrawComponent<SpriteComponent>("Sprite", entity, [](Entity entity, SpriteComponent& comp) {
        ImGui::ColorEdit4("Colour", value_ptr(comp.Colour));

        if (ImGui::BeginCombo("Texture", comp.TextureName.c_str())) {
            for (auto iter = MIST_TEXLIB->Begin(); iter != MIST_TEXLIB->End(); iter++) {
                bool isSelected = comp.TextureName == iter->first;
                if (ImGui::Selectable(iter->first.c_str(), isSelected))
                    comp.TextureName = iter->first;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    });
}

} // namespace Mist