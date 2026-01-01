#pragma once

namespace Mist {

class UUID {
public:
    UUID();
    UUID(uint64_t id);
    UUID(const UUID&) = default;

    operator uint64_t() const {
        return m_ID;
    }

    std::string ToString() const {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(12) << std::hex << m_ID << std::dec;
        return ss.str();
    }

private:
    uint64_t m_ID;
};

} // namespace Mist

template <typename T>
struct std::hash;

template <>
struct std::hash<Mist::UUID> {
    std::size_t operator()(const Mist::UUID& uuid) const {
        return uuid;
    }
};

template <>
struct fmt::formatter<Mist::UUID> : fmt::ostream_formatter {};

inline std::string format_as(const Mist::UUID& uuid) {
    return uuid.ToString();
}

inline std::ostream& operator<<(std::ostream& os, const Mist::UUID& uuid) {
    return os << uuid.ToString();
}
