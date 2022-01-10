# LED Dimmer MOSFET SSR

THIS PROJECT IS BASED ON 230V AND SHOULD BE USED WITH CAUTION!

This is my first circuit that uses mains voltage. It is a two-channel LED dimmer with MOSFET SSR. Everything is controlled by ESP32-WROOM-IE. There are two fuses - one for 3V3 Hi-Link power supply and another for two output channels. The two pairs of N-channel IRFP460 MOSFETs are driven by two Si8752 ICs, which allow them to be completely isolated from low-voltage part. There are also 4 CT354 optocouplers (zero-cross detection, switch, PIR, light sensor). Why aren't the inputs for switch, PIR and light sensor low-voltage instead of 230V? Well I wanted to use off-the-shelf sensors that have 230V outputs.

So far the only mistake I noticed is the placement of heatsinks, which doesn't allow for an easy repair, since you can't unhinge the MOSFETs out of their place. Heatsinks are soldered to mains ground and are isolated from MOSFETs.

In action: https://imgur.com/a/OvZu2P0
