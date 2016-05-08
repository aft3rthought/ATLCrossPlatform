//
//  ATLGameGestureRecognizer.h
//  ATL Cross Platform Library
//
//  Created by Maxwell Robinson on 9/16/14.
//  Copyright (c) 2014 All The Loot. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "ATLCrossPlatform/stage_fwd.h"
#include "ATLCrossPlatform/touch_handlers_fwd.h"

@interface ATLGestureRecognizer : UIGestureRecognizer

- (ATLGestureRecognizer*)initWithStage:(atl::stage*)pStage andTouchHandlers:(atl::touch_handlers*)pHandlers;

- (void)closeTouches;

- (void)reset;
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;

@end
