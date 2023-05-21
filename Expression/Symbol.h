/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#pragma once
#include "Math/Scalar.h"
#include "Utils/Array.h"

namespace Rt2::Eq
{
    class Symbol;
    using SymbolArray = SimpleArray<Symbol*>;

    enum SymbolType
    {
        None,
        Numerical,
        Identifier,
        UserFunction,
        Assignment,
        Grouping,
        Add,
        Sub,
        Mul,
        Div,
        Pow,
        Mod,
        Neg,
        Not,
        BitwiseNot,

        // math
        MathAbs,
        MathAcos,
        MathAsin,
        MathAtan,
        MathAtan2,
        MathCeil,
        MathCos,
        MathCosh,
        MathExp,
        MathFabs,
        MathFloor,
        MathFmod,
        MathLog,
        MathLog10,
        MathPow,
        MathSin,
        MathSinh,
        MathSqrt,
        MathTan,
        MathTanh,
        MathPi,
        MathE
    };

    class Symbol
    {
    private:
        SymbolType _type{None};
        Math::Real _value{0};
        String     _name{};

    public:
        Symbol() = default;
        explicit Symbol(SymbolType tok);
        ~Symbol();

        void setName(const String& str);

        void setValue(I32 integer);

        void setValue(Math::Real value);

        void setType(SymbolType value);

        SymbolType type() const;

        const String& name() const;

        Math::Real value() const;

        void print() const;

        void print(OStream& out) const;
    };

    inline Math::Real Symbol::value() const
    {
        return _value;
    }

    inline SymbolType Symbol::type() const
    {
        return _type;
    }

    inline void Symbol::setType(const SymbolType value)
    {
        _type = value;
    }

    inline void Symbol::setName(const String& str)
    {
        _name = str;
    }

    inline const String& Symbol::name() const
    {
        return _name;
    }

    inline void Symbol::setValue(const I32 integer)
    {
        _value = Math::Real(integer);
    }

    inline void Symbol::setValue(const Math::Real value)
    {
        _value = value;
    }

}  // namespace Rt2::Eq
