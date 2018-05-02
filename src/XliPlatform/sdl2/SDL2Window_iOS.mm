#include "SDL2Window.h"
#include <UIKit/UIKit.h>
#include <SDL.h>
#include <SDL_syswm.h>

// extern Xli::WindowEventHandler* GlobalEventHandler;
extern Xli::Window* GlobalWindow;

namespace Xli
{
    namespace PlatformSpecific
    {
        void SDL2Window::SetOnscreenKeyboardPosition(Vector2i position)
        {
        }

        Vector2i SDL2Window::GetOnscreenKeyboardPosition()
        {
            int scale = [[UIScreen mainScreen] scale];
            return Vector2i(0, (int)((this->h - GetOnscreenKeyboardSize().Y) * scale));
        }

        Vector2i SDL2Window::GetOnscreenKeyboardSize()
        {
            int scale = [[UIScreen mainScreen] scale];

            if (IsOnscreenKeyboardVisible())
            {
                return this->keyboardSize * scale;
            } else {
                return Vector2i((int)(this->keyboardSize.X * scale), 0);
            }
        }

        bool SDL2Window::IsStatusBarVisible()
        {
            return !this->fullscreen;
        }

        Vector2i SDL2Window::GetStatusBarPosition()
        {
            return Vector2i(0, 0);
        }

        bool statusSizeSupportsOrientation;
        Vector2i SDL2Window::GetStatusBarSize()
        {
            int scale = [[UIScreen mainScreen] scale];
            if (this->fullscreen)
            {
                if (statusSizeSupportsOrientation) {
                    return Vector2i((int)([UIApplication sharedApplication].statusBarFrame.size.width * scale), 0);
                } else {
                    UIInterfaceOrientation orien = [UIApplication sharedApplication].statusBarOrientation;
                    if (orien == UIInterfaceOrientationPortrait || orien == UIInterfaceOrientationPortraitUpsideDown)
                    {
                        return Vector2i((int)([UIApplication sharedApplication].statusBarFrame.size.width * scale), 0);
                    } else {
                        return Vector2i((int)([UIApplication sharedApplication].statusBarFrame.size.height * scale), 0);
                    }
                }
            } else {
                if (statusSizeSupportsOrientation) {
                    return Vector2i((int)([UIApplication sharedApplication].statusBarFrame.size.width * scale),
                                    (int)([UIApplication sharedApplication].statusBarFrame.size.height * scale));
                } else {
                    UIInterfaceOrientation orien = [UIApplication sharedApplication].statusBarOrientation;
                    if (orien == UIInterfaceOrientationPortrait || orien == UIInterfaceOrientationPortraitUpsideDown)
                    {
                        return Vector2i((int)([UIApplication sharedApplication].statusBarFrame.size.width * scale),
                                        (int)([UIApplication sharedApplication].statusBarFrame.size.height * scale));
                    } else {
                        return Vector2i((int)([UIApplication sharedApplication].statusBarFrame.size.height * scale),
                                        (int)([UIApplication sharedApplication].statusBarFrame.size.width * scale));
                    }
                }
            }
        }

        void SDL2Window::SDL2WindowInit()
        {
            int width, height;
            SDL_GetWindowSize(((SDL2Window*)GetMainWindow())->window, &width, &height);
            this->keyboardSize = Vector2i(width,0);

            statusSizeSupportsOrientation = ([[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] != NSOrderedAscending);

            NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
            NSOperationQueue *queue = [NSOperationQueue mainQueue];
            [center addObserverForName:UIKeyboardDidChangeFrameNotification
             object:nil
             queue:queue
             usingBlock:^(NSNotification *notification) {
                    UIInterfaceOrientation orien = [UIApplication sharedApplication].statusBarOrientation;
                    CGSize kbdSize = [[[notification userInfo] objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size;
                    int scale = 1;
                    // This is to hack around the fact that when the keyboard is split you only sometimes
                    // get size events. You get them when the split keyboard is visible and you change orientation,
                    // but you get 0 if you dock and then split the keyboard again. This is really annoying but there
                    // is no good way to get the split status of the keyboard. This may be fixed after changing the
                    // above UIKeyboardFrameBeginUserInfoKey to UIKeyboardFrameEndUserInfoKey
                    if ((float)kbdSize.width>0 && (float)kbdSize.height>0)
                    {
                        // if (statusSizeSupportsOrientation) {
                        //     this->keyboardSize = Vector2i((int)((float)kbdSize.width * scale), (int)((float)kbdSize.height * scale));
                        // } else {
                        //     UIInterfaceOrientation orien = [UIApplication sharedApplication].statusBarOrientation;
                        //     if (orien == UIInterfaceOrientationPortrait || orien == UIInterfaceOrientationPortraitUpsideDown)
                        //     {
                        //         this->keyboardSize = Vector2i((int)((float)kbdSize.width * scale), (int)((float)kbdSize.height * scale));
                        //         this->keyboardSize = Vector2i((int)([UIApplication sharedApplication].statusBarFrame.size.width * scale), 0);
                        //     } else {
                        //         this->keyboardSize = Vector2i((int)((float)kbdSize.height * scale), (int)((float)kbdSize.width * scale));
                        //     }
                        // }
                        this->keyboardSize = Vector2i((int)((float)kbdSize.width * scale), (int)((float)kbdSize.height * scale));

                        if (GlobalWindow) {
                            GlobalWindow->GetEventHandler()->OnKeyboardResized(GlobalWindow);
                        }
                    }
                }
                ];
        }
    }
};
