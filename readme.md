# Tally Light System for one vMix video switcher
## Features
* 6 visual signals
    * Active - red
    * Preview - green
    * Ready - blue
    * Call - blinking yellow
    * Connecting to video switcher - purple/blue
    * Disconnected - purple
* Multiple power supply options
* Multiple mounting options
* Three configurable frequency channels in the 433 MHz band

## User guide
### Setup
1. Connect the power supply and Ethernet cable to the transmitter.
2. IP address assignment:
    * The connected router should assign TLS an IP address
    * If there is no DHCP server (a router), the transmitter sets itself to a static IP address **192.168.0.99** after **10 seconds** from the Ethernet cable connection
3. Open a browser on a computer connected to the same network as the TLS.
4. Type the IP address of the TLS (dynamic from DHCP or the static one **192.168.0.99**)
5. Set the vMix video switcher IP address on the website (the IP address of the computer where vMix is running)
6. **TLS is ready for use!**

### Transmitter power supply options
* Power adapter, 5.5/2.1mm DC connector (7-12V)
* USB-B cable (5V)

### Receiver power supply options
* Battery pack Sony L-series NP-F (7.4V)
* Power adapter, 5.5/2.1mm DC connector (7-12V)
* USB-B Mini cable (5V)

> **_NOTE:_** The maximum input voltage is 12V, but it's recommended to choose a lower voltage adapter (7.5V or 9V).

### Receiver mounting options
* Camera screw 1/4 inch
* Hook-and-loop fastener ([AliExpress](https://www.aliexpress.com/item/4000402019602.html?spm=a2g0o.cart.0.0.125638damhasXE&mp=1) link)

## Developer info
### Commands
* ```arduino-cli compile -b arduino:avr:mega --libraries EthernetMT```
* ```arduino-cli upload -b arduino:avr:mega -p COM9 -v```

### Versions
#### Transmitter - TLS_G3_ETH_basic 
```v1.1.0``` ETH_basic_RLS04_06.09.2022 for Revart   
```v1.2.0``` TLS for Andrej Sliacky (ETH_basic-T3)
#### Receiver
```v1.2.0``` 14 LEDs

### MAC address assignment
|  Device name |    MAC address    | Static IP address |
|:------------:|:-----------------:|:-----------------:|
| TLS_basic-T1 | 02:54:4C:53:00:01 |         -         |
| TLS_basic-T2 | 02:54:4C:53:00:03 |   192.168.0.103   |
|   TLS_dual   | 02:54:4C:53:00:00 |         -         |
|    TLS_RSG   | 02:54:4C:53:01:00 |         -         |
|  TLS_hybrid  | 02:54:4C:53:00:04 |   192.168.0.102   |
|    TLS_G4    | 02:54:4C:53:00:02 |         -         |
|    TLS_SDI   | 02:54:4C:53:00:05 |         -         |
| TLS_basic-T3 | 02:54:4C:53:02:00 |   192.168.0.99    |