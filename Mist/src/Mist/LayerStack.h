#pragma once
#include "Layer.h"
#include "Mist/Core.h"

#include <vector>

namespace Mist {

class MIST_API LayerStack {
    using LayersIterator = std::vector<Layer*>::iterator;

public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    LayersIterator begin() {
        return m_Layers.begin();
    }
    LayersIterator end() {
        return m_Layers.end();
    }

private:
    std::vector<Layer*> m_Layers;
    LayersIterator m_LayersInsert;
};

} // namespace Mist
