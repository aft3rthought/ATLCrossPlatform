

#include "./stage.h"

namespace atl
{
    stage::stage(float inWidth, float inHeight)
    {
        setDimensions(inWidth, inHeight);
    }
    
    void stage::setDimensions(float inWidth, float inHeight)
    {
        mDimensions.w = inWidth;
        mDimensions.h = inHeight;
        
        float l_wRadius = inWidth / 2.f;
        float l_hRadius = inHeight / 2.f;
        
        mBounds.t = l_hRadius;
        mBounds.r = l_wRadius;
        mBounds.b = -l_hRadius;
        mBounds.l = -l_wRadius;
    }
}
