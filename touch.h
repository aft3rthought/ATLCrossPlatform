
#include "ATLUtil/math2d.h"

#pragma once

namespace atl
{
    class touch
    {
    public:
        touch(unsigned int pTouchId, const atl::point2f & pPosition);

        unsigned int mTouchId;
        atl::point2f mPosition;
        bool mIsLive;
    };
}