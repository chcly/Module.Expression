#pragma once
#include "Utils/Definitions.h"
#include "Utils/Exception.h"

namespace Rt2::Eq
{
    class CallState
    {
    private:
        const I32 _max{-1};
        I16       _depth{0};
        I16       _comma{0};

        CallState()                 = default;
        CallState(const CallState&) = default;

    public:
        explicit CallState(const I16& max) :
            _max(max) {}

        void depthGuard()
        {
            if (++_depth > _max || _depth < 0)
                throw Exception(
                    "maximum recursion depth exceeded");
        }

        void resetGuard() { _depth = 0; }

        void setCommaCount(const I16 c) { _comma = c; }

        const I16& commaCount() const { return _comma; }
    };

}  // namespace Rt2::Eq
