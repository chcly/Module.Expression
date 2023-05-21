#include "Expression/Statement.h"
#include "Math/Math.h"
#include "Utils/StreamMethods.h"

namespace Rt2::Eq
{

    OStream& operator<<(OStream& out, const StackValue& v)
    {
        out << "{ ";
        out << "c:" << Hex(v.c);
        out << ", ";
        out << "f:" << Hex(v.f);
        out << ", ";
        out << "v:" << SetD({v.v}, sizeof(double) << 1, 14);
        out << " }";
        return out;
    }

    void trace(EvalStack&    stack,
               const String& message,
               const bool    topToBottom = true)
    {
        const int iOffs = stack.sizeI();

        Console::println(Tab(4), message);

        for (int i = 0; i < iOffs; ++i)
        {
            int c = iOffs - i - 1;
            if (!topToBottom)
                c = i;
            Console::println(Tab(4), SetI({i}), ':', ' ', stack[c]);
        }
    }

    void Statement::push(const Math::Real& v, const size_t& idx, const U8 flag)
    {
        _stack.push({v, idx, flag});
    }

    void Statement::push(const Symbol* sy)
    {
        push(sy->value());
    }

    void Statement::store(const Symbol* sym)
    {
        size_t idx = _table.find(sym->name());
        if (idx == Npos)
        {
            _table.insert(sym->name(), {sym->value()});
            idx = _table.find(sym->name());
        }
        push(_table.at(idx).v, idx, StackValue::Id);
    }

    void Statement::add()
    {
        if (_stack.size() > 1)
        {
            const Math::Real& b = _stack.popTop().v;
            const Math::Real& a = _stack.popTop().v;
            push(a + b);
        }
        else
            argError("add");
    }

    void Statement::sub()
    {
        if (_stack.size() > 1)
        {
            const Math::Real& b = _stack.popTop().v;
            const Math::Real& a = _stack.popTop().v;
            push(a - b);
        }
        else
            argError("sub");
    }

    void Statement::neg()
    {
        if (_stack.isNotEmpty())
        {
            const Math::Real& a = _stack.popTop().v;
            push(-a);
        }
        else
            argError("neg");
    }

    void Statement::mul()
    {
        if (_stack.size() > 1)
        {
            const Math::Real& b = _stack.popTop().v;
            const Math::Real& a = _stack.popTop().v;
            push(a * b);
        }
        else
            argError("mul");
    }

    void Statement::div()
    {
        if (_stack.size() > 1)
        {
            Math::Real        b = _stack.popTop().v;
            const Math::Real& a = _stack.popTop().v;
            if (abs(b) > DBL_EPSILON)
                b = 1.0 / b;
            else
                b = NAN;

            push(a * b);
        }
        else
            argError("div");
    }

    void Statement::mod()
    {
        if (_stack.size() > 1)
        {
            const Math::Real& b = _stack.popTop().v;
            const Math::Real& a = _stack.popTop().v;
            push(Math::Real(fmod(a, b)));
        }
        else
            argError("mod");
    }

    void Statement::pow()
    {
        if (_stack.size() > 1)
        {
            const Math::Real& b = _stack.popTop().v;
            const Math::Real& a = _stack.popTop().v;
            push(::pow(a, b));
        }
        else
            argError("pow");
    }

    void Statement::group()
    {
        if (_stack.size() > 1)
        {
            const auto& [v, c, f] = _stack.popTop();
            if (const U8 nr = U8(v);
                _stack.size() > nr && nr > 0)
            {
                ValueGrouping* vg = new ValueGrouping();
                for (U8 i = 0; i < nr; ++i)
                    vg->push_back(_stack.popTop());

                _groups.insert(_hashCount, vg);

                _stack.push({
                    Math::Real(_hashCount),
                    _hashCount,
                    StackValue::List,
                });

                _hashCount++;
            }
        }
        else
            argError("group");
    }

    void Statement::assign()
    {
        if (_stack.size() > 1)
        {
            StackValue       c{NAN, Npos, 0};
            const StackValue b = _stack.popTop();
            const StackValue a = _stack.popTop();

            // a = b
            if (b.isId())
            {
                if (b.c < _table.size())
                {
                    c.v = _table[b.c].v;
                    c.c = b.c;
                    c.f = StackValue::Id;
                }
            }
            else if (b.isList())
            {
                c.v = Math::Real(b.c);
                c.c = b.c;
                c.f = StackValue::List;
            }
            else
            {
                c.v = b.v;
                c.c = Npos;
                c.f = StackValue::Value;
            }

            if (a.isId())
            {
                if (a.c < _table.size())
                    _table[a.c] = c;
            }

            _stack.push(c);
        }
        else
            argError("assign");
    }

    void Statement::mathFncA1(WrapFuncA1 f)
    {
        if (_stack.size() > 1)
        {
            if (_stack.popTop().integer() != 1)
            {
                error(
                    "expected one argument to the "
                    "supplied math function");
            }
            const Math::Real& a = _stack.popTop().v;
            push(f(a));
        }
        else
            error(
                "supplied math function requires "
                "at least two elements on the stack.");
    }

    void Statement::mathFncA2(WrapFuncA2 f)
    {
        if (_stack.size() > 2)
        {
            if (_stack.popTop().integer() != 2)
            {
                error(
                    "expected two argument to the "
                    "supplied math function");
            }
            const Math::Real& b = _stack.popTop().v;
            const Math::Real& a = _stack.popTop().v;
            push(f(a, b));
        }
        else
            error(
                "supplied math function requires "
                "at least three elements on the stack.");
    }

    double lMod(const double a, const double b)
    {
        const double r = remainder(a, b);
        return r < 0 ? b + r : r;
    }

    void Statement::eval(const Symbol* sy)
    {
        // clang-format off
    switch (sy->type()) {
    case Numerical  : push(sy);         break;
    case Identifier : store(sy);        break;
    case MathPi     : push(Math::Pi);   break;
    case MathE      : push(Math::E);    break;
    case Add        : add();            break;
    case Sub        : sub();            break;
    case Neg        : neg();            break;
    case Mul        : mul();            break;
    case Div        : div();            break;
    case Pow        : pow();            break;
    case Mod        : mod();            break;
    case Assignment : assign();         break;
    case Grouping   : group();          break;
    case MathSin    : mathFncA1(sin);   break;
    case MathAtan   : mathFncA1(atan);  break;
    case MathAbs    : mathFncA1(fabs);  break;
    case MathAcos   : mathFncA1(acos);  break;
    case MathAsin   : mathFncA1(asin);  break;
    case MathAtan2  : mathFncA2(atan2); break;
    case MathCeil   : mathFncA1(ceil);  break;
    case MathCos    : mathFncA1(cos);   break;
    case MathCosh   : mathFncA1(cosh);  break;
    case MathExp    : mathFncA1(exp);   break;
    case MathFabs   : mathFncA1(fabs);  break;
    case MathFloor  : mathFncA1(floor); break;
    case MathFmod   : mathFncA2(lMod);  break;
    case MathLog    : mathFncA1(log);   break;
    case MathLog10  : mathFncA1(log10); break;
    case MathPow    : mathFncA2(::pow); break;
    case MathSinh   : mathFncA1(sinh);  break;
    case MathSqrt   : mathFncA1(sqrt);  break;
    case MathTan    : mathFncA1(tan);   break;
    case MathTanh   : mathFncA1(tanh);  break;
    case UserFunction: 
    case None:
    case Not:
    case BitwiseNot:
    default:
        break;
    }
        // clang-format on
    }

    Math::Real Statement::executeImpl(const SymbolArray& val)
    {
        _stack.resizeFast(0);
        for (const auto& sy : val)
            eval(sy);
        // trace(_stack, "RESULTS");
        return _stack.empty() ? 0 : _stack.top().v;
    }

    void Statement::argError(const char* op)
    {
        error(
            "not enough arguments "
            "supplied to the '",
            op,
            "' operation ");
    }

    Math::Real Statement::execute(const SymbolArray& val)
    {
        try
        {
            return executeImpl(val);
        }
        catch (...)
        {
            _stack.resizeFast(0);
            return 0;
        }
    }

    void Statement::set(const String& name, const Math::Real value)
    {
        if (const size_t idx = _table.find(name);
            idx == Npos)
            _table.insert(name, {value, Npos, StackValue::Value});
        else
            _table[idx] = {value, Npos, StackValue::Value};
    }

    void Statement::set(const VInt index, const Math::Real value)
    {
        if (index < _table.size())
            _table[index] = {value, Npos, StackValue::Value};
    }

    VInt Statement::indexOf(const String& name) const
    {
        return _table.find(name);
    }

    Math::Real Statement::get(const String& name, const Math::Real def)
    {
        if (const size_t idx = _table.find(name);
            idx != Npos)
            return _table[idx].v;
        return def;
    }

    Math::Real Statement::peek(I32 idx)
    {
        idx = (_stack.topI() - idx);
        if (idx >= 0 && idx < _stack.sizeI())
            return _stack[idx].v;
        return 0;
    }

    void Statement::get(const String& name, ValueList& dest)
    {
        dest.resizeFast(0);
        const U32 hashKey = (U32)get(name, -1);

        if (const size_t idx = _groups.find(hashKey);
            idx != Npos)
        {
            if (ValueGrouping* vg = _groups.at(idx);
                vg != nullptr)
            {
                for (I32 i = vg->sizeI() - 1; i >= 0; --i)
                    dest.push_back(vg->at(i).v);
            }
        }
    }

    Statement::~Statement()
    {
        for (const auto& ele : _groups)
            delete ele.second;
    }

}  // namespace Rt2::Eq
