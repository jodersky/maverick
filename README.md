# Maverick
Maverick is a simple gamepad controller that outputs messages according to the mavlink protocol.
It can be used as a remote control for UAVs.

Maverick only prints control messages to stdout, transport is left up to other mechanisms.
E.g. redirecting output to a serial port:
```./maverick > /sev/ttyUSB0```

## Dependencies
 - SDL