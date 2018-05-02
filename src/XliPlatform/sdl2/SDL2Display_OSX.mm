#include <uBase/Console.h>
#include <XliPlatform/Display.h>
#include <SDL2/SDL.h>

#import <Foundation/Foundation.h>
#import <AppKit/NSScreen.h>

namespace Xli
{
    float Display::GetDensity(int displayIndex)
    {
        float dpi = (float)Display::GetDpi(displayIndex).X;
        // the scale factor is to bring it in line with android.
        return dpi / 160.0f;
    }

    Vector2 Display::GetDpi(int displayIndex)
    {
        NSArray* screens = [NSScreen screens];
        NSScreen* screen = [screens objectAtIndex: displayIndex];
        NSDictionary *description = [screen deviceDescription];
        NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
        CGSize displayPhysicalSize = CGDisplayScreenSize([[description objectForKey:@"NSScreenNumber"] unsignedIntValue]);
        float dpi = ((displayPixelSize.width / displayPhysicalSize.width) * 25.4f); // there being 25.4 mm in an inch
        return Vector2(dpi, dpi);
    }
}
