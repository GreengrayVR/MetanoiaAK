#pragma once

#include <cstddef>
#include <bit>

template <typename TRet, typename TVal>
[[nodiscard]] static inline constexpr TRet* poff(TVal* ptr, ptrdiff_t offset = 0)
{
    return std::bit_cast<TRet*>(std::bit_cast<char*>(ptr) + offset);
}
template <typename TRet, typename TVal>
[[nodiscard]] static inline constexpr const TRet* poff(const TVal* ptr, ptrdiff_t offset = 0)
{
    return std::bit_cast<const TRet*>(std::bit_cast<const char*>(ptr) + offset);
}

template <typename TVal>
[[nodiscard]] static inline constexpr TVal* poff(TVal* ptr, ptrdiff_t offset = 0)
{
    return poff<TVal, TVal>(ptr, offset);
}
template <typename TVal>
[[nodiscard]] static inline constexpr const TVal* poff(const TVal* ptr, ptrdiff_t offset = 0)
{
    return poff<TVal, TVal>(ptr, offset);
}

template <typename TRet, typename TVal>
[[nodiscard]] static inline constexpr TRet voff(const TVal* ptr, ptrdiff_t offset = 0)
{
    return *poff<TRet, TVal>(ptr, offset);
}

template <typename TRet, typename TVal>
[[nodiscard]] static inline constexpr TRet& roff(TVal* ptr, ptrdiff_t offset = 0)
{
    return *poff<TRet, TVal>(ptr, offset);
}
template <typename TRet, typename TVal>
[[nodiscard]] static inline constexpr const TRet& roff(const TVal* ptr, ptrdiff_t offset = 0)
{
    return *poff<TRet, TVal>(ptr, offset);
}

template <typename>
struct _fcast {};

template <typename TRet, typename... TArgs>
struct _fcast<TRet(TArgs...)>
{
    using pointer = TRet(*)(TArgs...);
};

template <typename TRet, typename TVal>
[[nodiscard]] static inline constexpr typename _fcast<TRet>::pointer foff(TVal* ptr, ptrdiff_t offset = 0)
{
    return std::bit_cast<typename _fcast<TRet>::pointer>(poff<void, TVal>(ptr, offset));
}
template <typename TRet, typename TVal>
[[nodiscard]] static inline constexpr const typename _fcast<TRet>::pointer foff(const TVal* ptr, ptrdiff_t offset = 0)
{
    return std::bit_cast<const typename _fcast<TRet>::pointer>(poff<void, TVal>(ptr, offset));
}