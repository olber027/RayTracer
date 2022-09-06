//
// Created by olber on 9/4/2022.
//

#pragma once
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <type_traits>

namespace utility
{
    template<typename T, size_t SIZE>
    struct bigEndianHelper;

    template<typename T>
    struct bigEndianHelper<T, 1> {
        static T convert(const T t) { return t; }
    };

    template<typename T>
    struct bigEndianHelper<T, 2> {
        static T convert(const T t) { return htons(t); }
    };

    template<typename T>
    struct bigEndianHelper<T, 4> {
        static T convert(const T t) { return htonl(t); }
    };

    template <typename T>
    [[nodiscard]] T ToBigEndian(const T t)
    {
        static_assert(std::is_integral_v<T>, "T must be an integral type");
        return bigEndianHelper<T, sizeof(T)>::convert(t);
    }


    template<typename T, size_t SIZE>
    struct littleEndianHelper;

    template<typename T>
    struct littleEndianHelper<T, 1> {
        static T convert(const T t) { return t; }
    };

    template<typename T>
    struct littleEndianHelper<T, 2> {
        static T convert(const T t) { return ntohs(t); }
    };

    template<typename T>
    struct littleEndianHelper<T, 4> {
        static T convert(const T t) { return ntohl(t); }
    };

    template <typename T>
    [[nodiscard]] T ToLittleEndian(const T t)
    {
        static_assert(std::is_integral_v<T>, "T must be an integral type");
        return littleEndianHelper<T, sizeof(T)>::convert(t);
    }
}