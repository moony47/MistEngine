#include "mistpch.h"
#include "LayerStack.h"

namespace Mist {

LayerStack::LayerStack() {
    m_LayersInsert = m_Layers.begin();
}

LayerStack::~LayerStack() {
    for (Layer* layer : m_Layers)
        delete layer;
}

void LayerStack::PushLayer(Layer* layer) {
    m_LayersInsert = m_Layers.emplace(m_LayersInsert, layer);
}

void LayerStack::PushOverlay(Layer* overlay) {
    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer) {
    auto iter = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (iter != m_Layers.end()) {
        m_Layers.erase(iter);
        m_LayersInsert--;
    }
}

void LayerStack::PopOverlay(Layer* overlay) {
    auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);
    if (iter != m_Layers.end())
        m_Layers.erase(iter);
}

} // namespace Mist