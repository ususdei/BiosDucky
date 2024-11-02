
# BIOS Ducky

This is a very simple Raspberry Pi Pico application, which solves a very specific problem:

I had a somewhat glitchy headless server board, where the IPMI interface was not available
for some time after booting. So in order to access the BIOS menu, one hat to connect an
actual keyboard, manually smash the DEL key during the first minute and wait for the IPMI
to come online in order to access the BIOS menu remotely.

This is where the BIOS Ducky comes it.
It emulates a keyboard and continuously presses and releases the DEL key for 90 seconds
after power up.
So when connected, it would ensure that the board directly boots into BIOS menu
and that was accessible via IPMI afterwards.

Problem solved, end of story.


# Credits

Thanks to Earle F. Philhower, III and Max Gerhard
for the integration of the Raspberry Pi Pico in platform.io.

Also thanks to Adafruit for the TinyUSB implementation
and many more contributions to the Arduino ecosystem.

