#include "mistpch.h"
#include "Mist/Layer.h"

namespace Mist {

Layer::Layer(const std::string& name) :
    m_DebugName(name) {
}

Layer::~Layer() {
}

} // namespace Mist