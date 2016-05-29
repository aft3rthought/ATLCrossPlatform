

#pragma once

#include "ATLUtil/math2d.h"

namespace atl
{
    struct stage
    {
    private:
        atl::size2f mDimensions;
        atl::box2f mBounds;
        
    public:
        stage(const atl::size2f & inSize) : stage(inSize.w, inSize.h) {};
        stage(float inWidth, float inHeight);
        
        void setDimensions(float inWidth, float inHeight);
        
        const atl::size2f & dimensions() const { return mDimensions; }
        const atl::box2f & bounds() const { return mBounds; }
    };
}
