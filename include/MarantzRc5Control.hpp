#include "Arduino.h"


class MarantzRc5Control 
{
public:
    void power_on();
    void power_off();
    void power_toggle();
    void volume_up();
    void volume_down();
    void volume_mute();
    void input_next();
    void input_previous();
    void input_phono();
    void input_cd();
    void input_network();
    void input_tuner();
    void input_recorder();
    void input_coax();
    void input_optical();
    void toggle_speaker_a();
    void toggle_speaker_b();

    /*
    *  normal Philips RC-5 as in the https://en.wikipedia.org/wiki/RC-5
    *  code taken from IRremote with some changes
    */
    int sendRC5(uint8_t aAddress, uint8_t aCommand, uint_fast8_t aNumberOfRepeats);

    /*
    *  Marantz 20 bit RC5 extension, see
    *  http://lirc.10951.n7.nabble.com/Marantz-RC5-22-bits-Extend-Data-Word-possible-with-lircd-conf-semantic-td9784.html
    *  could be combined with sendRC5, but ATM split to simplify debugging
    */
    int sendRC5_X(uint8_t aAddress, uint8_t aCommand, uint8_t aExt, uint_fast8_t aNumberOfRepeats);

private:
    void send_1();
    void send_0();
};