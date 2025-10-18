#pragma once
#include "Mist/Core/Core.h"
#include "Mist/Core/Layer.h"

#include <vector>

namespace Mist {

class LayerStack {

public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Ref<Layer> layer);
    void PushOverlay(Ref<Layer> overlay);
    void PopLayer(Ref<Layer> layer);
    void PopOverlay(Ref<Layer> overlay);

    std::vector<Ref<Layer>>::iterator begin() {
        return m_Layers.begin();
    }
    std::vector<Ref<Layer>>::iterator end() {
        return m_Layers.end();
    }

private:
    std::vector<Ref<Layer>> m_Layers;
    uint32_t m_LayersInsertIndex = 0;
};

} // namespace Mist
