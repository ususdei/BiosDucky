
/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

// DOC: adapted from
// https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_boot_keyboard/hid_boot_keyboard.ino

#include "Arduino.h"

#include "Adafruit_TinyUSB.h"


Adafruit_USBD_HID usb_hid;

// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
const uint8_t desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD()
};

// For keycode definition check out https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h

const uint32_t SMASH_DURATION = 90000;

static uint32_t start_millis = 0;
static uint32_t next_action = 0;
static int state = 2;

static void start_smashing()
{
    start_millis = millis();
    next_action = start_millis;
    state = 0;
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    TinyUSBDevice.begin(0);

    usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
    usb_hid.setPollInterval(2);
    usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
    usb_hid.setStringDescriptor("TinyUSB Keyboard");

    usb_hid.begin();

    start_smashing();
}


void loop() {

#ifdef TINYUSB_NEED_POLLING_TASK
    // Manual call tud_task since it isn't called by Core's background
    TinyUSBDevice.task();
#endif

    // not enumerated()/mounted() yet: nothing to do
    if (!TinyUSBDevice.mounted())
        return;

    const uint32_t now = millis();
    if( now > start_millis + SMASH_DURATION ) {
        state = 2;
        return;
    }

    if( now < next_action )
        return;

    if( !TinyUSBDevice.ready() )
        return;


    switch( state ) {
        case 0: {
            usb_hid.keyboardRelease(0);
            digitalWrite(LED_BUILTIN, LOW);

            state = 1;
            next_action = now + 200;
            break;
        }
        case 1: {
            uint8_t keycode[6] = {0,0,0,0,0,0};
            keycode[0] = HID_KEY_DELETE;
            const uint8_t report_id = 0;
            const uint8_t modifier = 0;
            usb_hid.keyboardReport(report_id, modifier, keycode);
            digitalWrite(LED_BUILTIN, HIGH);

            state = 0;
            next_action = now + 200;
            break;
        }
        default: {
            digitalWrite(LED_BUILTIN, LOW);
            next_action = now + 200;
            break;
        }
    }
}

