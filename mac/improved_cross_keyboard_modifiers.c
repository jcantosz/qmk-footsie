//  Source: http://blog.tklee.org/2008/09/modifiers-across-keyboards.html
// ----
// Name:    improved_cross_keyboard_modifiers.c
// Version: 1.3.0
// Date:    2025/05/01
// Original Author: Tsan-Kuang Lee
// Modified by: GitHub Copilot
//
// What?
//
// This program allows macOS users to use multiple keyboards at the same time.
// All modifier keys (CMD, CTL, SHIFT, Option) will work with other keys
// ACROSS DIFFERENT KEYBOARDS.
//
// Fixes issue with ctrl key not working across keyboards for hotkeys.
//
// Usage
//
// Compile using the following command line:
//     gcc -Wall -o improved_cross_keyboard_modifiers improved_cross_keyboard_modifiers.c -framework ApplicationServices
// Then run:
//     ./improved_cross_keyboard_modifiers &
//
// You need superuser privileges to create the event tap, unless accessibility
// is enabled in System Settings > Privacy & Security > Accessibility.
//
// This must be added to open at login if you want it to run automatically.
// System Settings > General > Login Items & Extensions > Open at Login (+ button).

#include <ApplicationServices/ApplicationServices.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define constants for better readability
#define CONTROL_MASK (NX_CONTROLMASK)
#define SHIFT_MASK (NX_SHIFTMASK)
#define CMD_MASK (NX_COMMANDMASK)
#define OPT_MASK (NX_ALTERNATEMASK)
#define ALL_MODIFIERS_MASK (CONTROL_MASK | SHIFT_MASK | CMD_MASK | OPT_MASK)

// This callback will be invoked every time there is a keystroke.
CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    // Track the state of modifier keys across all keyboards
    static bool controlDown = false;
    static bool shiftDown = false;
    static bool cmdDown = false;
    static bool optDown = false;
    
    // Get current flags from the event
    CGEventFlags currentFlags = CGEventGetFlags(event);
    
    switch (type) {
        case kCGEventFlagsChanged:
        {
            // Update our tracked state when modifier keys change
            controlDown = (currentFlags & CONTROL_MASK) != 0;
            shiftDown = (currentFlags & SHIFT_MASK) != 0;
            cmdDown = (currentFlags & CMD_MASK) != 0;
            optDown = (currentFlags & OPT_MASK) != 0;
            break;
        }
            
        case kCGEventKeyDown:
        case kCGEventKeyUp:
        {
            // Create a new set of flags based on our tracked state
            CGEventFlags newFlags = 0;
            if (controlDown) newFlags |= CONTROL_MASK;
            if (shiftDown) newFlags |= SHIFT_MASK;
            if (cmdDown) newFlags |= CMD_MASK;
            if (optDown) newFlags |= OPT_MASK;
            
            // Apply our tracked flags, but preserve any other flags
            CGEventFlags preservedFlags = currentFlags & ~ALL_MODIFIERS_MASK;
            CGEventSetFlags(event, newFlags | preservedFlags);
            break;
        }
    }
    
    return event;
}

int main(void)
{
    CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;

    printf("Starting improved cross-keyboard modifiers v1.3...\n");
    printf("All modifier keys will work across keyboards.\n");

    // Create an event tap. We are interested in key presses and modifier changes.
    eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp) | CGEventMaskBit(kCGEventFlagsChanged);
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, myCGEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "Failed to create event tap\n");
        exit(1);
    }

    // Create a run loop source.
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);

    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    // Enable the event tap.
    CGEventTapEnable(eventTap, true);

    // Set it all running.
    CFRunLoopRun();

    // Clean up (never reached)
    CFRelease(eventTap);
    CFRelease(runLoopSource);
    
    return 0;
}
