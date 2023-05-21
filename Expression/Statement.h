#pragma once
#include "Expression/StackValue.h"
#include "Expression/StatementParser.h"

namespace Rt2::Eq
{
    typedef  double (*WrapFuncA1)(double a1);
    typedef  double (*WrapFuncA2)(double a1, double a2);

    class Statement
    {
    private:
        EvalStack     _stack;
        EvalHash      _table;
        EvalGroupHash _groups;
        U32           _hashCount{InitialHash};

        void push(const Math::Real&    v,
                  const size_t& idx  = Npos,
                  U8            flag = StackValue::Value);

        void push(const Symbol* sy);

        void store(const Symbol* sym);

        void add();
        void sub();
        void neg();
        void mul();
        void div();
        void mod();
        void pow();
        void group();
        void assign();

        void mathFncA1(WrapFuncA1 f);
        void mathFncA2(WrapFuncA2 f);
        
        void eval(const Symbol* sy);

        Math::Real executeImpl(const SymbolArray& val);

        template <typename... Args>
        [[noreturn]] void error(Args&&... args);

        [[noreturn]] void argError(const char*);

    public:
        Statement() = default;
        ~Statement();

        void set(const String& name, Math::Real value);
        void set(VInt index, Math::Real value);

        VInt indexOf(const String& name) const;

        Math::Real get(const String& name, Math::Real def = 0);

        Math::Real peek(I32 idx);

        void get(const String& name, ValueList& dest);

        Math::Real execute(const SymbolArray& val);
    };

    template <typename... Args>
    void Statement::error(Args&&... args)
    {
        OutputStringStream stream;
        ((stream << std::forward<Args>(args)), ...);
        throw Exception(stream.str());
    }

}  // namespace Jam::Eq
