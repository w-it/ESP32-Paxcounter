# www.pdrukteradar.be
**Wifi driven sensor**


# Use case

It's a proof-of-concept device for metering passenger flows in realtime. It counts how many mobile devices are around. This gives an estimation how many people are around. Paxcounter detects Wifi and Bluetooth signals in the air, focusing on mobile devices by filtering vendor OUIs in the MAC adress.

Intention of this project is to do this without intrusion in privacy: You don't need to track people owned devices, if you just want to count them. Therefore, it does not persistenly store MAC adresses and does no kind of fingerprinting the scanned devices.

Data is transferred to a server via a LoRaWAN network, and/or a wired SPI slave interface. 


# Credits

Thanks to 
- [Oliver Brandmüller](https://github.com/spmrider) for idea and initial setup of this project
- [Charles Hallard](https://github.com/hallard) for major code contributions to this project
- [robbi5](https://github.com/robbi5) for the payload converter
- [terrillmoore](https://github.com/mcci-catena) for maintaining the LMIC for arduino LoRaWAN stack
- [sbamueller](https://github.com/sbamueller) for writing the tutorial in Make Magazine
- [Stefan](https://github.com/nerdyscout) for paxcounter opensensebox integration
- [August Quint](https://github.com/AugustQu) for adding SD card data logger and SDS011 support
