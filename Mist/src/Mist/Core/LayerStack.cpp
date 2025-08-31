#include "mistpch.h"
#include "LayerStack.h"

namespace Mist {

LayerStack::LayerStack() {
}

LayerStack::~LayerStack() {
    // for (Layer* layer : m_Layers)
    //     delete layer;
}

void LayerStack::PushLayer(Ref<Layer> layer) {
    m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, layer);
    m_LayersInsertIndex++;
}

void LayerStack::PushOverlay(Ref<Layer> overlay) {
    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Ref<Layer> layer) {
    auto iter = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (iter != m_Layers.end()) {
            layer->OnDetach();
        m_Layers.erase(iter);
        m_LayersInsertIndex--;
    }
}

void LayerStack::PopOverlay(Ref<Layer> overlay) {
    auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);
    if (iter != m_Layers.end()) {
        overlay->OnDetach();
        m_Layers.erase(iter);
    }
}

} // namespace Mist