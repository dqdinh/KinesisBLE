#include <Arduino.h>
#include "battery.h"
#include "config.h"

unsigned long  timeLastBreak = 0;             // Timestamp of end of last break
bool onBreak = false;               // Already taking a break
bool restLedOn = false;             // Toggle LED state
int  timeLastRestLedToggle = 0;     // Timer to flash LED
bool restTimerUsingLeds = false;    // Let other functions know if they can use the LED(s)

/**
 * Let user know that they need to take a break.
 */
void indicateNeedBreak(int now) {

      restTimerUsingLeds = true;

      // Only set LED(s) every so often to give flashing effect.
      if (now - timeLastRestLedToggle < 300) {
          return;
      }
      
      if(restLedOn) {
          setAllBatteryLed(LOW);
          restLedOn = false;
      } else {
          setAllBatteryLed(HIGH);
          restLedOn = true;
      }
      
      timeLastRestLedToggle = now;
}

/**
 * Check if user has taken a break.
 */
bool hasTakenBreak(unsigned long now, unsigned long lastKeypressTimestamp) {
    return now - lastKeypressTimestamp > REST_INTERVAL_SECS*1000;
}

/**
 * Check if user is already on break / afk.
 */
bool awayFromKeyboard(unsigned long now, unsigned long lastKeypressTimestamp) {
  return now - lastKeypressTimestamp >  WORK_TIMEOUT_MINS*60*1000;
}

/**
 *  Has the user pressed a key since the last time they
 *  went on break?
 */
bool backFromBreak(unsigned long timeLastBreak, unsigned long lastKeypressTimestamp) {
  return lastKeypressTimestamp > timeLastBreak;
}

/**
 * If we have been working for longer than our set work interval limit,
 * we should take a break.
 */
bool needsBreak(unsigned long now, unsigned long timeLastBreak) {
  return now - timeLastBreak > WORK_INTERVAL_MINS*60*1000;
}

/**
 * Going on break.
 */
void start_break() {
    onBreak = true;   
    timeLastBreak = millis();
    setAllBatteryLed(LOW);
    restTimerUsingLeds = false;
}

/**
 * Rest Timer
 */
void process_rest_timer(unsigned long lastKeypressTimestamp) {
  
    unsigned long now = millis(); 

    if( onBreak && backFromBreak(timeLastBreak, lastKeypressTimestamp) ) {
      onBreak = false;
    }

    if( onBreak || awayFromKeyboard(now, lastKeypressTimestamp) ) {
      timeLastBreak = now;
    }

    if(needsBreak(now,timeLastBreak)) {

        indicateNeedBreak(now);

        #ifdef REST_AUTO_BREAKS
            if(hasTakenBreak(now, lastKeypressTimestamp)){
              start_break();
            }
        #endif    
        
    }
    
}






