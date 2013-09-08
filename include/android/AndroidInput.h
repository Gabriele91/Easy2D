#ifndef ANDROIDINPUT_H
#define ANDROIDINPUT_H

#include <Config.h>
#include <Math3D.h>
#include <Input.h>
#include <AndroidApp.h>

namespace Easy2D {

	class AndroidInput : Input{

	public:

			/**
			* get mouse position
			* @return mouse position
			*/
			virtual Vec2 getMouse() const{
				return Vec2::ZERO;
			}
			/**
			* get finger position
			* @param finger number
			* @return finger position
			*/
			virtual Vec2 getFinger(Key::Finger id) const{
				return Vec2::ZERO;
			}
			/**
			* get accelerometer values
			* @return (x,y,z, azimuth, pitch, roll) accelerometer values
			*/
			virtual const AccelerometerValues& getAccelerometer() const{
				return accelerometerVs;
			}
			/**
			* update window event,
			* call this method in a loop
			*/
			virtual void update();
			/**
			* Return true if window has focus
			* @return focus status
			*/
			virtual bool getFocus() const{
				return ewindow.focus;
			}
			/**
			* Return true if keyboard button is down
			* @param key's id
			* @return status
			*/
			virtual bool getKeyDown(Key::Keyboard id) const{
				return false;
			}
			/**
			*  Return true if go up keyboard button
			* @param key's id
			* @return status
			*/
			virtual bool getKeyUp(Key::Keyboard id) const{
				return false;
			}
			/**
			*  Return true if hit keyboard button
			* @param key's id
			* @return status
			*/
			virtual bool getKeyHit(Key::Keyboard id) const{
				return false;
			}
			/**
			* Return true if mouse button is down
			* @param mouse button
			* @return status
			*/
			virtual bool getMouseDown(Key::Mouse id) const{
				return false;
			}
			/**
			* Return true if mouse button is up
			* @param mouse button
			* @return status
			*/
			virtual bool getMouseUp(Key::Mouse id) const{
				return false;
			}
			/**
			* Return true if mouse button is hit
			* @param mouse button
			* @return status
			*/
			virtual bool getMouseHit(Key::Mouse id) const{
				return false;
			}
			/**
			* Return scroll wheel delta
			* @return scroll delta
			*/
			virtual float getScroll() const{
				return 0;
			}
			/**
			* Return true if finger is down
			* @param finger number
			* @return status
			*/
			virtual bool getFingerDown(Key::Finger id) const{
				return efingers.status[id] ? true : false;
			}
			/**
			* Return true if finger is up
			* @param finger number
			* @return status
			*/
			virtual bool getFingerUp(Key::Finger id) const{
				return efingers.status[id] ? true : false;
			}
			/**
			* Return true if finger hit the screen
			* @param  finger number
			* @return status
			*/
			virtual bool getFingerHit(Key::Finger id) const{
				return efingers.hit[id] ? true : false;;
			}
			/**
			* Return true if window minimized
			* @return window status
			*/
			virtual bool getMinimized() const{
				return ewindow.minimized;
			}
			/**
			* Return true if window maximized
			* @return window status
			*/
			virtual bool getMaximized() const{
				return ewindow.maximized;
			}
			/**
			* Return true if window is closed
			* @return window status
			*/
			virtual bool getClose() const{
				return ewindow.close;
			}
			/**
			* Return true if window is resized
			* @return window status
			*/
			virtual bool getResize() const{
				return ewindow.resize;
			}
			/**
			* window size (when it is resized)
			* @return window size
			*/
			virtual Vec2 getResizeValues() const{
				return ewindow.windowResize;
			}


		protected:
			//window
			struct EventWindow{
				bool focus,
					 minimized,
					 maximized,
					 resize,
					 close;
				Vec2 windowResize;
				void __init(){
					memset(this,0,sizeof(EventWindow));
				}
			}ewindow;
			//fingers
			struct EventFingers{
				Vec3 fingerPos[Key::FINGERMAX];
				char status[Key::FINGERMAX];
				char hit[Key::FINGERMAX];

				void __init(){
					memset(this,0,sizeof(EventFingers));
				}
				void __fingerDown(Key::Finger k,const Vec3& pos){
					status[k]=(char)(0x0001 | 0x0002*(status[k]&0x0001));
					fingerPos[k]=pos;
				}
				void __fingerUp(Key::Finger k,const Vec3& pos){
					hit[k]=(hit[k]&0x0001)!=0;
					status[k]=false;
					fingerPos[k]=pos;
				}
				void __clearHit(){
					memset(hit,false,Key::FINGERMAX);
				}
				void __update(AndroidInput *self){
					for (int i=Key::FINGER1; i<((int)Key::FINGERMAX); ++i){
							if(status[i])
								self->__callOnFingerDown(fingerPos[i],(Key::Finger)i);
					}
				}
			}efingers;
			//accellerometer
			AccelerometerValues accelerometerVs;
			//calls
			void __callOnFingerMove(const Vec3& fingerPosition, Key::Finger fingerId);
			void __callOnFingerPress(const Vec3& fingerPosition, Key::Finger fingerId);
			void __callOnFingerDown(const Vec3& fingerPosition, Key::Finger fingerId);
			void __callOnFingerRelease(const Vec3& fingerPosition, Key::Finger fingerId);
			void __callOnAccelerometerEvent(const AccelerometerValues& acVs);
			//hide constructor
			AndroidInput();
			//friends class
			friend class AndroidApp;

	};

};

#endif
