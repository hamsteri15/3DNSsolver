#pragma once

struct AxisIndex{

    constexpr AxisIndex() = default; 
    constexpr AxisIndex(size_t i) : m_val(i) {}

    constexpr operator size_t&() { return m_val; }
    constexpr operator size_t() const { return m_val; }

private:
    size_t m_val;

};
