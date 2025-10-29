#include "SceneHierarchyPanel.h"

#include <ImGui/imgui.h>

namespace Mist {

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
    m_Context = context;
}

void SceneHierarchyPanel::OnImGuiRender() {
    {
        ImGui::Begin("Scene Hierarchy");

        auto view = m_Context->m_Registry.view<entt::entity>();
        for (auto entityID : view) {
            Entity entity(m_Context.get(), entityID);
            DrawEntityNode(entity);
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        ImGui::End();
    }

    {
        ImGui::Begin("Properties");
        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);
        ImGui::End();
    }
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags =
        (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : NULL) | ImGuiTreeNodeFlags_OpenOnArrow;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    if (ImGui::IsItemClicked())
        m_SelectionContext = entity;

    if (opened) {
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::DrawComponents(Entity entity) {
    if (entity.HasComponent<TagComponent>()) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        static char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());

        if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
            tag = std::string(buffer);
        }
    }

    if (entity.HasComponent<TransformComponent>()) {
        ImGui::Separator();
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
                              "Transform")) {
            auto& transform = entity.GetComponent<TransformComponent>();

            glm::vec3 position = transform.GetPosition();
            if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f, 0.0f, 0.0f, "%.2f"))
                transform.SetPosition(position);

            glm::vec3 rotation = glm::degrees(transform.GetRotation());
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.5f, -180.0f, 180.0f, "%.2f",
                                  ImGuiSliderFlags_WrapAround))
                transform.SetRotation(glm::radians(rotation));

            glm::vec3 scale = transform.GetScale();
            if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f, 0.1f, 100.0f, "%.2f"))
                transform.SetScale(scale);

            ImGui::TreePop();
        }
    }

    if (entity.HasComponent<CameraComponent>()) {
        ImGui::Separator();
        if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
            auto& camera = entity.GetComponent<CameraComponent>().Camera;

            const static char* typeNames[] = {"Perspective", "Orthographic"};
            int type = (int)camera.GetType();

            if (ImGui::Combo("Camera Type", &type, typeNames, 2))
                camera.SetType((SceneCamera::CameraType)type);

            //if (ImGui::BeginCombo("Camera Type", typeNames[type])) {
            //    ImGui::EndCombo();
            //}

            float size = camera.GetOrthographicSize();
            if (ImGui::DragFloat("Orthographic Size", &size, 0.1f, 0.0f, 0.0f, "%.2f"))
                camera.SetOrthographicSize(size);

            ImGui::TreePop();
        }
    }
}

} // namespace Mist