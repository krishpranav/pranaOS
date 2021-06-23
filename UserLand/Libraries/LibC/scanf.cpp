/*
 * Copyright (c) 2021, Krisna Pranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Assertions.h>
#include <AK/Format.h>
#include <AK/GenericLexer.h>
#include <AK/StdLibExtras.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LengthModifier {
    None,
    Default,
    Char,
    Short,
    Long,
    LongLong,
    IntMax,
    Size,
    PtrDiff,
    LongDouble
};

enum ConversionSpecifier {
    Unspecified,
    Decimal,
    Integer,
    Octal,
    Unsigned,
    Hex,
    Floating,
    String,
    UseScanList,
    Character,
    Pointer,
    OutputNumberOfBytes,
    Invalid,
};

enum class ReadKind {
    Normal,
    Octal,
    Hex,
    Infer, 
};

template<typename T, typename ApT, ReadKind kind = ReadKind::Normal>
struct ReadElementConcrete {
    bool operator()(GenericLexer&, va_list)
    {
        return false;
    }
};

template<typename ApT, ReadKind kind>
struct ReadElementConcrete<int, ApT, kind> {
    bool operator()(GenericLexer& lexer, va_list* ap)
    {
        lexer.ignore_while(isspace);

        auto* ptr = ap ? va_arg(*ap, ApT*) : nullptr;
        long value = 0;
        char* endptr = nullptr;
        auto nptr = lexer.remaining().characters_without_null_termination();
        if constexpr (kind == ReadKind::Normal)
            value = strtol(nptr, &endptr, 10);
        if constexpr (kind == ReadKind::Octal)
            value = strtol(nptr, &endptr, 8);
        if constexpr (kind == ReadKind::Hex)
            value = strtol(nptr, &endptr, 16);
        if constexpr (kind == ReadKind::Infer)
            value = strtol(nptr, &endptr, 0);

        if (!endptr)
            return false;

        if (endptr == nptr)
            return false;

        auto diff = endptr - nptr;
        VERIFY(diff > 0);
        lexer.ignore((size_t)diff);

        if (ptr)
            *ptr = value;
        return true;
    }
};

template<typename ApT, ReadKind kind>
struct ReadElementConcrete<char, ApT, kind> {
    bool operator()(GenericLexer& lexer, va_list* ap)
    {
        static_assert(kind == ReadKind::Normal, "Can't read a non-normal character");

        auto* ptr = ap ? va_arg(*ap, ApT*) : nullptr;

        if (lexer.is_eof())
            return false;

        auto ch = lexer.consume();
        if (ptr)
            *ptr = ch;
        return true;
    }
};

template<typename ApT, ReadKind kind>
struct ReadElementConcrete<unsigned, ApT, kind> {
    bool operator()(GenericLexer& lexer, va_list* ap)
    {
        lexer.ignore_while(isspace);

        auto* ptr = ap ? va_arg(*ap, ApT*) : nullptr;
        unsigned long value = 0;
        char* endptr = nullptr;
        auto nptr = lexer.remaining().characters_without_null_termination();
        if constexpr (kind == ReadKind::Normal)
            value = strtoul(nptr, &endptr, 10);
        if constexpr (kind == ReadKind::Octal)
            value = strtoul(nptr, &endptr, 8);
        if constexpr (kind == ReadKind::Hex)
            value = strtoul(nptr, &endptr, 16);
        if constexpr (kind == ReadKind::Infer)
            value = strtoul(nptr, &endptr, 0);

        if (!endptr)
            return false;

        if (endptr == nptr)
            return false;

        auto diff = endptr - nptr;
        VERIFY(diff > 0);
        lexer.ignore((size_t)diff);

        if (ptr)
            *ptr = value;
        return true;
    }
};


template<typename ApT, ReadKind kind>
struct ReadElementConcrete<long long, ApT, kind> {
    bool operator()(GenericLexer& lexer, va_list* ap)
    {
        lexer.ignore_while(isspace);

        auto* ptr = ap ? va_arg(*ap, ApT*) : nullptr;
        long long value = 0;
        char* endptr = nullptr;
        auto nptr = lexer.remaining().characters_without_null_termination();
        if constexpr (kind == ReadKind::Normal)
            value = strtoll(nptr, &endptr, 10);
        if constexpr (kind == ReadKind::Octal)
            value = strtoll(nptr, &endptr, 8);
        if constexpr (kind == ReadKind::Hex)
            value = strtoll(nptr, &endptr, 16);
        if constexpr (kind == ReadKind::Infer)
            value = strtoll(nptr, &endptr, 0);

        if (!endptr)
            return false;

        if (endptr == nptr)
            return false;

        auto diff = endptr - nptr;
        VERIFY(diff > 0);
        lexer.ignore((size_t)diff);

        if (ptr)
            *ptr = value;
        return true;
    }
};

template<typename ApT, ReadKind kind>
struct ReadElementConcrete<unsigned long long, ApT, kind> {
    bool operator()(GenericLexer& lexer, va_list* ap)
    {
        lexer.ignore_while(isspace);

        auto* ptr = ap ? va_arg(*ap, ApT*) : nullptr;
        unsigned long long value = 0;
        char* endptr = nullptr;
        auto nptr = lexer.remaining().characters_without_null_termination();
        if constexpr (kind == ReadKind::Normal)
            value = strtoull(nptr, &endptr, 10);
        if constexpr (kind == ReadKind::Octal)
            value = strtoull(nptr, &endptr, 8);
        if constexpr (kind == ReadKind::Hex)
            value = strtoull(nptr, &endptr, 16);
        if constexpr (kind == ReadKind::Infer)
            value = strtoull(nptr, &endptr, 0);

        if (!endptr)
            return false;

        if (endptr == nptr)
            return false;

        auto diff = endptr - nptr;
        VERIFY(diff > 0);
        lexer.ignore((size_t)diff);

        if (ptr)
            *ptr = value;
        return true;
    }
};


template<typename ApT, ReadKind kind>
struct ReadElementConcrete<float, ApT, kind> {
    bool operator()(GenericLexer& lexer, va_list* ap)
    {
        lexer.ignore_while(isspace);

        auto* ptr = ap ? va_arg(*ap, ApT*) : nullptr;

        double value = 0;
        char* endptr = nullptr;
        auto nptr = lexer.remaining().characters_without_null_termination();
        if constexpr (kind == ReadKind::Normal)
            value = strtod(nptr, &endptr);
        else
            return false;

        if (!endptr)
            return false;

        if (endptr == nptr)
            return false;

        auto diff = endptr - nptr;
        VERIFY(diff > 0);
        lexer.ignore((size_t)diff);

        if (ptr)
            *ptr = value;
        return true;
    }
};

template<typename T, ReadKind kind>
struct ReadElement {
    bool operator()(LengthModifier length_modifier, GenericLexer& input_lexer, va_list* ap)
    {
        switch (length_modifier) {
        default:
        case None:
            VERIFY_NOT_REACHED();
        case Default:
            return ReadElementConcrete<T, T, kind> {}(input_lexer, ap);
        case Char:
            return ReadElementConcrete<T, char, kind> {}(input_lexer, ap);
        case Short:
            return ReadElementConcrete<T, short, kind> {}(input_lexer, ap);
        case Long:
            if constexpr (IsSame<T, int>)
                return ReadElementConcrete<T, long, kind> {}(input_lexer, ap);
            if constexpr (IsSame<T, unsigned>)
                return ReadElementConcrete<T, unsigned, kind> {}(input_lexer, ap);
            if constexpr (IsSame<T, float>)
                return ReadElementConcrete<int, double, kind> {}(input_lexer, ap);
            return false;
        case LongLong:
            if constexpr (IsSame<T, int>)
                return ReadElementConcrete<long long, long long, kind> {}(input_lexer, ap);
            if constexpr (IsSame<T, unsigned>)
                return ReadElementConcrete<unsigned long long, unsigned long long, kind> {}(input_lexer, ap);
            if constexpr (IsSame<T, float>)
                return ReadElementConcrete<long long, double, kind> {}(input_lexer, ap);
            return false;
        case IntMax:
            return ReadElementConcrete<T, intmax_t, kind> {}(input_lexer, ap);
        case Size:
            return ReadElementConcrete<T, size_t, kind> {}(input_lexer, ap);
        case PtrDiff:
            return ReadElementConcrete<T, ptrdiff_t, kind> {}(input_lexer, ap);
        case LongDouble:
            return ReadElementConcrete<T, long double, kind> {}(input_lexer, ap);
        }
    }
};