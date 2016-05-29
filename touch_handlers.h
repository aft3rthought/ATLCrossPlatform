
#pragma once

#include "ATLUtil/math2d_fwd.h"
#include <functional>

namespace atl
{
    class touch_handlers
    {
    public:
        touch_handlers(std::function<void(uint32_t, const atl::point2f &)> inTouchStartHandler,
                       std::function<void(uint32_t, const atl::point2f &)> inTouchMovedHandler,
                       std::function<void(uint32_t, const atl::point2f &)> inTouchEndHandler,
                       std::function<void(uint32_t)> inTouchLostHandler);
        
        std::function<void(uint32_t, const atl::point2f &)> mTouchStartHandler;
        std::function<void(uint32_t, const atl::point2f &)> mTouchMovedHandler;
        std::function<void(uint32_t, const atl::point2f &)> mTouchEndHandler;
        std::function<void(uint32_t)> mTouchLostHandler;
    };
}
