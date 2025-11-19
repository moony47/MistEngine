#pragma once

#include <Mist.h>

namespace Mist {

class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void OnImGuiRender();

    inline void SetSelectedEntity(uint32_t entity) {
        m_SelectionContext = {m_Context.get(), (entt::entity) entity};
    }
    inline Entity GetSelectedEntity() const {
        return m_SelectionContext;
    }

private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);

private:
    Ref<Scene> m_Context;
    Entity m_SelectionContext;
};

} // namespace Mist