
#include "./touch_handlers.h"

namespace atl
{
    touch_handlers::touch_handlers(std::function<void(uint32_t, const atl::point2f &)> inTouchStartHandler,
                                   std::function<void(uint32_t, const atl::point2f &)> inTouchMovedHandler,
                                   std::function<void(uint32_t, const atl::point2f &)> inTouchEndHandler,
                                   std::function<void(uint32_t)> inTouchLostHandler) :
    mTouchStartHandler(inTouchStartHandler),
    mTouchMovedHandler(inTouchMovedHandler),
    mTouchEndHandler(inTouchEndHandler),
    mTouchLostHandler(inTouchLostHandler)
    {}
}