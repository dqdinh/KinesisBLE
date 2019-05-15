#ifndef BATTERY_H
#define BATTERY_H

#define MV_PER_LSB   (0.73242188F)   // 3.0V ADC range and 12-bit ADC resolution = 3000mV/4096

#define VBAT_DIVIDER      (0.71275837F)   // 2M + 0.806M voltage divider on VBAT = (2M / (0.806M + 2M))
#define VBAT_DIVIDER_COMP (1.403F)        // Compensation factor for the VBAT divider, This is just 1 / VBAT_DIVIDER (above)

uint8_t batteryPercentage(); 
int     batteryMv();
bool    usbConnected();
int     usbVoltage();

extern bool batteryLedOn;
extern unsigned long  batteryLedTimer;
extern unsigned long  chargingAnimationLastToggle;
extern bool chargingAnimationOn;

void showBatteryLevel();
void setLED(int pin, bool state);
void setAllBatteryLed(bool state);
void batteryChargingAnimation();

#endif //BATTERY_H
