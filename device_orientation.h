
#pragma once

#include "ATLUtil/math2d_fwd.h"
#include <functional>

namespace atl
{
    enum class PhoneOrientation {
        Portrait,
        PortraitUpsideDown,
        LandscapeLeft,
        LandscapeRight,
        Other
    };
    
    enum class AllowedOrientations {
        PortraitUp,
        PortraitUpAndDown,
        LandscapeLeft,
        LandscapeLeftAndRight,
        All
    };
}
