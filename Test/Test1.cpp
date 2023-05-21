#include <cstdio>
#include "ExprData.inl"
#include "Expression/Stmt.h"
#include "Expression/StmtParser.h"
#include "Expression/StmtScanner.h"
#include "ThisDir.h"
#include "Utils/StreamMethods.h"
#include "gtest/gtest.h"
#include "Math/Math.h"

using namespace Rt2::Math;
using namespace Rt2::Eq;
using namespace Rt2;

///////////////////////////////////////////////////////////////////////////////
constexpr Real ConsolePi = Real(3.1415926535897932);
constexpr Real Exit      = Real(3.018392941684311);

GTEST_TEST(Expression, Parse00c)
{
    StringStream ss;
    ss << "1000*x";

    StmtParser code;
    code.read(ss);
    logSymbols(code.symbols());

    Stmt eval;
    eval.set("x", 2);
    eval.execute(code.symbols());
    EXPECT_DOUBLE_EQ((int)eval.peek(0), 2000.0);
}

GTEST_TEST(Expression, Parse00b)
{
    StringStream ss;
    ss << "mod(cos(x), 3)";

    StmtParser code;
    code.read(ss);
    logSymbols(code.symbols());

    Stmt eval;
    eval.set("x", Real(Pi));
    eval.execute(code.symbols());
    EXPECT_DOUBLE_EQ((int)eval.peek(0), 2.0);
}

GTEST_TEST(Expression, Parse00a)
{
    StringStream ss;
    ss << "mod(x,b)";

    StmtParser code;
    code.read(ss);
    logSymbols(code.symbols());

    Stmt eval;
    eval.set("b", 5);

    int i = 0;
    do
    {
        eval.set("x", i++);
    } while (eval.execute(code.symbols()) <= 3.0 && i < 10);

    EXPECT_EQ(i, 5);
    EXPECT_DOUBLE_EQ(eval.peek(0), 4.0);
}

GTEST_TEST(Expression, Parse009)
{
    StringStream ss;

    ss << "(3.1415926535897932*x-a)/(x+b)";
    StmtParser parse;
    parse.read(ss);

    constexpr Real a = 1;
    constexpr Real b = 1;

    Stmt eval;
    int  i = 0;

    eval.set("x", i);
    eval.set("a", a);
    eval.set("b", b);

    eval.execute(parse.symbols());
    while (eval.peek(0) < Exit)
    {
        const Real x = Real(i);
        eval.set("x", x);

        EXPECT_DOUBLE_EQ(
            //
            eval.execute(parse.symbols()),
            //
            (3.1415926535897932 * x - a) / (x + b));

        ++i;
    }

    EXPECT_EQ(i, 34);
}

GTEST_TEST(Expression, Parse008)
{
    StringStream ss;
    ss << "1.0 + 9.0 / 10.0 + 9.0 / 100.0 + 6.0 / 1000.0 + 5.0 / 10000.0";
    StmtParser parse;
    parse.read(ss);
    logSymbols(parse.symbols());
    Stmt eval;
    Real result = eval.execute(parse.symbols());
    //Console::println(SetD({1.9965, result}, 0, 4, true));
    EXPECT_DOUBLE_EQ(result, 1.9965);

    ss.str("(1-2^(-24))*2^128");
    ss.clear();

    parse.read(ss);
    logSymbols(parse.symbols());
    result = eval.execute(parse.symbols());
    EXPECT_DOUBLE_EQ(result, (1 - pow(2, -24)) * pow(2, 128));
}

GTEST_TEST(Expression, Parse007)
{
    StringStream ss;
    ss << "a=sin(x/2),b=4*atan(1)";

    StmtParser parse;
    parse.read(ss);
    logSymbols(parse.symbols());

    Stmt eval;
    eval.set("x", 3.1415926535897932384626433832795);
    eval.execute(parse.symbols());

    EXPECT_DOUBLE_EQ(eval.get("a"), 1.0);
    EXPECT_DOUBLE_EQ(eval.get("b"), 3.1415926535897932384626433832795);
}
///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse006)
{
    StringStream ss;
    ss << "x={0,1,2,3}, y=[4,5,6,7], z={8,9,10,11}";

    StmtParser parse;
    parse.read(ss);

    const SymbolArray& exec = parse.symbols();
    logSymbols(exec);

    ExpectDataTest(exec, Parse6Values, exec.size());

    Stmt       eval;
    const Real v = eval.execute(exec);
    EXPECT_EQ(v, Eq::InitialHash + 2);

    ValueList values[3] = {{}, {}, {}};

    eval.get("x", values[0]);
    EXPECT_EQ(eval.get("x"), Eq::InitialHash);

    eval.get("y", values[1]);
    EXPECT_EQ(eval.get("y"), Eq::InitialHash + 1);

    eval.get("z", values[2]);
    EXPECT_EQ(eval.get("z"), Eq::InitialHash + 2);

    int j = 0;
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(values[i][0], j);
        EXPECT_EQ(values[i][1], j + 1);
        EXPECT_EQ(values[i][2], j + 2);
        EXPECT_EQ(values[i][3], j + 3);
        j += 4;
    }
}

///////////////////////////////////////////////////////////////////////////////

void TestDouble(Real a, Real b)
{
    if (std::isnan(a) && std::isnan(b))
        return;

    EXPECT_DOUBLE_EQ(b, a);
}

GTEST_TEST(Expression, Parse004)
{
    StringStream ss;
    ss << "y = 7+2*2";

    StmtParser parse;
    parse.read(ss);
    const SymbolArray& exec = parse.symbols();
    EXPECT_EQ(exec.size(), 7);
    logSymbols(exec);

    Stmt eval;
    EXPECT_EQ(eval.execute(exec), 11);
    EXPECT_EQ(eval.get("y"), 11);
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse003)
{
    StringStream ss;
    ss << "y = 7+2*2";

    StmtParser parse;
    parse.read(ss);
    const SymbolArray& exec = parse.symbols();
    logSymbols(exec);

    EXPECT_EQ(exec.size(), 7);
    ExpectDataTest(exec, Parse3Values, exec.size());
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse002)
{
    StringStream ss;
    ss << "# Tests recursive assignment.." << std::endl;
    ss << "a=b=c=d=e=f=1";

    StmtParser parse;
    parse.read(ss);
    const SymbolArray& exec = parse.symbols();
    logSymbols(exec);

    EXPECT_EQ(exec.size(), 13);
    ExpectDataTest(exec, Parse2Values, exec.size());

    Stmt eval;
    EXPECT_EQ(eval.execute(exec), 1);
    EXPECT_EQ(eval.get("a", -1), 1);
    EXPECT_EQ(eval.get("b", -1), 1);
    EXPECT_EQ(eval.get("c", -1), 1);
    EXPECT_EQ(eval.get("d", -1), 1);
    EXPECT_EQ(eval.get("e", -1), 1);
    EXPECT_EQ(eval.get("f", -1), 1);
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Parse001)
{
    StringStream ss;
    ss << "y = [0,1,2,3]";

    StmtParser parse;
    parse.read(ss);
    const SymbolArray& exec = parse.symbols();

    logSymbols(exec);
    EXPECT_EQ(exec.size(), 8);
    ExpectDataTest(exec, Parse1Values, exec.size());

    Stmt eval;
    EXPECT_EQ(eval.execute(exec), Eq::InitialHash);

    ValueList v;
    eval.get("y", v);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 3);
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Scan2)
{
    const String fileName = TestFile("scan3.eq");

    InputFileStream fs;
    fs.open(fileName);
    EXPECT_TRUE(fs.is_open());

    StmtScanner sc;
    sc.attach(&fs, PathUtil(fileName));

    Token  tok;
    int8_t val = TOK_KW_ST;
    while (val != TOK_EOF)
    {
        sc.scan(tok);
        val = tok.type();
        if (val != TOK_EOF)
        {
            bool test = val >= TOK_KW_ST && val <= TOK_KW_EN;
            EXPECT_EQ(test, true);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Scan1)
{
    const String fileName = TestFile("scan1.eq");

    InputFileStream fs;
    fs.open(fileName);
    EXPECT_TRUE(fs.is_open());

    StmtScanner sc;
    sc.attach(&fs, PathUtil(fileName));

    constexpr TokenType exp[] = {
        TOK_MUL,
        TOK_DIV,
        TOK_PLUS,
        TOK_MINUS,
        TOK_O_PAR,
        TOK_C_PAR,
        TOK_O_BRACKET,
        TOK_C_BRACKET,
        TOK_O_BRACE,
        TOK_C_BRACE,
        TOK_COMMA,
        TOK_PERIOD,
        TOK_EQUALS,
        TOK_POWS,
    };

    Token tok;
    for (auto type : exp)
    {
        sc.scan(tok);
        EXPECT_EQ(tok.type(), type);
    }
}

///////////////////////////////////////////////////////////////////////////////

GTEST_TEST(Expression, Scan0)
{
    const String Scan0 = TestFile("scan0.eq");

    InputFileStream fs;
    fs.open(Scan0);
    EXPECT_TRUE(fs.is_open());

    StmtScanner sc;
    sc.attach(&fs, PathUtil(Scan0));

    Token tok;
    sc.scan(tok);
    EXPECT_EQ(tok.type(), Eq::TOK_FLOAT);
    EXPECT_EQ(123, sc.real(tok.index()));

    sc.scan(tok);
    EXPECT_EQ(tok.type(), Eq::TOK_MUL);

    sc.scan(tok);
    EXPECT_EQ(tok.type(), Eq::TOK_FLOAT);
    EXPECT_EQ(987, sc.real(tok.index()));

    sc.scan(tok);
    EXPECT_EQ(tok.type(), Eq::TOK_EOF);
}

///////////////////////////////////////////////////////////////////////////////

void logSymbols(const SymbolArray& sym)
{
    //for (const auto element : sym)
    //    element->print();
    //Console::writeLine("");
}

void ExpectDataTest(const SymbolArray& actual,
                    const ExpectData*  expected,
                    size_t             size)
{
    for (U32 i = 0; i < size; ++i)
    {
        EXPECT_EQ(actual.at(i)->type(), expected[i].type);

        if (expected[i].type == Identifier)
            EXPECT_EQ(actual.at(i)->name(), expected[i].name);
        else if (expected[i].type == Numerical)
            EXPECT_EQ(actual.at(i)->value(), expected[i].value);
    }
}
