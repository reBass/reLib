#pragma once

#include <gsl/span>

namespace re {

template <
    std::ptrdiff_t Count,
    typename ElementType,
    std::ptrdiff_t Extent,
    typename = std::enable_if_t<std::is_const<ElementType>::value>
>
constexpr gsl::span<ElementType const, Count>
subspan(const gsl::span<ElementType, Extent>& s, std::ptrdiff_t offset) {
    return {
        s.data() + offset,
        Count == gsl::dynamic_extent ? s.size() - offset : Count
    };
}

template <
    std::ptrdiff_t Count,
    typename ElementType,
    std::ptrdiff_t Extent,
    typename = std::enable_if_t<!std::is_const<ElementType>::value>
>
constexpr gsl::span<ElementType, Count>
subspan(const gsl::span<ElementType, Extent>& s, std::ptrdiff_t offset) {
    return {
        s.data() + offset,
        Count == gsl::dynamic_extent ? s.size() - offset : Count
    };
}

template <
    std::ptrdiff_t Offset,
    std::ptrdiff_t Count,
    typename ElementType,
    std::ptrdiff_t Extent,
    typename = std::enable_if_t<std::is_const<ElementType>::value>
>
constexpr gsl::span<ElementType const, Count>
subspan(const gsl::span<ElementType, Extent>& s) {
    return {
        s.data() + Offset,
        Count == gsl::dynamic_extent ? s.size() - Offset : Count
    };
}

template <
    std::ptrdiff_t Offset,
    std::ptrdiff_t Count,
    typename ElementType,
    std::ptrdiff_t Extent,
    typename = std::enable_if_t<!std::is_const<ElementType>::value>
>
constexpr gsl::span<ElementType, Count>
subspan(const gsl::span<ElementType, Extent>& s) {
    return {
        s.data() + Offset,
        Count == gsl::dynamic_extent ? s.size() - Offset : Count
    };
}

}
