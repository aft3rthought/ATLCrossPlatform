
#include "./touch.h"

namespace atl
{
    touch::touch(unsigned int pTouchId, const atl::point2f & pPosition) :
    mTouchId(pTouchId),
    mPosition(pPosition),
    mIsLive(true)
    {}
}