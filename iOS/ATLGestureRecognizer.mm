//
//  ATLGestureRecognizer.m
//  ATL Cross Platform Library
//
//  Created by Maxwell Robinson on 9/16/14.
//  Copyright (c) 2014 All The Loot. All rights reserved.
//

#import "./ATLGestureRecognizer.h"

#import <UIKit/UIGestureRecognizerSubclass.h>

#include "ATLCrossPlatform/touch.h"
#include "ATLCrossPlatform/stage.h"
#include "ATLCrossPlatform/touch_handlers.h"
#include <vector>

@interface ATLGestureRecognizer ()

@property atl::touch_handlers *handlers;
@property atl::stage* stage;
@property std::vector<atl::touch> touches;
@property unsigned int nextTouchId;

@end

@implementation ATLGestureRecognizer

- (ATLGestureRecognizer*)initWithStage:(atl::stage*)pStage andTouchHandlers:(atl::touch_handlers*)pHandlers
{
    self = [super init];
    if(self != nil)
    {
        _stage = pStage;
        _handlers = pHandlers;
    }
    return self;
}

- (void)reset
{
    [super reset];
}

- (void) onTouchesLost
{
    [self closeTouches];
}

- (void) closeTouches
{
    // Have all touches end where they were last seen
    for(const auto & lGameTouch : _touches)
        _handlers->mTouchLostHandler(lGameTouch.mTouchId);
    
    // Clear touches array:
    _touches.clear();
    _nextTouchId = 0;
    
    // Mark gesture as "recognized"
    if(self.state == UIGestureRecognizerStatePossible)
        self.state = UIGestureRecognizerStateRecognized;
}

- (atl::point2f) transformPoint:(CGPoint)point
{
    float l_pctX = point.x / [self.view bounds].size.width;
    float l_pctY = point.y / [self.view bounds].size.height;

    return atl::point2f(_stage->bounds().l + _stage->dimensions().w * l_pctX,
                        _stage->bounds().t - _stage->dimensions().h * l_pctY);

    return atl::point2f(0.f, 0.f);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    
    if(_touches.size() == 0)
        self.state = UIGestureRecognizerStatePossible;
    
    for(UITouch* lTouch in touches)
    {
        atl::point2f lGameScreenPosition = [self transformPoint:[lTouch locationInView:self.view]];
        
        size_t lGameTouchIdx = std::numeric_limits<size_t>::max();
        for(int idx = 0; idx < _touches.size(); idx++)
        {
            auto& lGameTouch = _touches[idx];
            if(!lGameTouch.mIsLive)
            {
                lGameTouchIdx = idx;
                lGameTouch.mTouchId = _nextTouchId++;
                lGameTouch.mIsLive = true;
                lGameTouch.mPosition = lGameScreenPosition;
                break;
            }
        }
        if(lGameTouchIdx == std::numeric_limits<size_t>::max())
        {
            lGameTouchIdx = _touches.size();
            _touches.emplace_back(_nextTouchId++, lGameScreenPosition);
        }

        _handlers->mTouchStartHandler(_touches[lGameTouchIdx].mTouchId, _touches[lGameTouchIdx].mPosition);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    
    if(self.state == UIGestureRecognizerStateFailed)
        return;
    
    for(UITouch* lTouch in touches)
    {
        atl::point2f l_gamePrevScreenPosition = [self transformPoint:[lTouch previousLocationInView:self.view]];
        atl::point2f l_gameScreenPosition = [self transformPoint:[lTouch locationInView:self.view]];
        
        int lGameTouchIdx = -1;
        for(int idx = 0; idx < _touches.size(); idx++)
        {
            auto& lGameTouch = _touches[idx];
            if(lGameTouch.mPosition == l_gamePrevScreenPosition)
            {
                lGameTouchIdx = idx;
                lGameTouch.mPosition = l_gameScreenPosition;
                break;
            }
        }

        if(lGameTouchIdx != -1)
        {
            _handlers->mTouchMovedHandler(_touches[lGameTouchIdx].mTouchId, _touches[lGameTouchIdx].mPosition);
        }
        else
        {
            [self onTouchesLost];
        }
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesEnded:touches withEvent:event];
    if (self.state == UIGestureRecognizerStatePossible)
    {        
        for(UITouch* lTouch in touches)
        {
            atl::point2f l_gamePrevScreenPosition = [self transformPoint:[lTouch previousLocationInView:self.view]];
            atl::point2f l_gameScreenPosition = [self transformPoint:[lTouch locationInView:self.view]];

            int lGameTouchIdx = -1;
            for(int idx = 0; idx < _touches.size(); idx++)
            {
                auto& lGameTouch = _touches[idx];
                if(lGameTouch.mPosition == l_gamePrevScreenPosition ||
                   lGameTouch.mPosition == l_gameScreenPosition)
                {
                    lGameTouchIdx = idx;
                    lGameTouch.mPosition = l_gameScreenPosition;
                    lGameTouch.mIsLive = false;
                    break;
                }
            }
            
            if(lGameTouchIdx != -1)
            {
                _handlers->mTouchEndHandler(_touches[lGameTouchIdx].mTouchId, _touches[lGameTouchIdx].mPosition);
            }
            else
            {
                [self onTouchesLost];
            }
        }
        
        // If all touches are no longer live, then switch to recognized state:
        bool lNoTouchesLeft = true;
        for(const auto& lGameTouch : _touches)
        {
            if(lGameTouch.mIsLive)
            {
                lNoTouchesLeft = false;
                break;
            }
        }
        
        if(lNoTouchesLeft)
            self.state = UIGestureRecognizerStateRecognized;
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Assumes all touches are going to be cancelled when this message is received.
    
    [super touchesCancelled:touches withEvent:event];
    
    for(auto & lGameTouch : _touches)
    {
        _handlers->mTouchLostHandler(lGameTouch.mTouchId);
        lGameTouch.mIsLive = false;
    }
    
    if (self.state == UIGestureRecognizerStatePossible)
        self.state = UIGestureRecognizerStateFailed;
}

@end
