#ifndef USER_LED_H
#define USER_LED_H

typedef enum {
    USER_LED_STATE_OFF = 0,
    USER_LED_STATE_ON = 1,
} userLed_state_e;

void userLed_setup(void);
void userLed_set(userLed_state_e state);

#endif // USER_LED_H