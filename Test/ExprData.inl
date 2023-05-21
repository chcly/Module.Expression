#pragma once
#include "Expression/StmtParser.h"

namespace Rt2
{
    struct ExpectData
    {
        Eq::SymbolType type;
        String         name;
        Math::Real     value;
    };

    const ExpectData Parse6Values[24] = {
        {Eq::SymbolType::Identifier, "x",  0},
        { Eq::SymbolType::Numerical,  "",  0},
        { Eq::SymbolType::Numerical,  "",  1},
        { Eq::SymbolType::Numerical,  "",  2},
        { Eq::SymbolType::Numerical,  "",  3},
        { Eq::SymbolType::Numerical,  "",  4},
        {  Eq::SymbolType::Grouping,  "",  0},
        {Eq::SymbolType::Assignment,  "",  0},
        {Eq::SymbolType::Identifier, "y",  0},
        { Eq::SymbolType::Numerical,  "",  4},
        { Eq::SymbolType::Numerical,  "",  5},
        { Eq::SymbolType::Numerical,  "",  6},
        { Eq::SymbolType::Numerical,  "",  7},
        { Eq::SymbolType::Numerical,  "",  4},
        {  Eq::SymbolType::Grouping,  "",  0},
        {Eq::SymbolType::Assignment,  "",  0},
        {Eq::SymbolType::Identifier, "z",  0},
        { Eq::SymbolType::Numerical,  "",  8},
        { Eq::SymbolType::Numerical,  "",  9},
        { Eq::SymbolType::Numerical,  "", 10},
        { Eq::SymbolType::Numerical,  "", 11},
        { Eq::SymbolType::Numerical,  "",  4},
        {  Eq::SymbolType::Grouping,  "",  0},
        {Eq::SymbolType::Assignment,  "",  0},
    };

    const ExpectData Parse3Values[7] = {
        {Eq::SymbolType::Identifier, "y", 0},
        { Eq::SymbolType::Numerical,  "", 7},
        { Eq::SymbolType::Numerical,  "", 2},
        { Eq::SymbolType::Numerical,  "", 2},
        {       Eq::SymbolType::Mul,  "", 0},
        {       Eq::SymbolType::Add,  "", 0},
        {Eq::SymbolType::Assignment,  "", 0},
    };

    const ExpectData Parse2Values[13] = {
        {Eq::SymbolType::Identifier, "a", 0},
        {Eq::SymbolType::Identifier, "b", 0},
        {Eq::SymbolType::Identifier, "c", 0},
        {Eq::SymbolType::Identifier, "d", 0},
        {Eq::SymbolType::Identifier, "e", 0},
        {Eq::SymbolType::Identifier, "f", 0},
        { Eq::SymbolType::Numerical,  "", 1},
        {Eq::SymbolType::Assignment,  "", 0},
        {Eq::SymbolType::Assignment,  "", 0},
        {Eq::SymbolType::Assignment,  "", 0},
        {Eq::SymbolType::Assignment,  "", 0},
        {Eq::SymbolType::Assignment,  "", 0},
        {Eq::SymbolType::Assignment,  "", 0},
    };

    const ExpectData Parse1Values[8] = {
        {Eq::SymbolType::Identifier, "y", 0},
        { Eq::SymbolType::Numerical,  "", 0},
        { Eq::SymbolType::Numerical,  "", 1},
        { Eq::SymbolType::Numerical,  "", 2},
        { Eq::SymbolType::Numerical,  "", 3},
        { Eq::SymbolType::Numerical,  "", 4},
        {  Eq::SymbolType::Grouping,  "", 0},
        {Eq::SymbolType::Assignment,  "", 0},
    };

}  // namespace Rt2

void ExpectDataTest(const Rt2::Eq::SymbolArray& actual,
                    const Rt2::ExpectData*      expected,
                    size_t                      size);
void logSymbols(const Rt2::Eq::SymbolArray& sym);
