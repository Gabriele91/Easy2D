//
//  CocoaInput.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//

#include <CocoaInput.h>
using namespace Easy2D;

Key::Keyboard KeyMapCocoa[Key::KEYBOARDMAX];

CocoaInput::CocoaInput(){
	//set base value
    memset(KeyMapCocoa,Key::KEY_NULL,sizeof(Key::Keyboard)*Key::KEYBOARDMAX);
    //
	//input window
	ewindow.__init();
	//input keyboard hit
	ekeyboard.__init();
	//input mouse hit
	emouse.__init();
    
}

void CocoaInput::__updateCocoaEvent(){
    
    
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
	for ( ; ; ) {
		NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES ];
		if ( event == nil ) {
			break;
		}
		switch ([event type]) {
			case NSKeyDown:
			case NSKeyUp:
				//			case NSFlagsChanged:
				// XXX handle key event
				//Cocoa_HandleKeyEvent(event);
                
				/* Fall through to pass event to NSApp; er, nevermind... */
				
				/* Add to support system-wide keyboard shortcuts like CMD+Space */
				if (([event modifierFlags] & NSCommandKeyMask) || [event type] == NSFlagsChanged)
					[NSApp sendEvent: event];
				break;
			default:
				[NSApp sendEvent:event];
				break;
		}
	}
    
	[pool release];

}

void CocoaInput::update(){
	//update input
	//reset input window
	ewindow.resize=false;
	//update hit
	ekeyboard.__clearHit();
	emouse.__clearHit();
	//update cocoa events
    __updateCocoaEvent();
	///////////////////////////LOOP EVENT
	//update down keys
	ekeyboard.__update(this);
	emouse.__update(this);
    
}

//keyboard
void CocoaInput::__callOnKeyPress(Key::Keyboard key) {
	for(auto ih : vkeyboardh )
		ih->onKeyPress(key);
}
void CocoaInput::__callOnKeyRelease(Key::Keyboard key) {
	for(auto ih : vkeyboardh )
		ih->onKeyRelease(key);
}
void CocoaInput::__callOnKeyDown(Key::Keyboard key) {
	for(auto ih : vkeyboardh )
		ih->onKeyDown(key);
}

//mouse
void CocoaInput::__callOnMouseMove(Vec2 mousePosition) {
	for(auto ih : vmouseh )
		ih->onMouseMove(mousePosition);
}
void CocoaInput::__callOnMousePress(Vec2 mousePosition, Key::Mouse button) {
	for(auto ih : vmouseh )
		ih->onMousePress(mousePosition,button);
}
void CocoaInput::__callOnMouseDown(Vec2 mousePosition, Key::Mouse button) {
	for(auto ih : vmouseh )
		ih->onMouseDown(mousePosition,button);
}
void CocoaInput::__callOnMouseRelease(Vec2 mousePosition, Key::Mouse button) {
	for(auto ih : vmouseh )
		ih->onMouseRelease(mousePosition,button);
}
void CocoaInput::__callOnMouseScroll(short scrollDelta) {
	for(auto ih : vmouseh )
		ih->onMouseScroll(scrollDelta);
}
