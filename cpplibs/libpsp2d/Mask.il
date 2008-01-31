// -*- c -*-

inline void PSP2D::Mask::set(u16 x, u16 y)
{
    *(_data + (y) * _wcap + ((x) / 32)) |= (1U << ((x) & 31));
}

inline void PSP2D::Mask::clear(u16 x, u16 y)
{
    *(_data + (y) * _wcap + ((x) / 32)) &= ~(1U << ((x) & 31));
}

inline bool PSP2D::Mask::isSet(u16 x, u16 y)
{
    return (((*(_data + (y) * _wcap + ((x) / 32))) & (1U << ((x) & 31))) != 0);
}
