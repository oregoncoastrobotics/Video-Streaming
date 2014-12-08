# Streaming video hardware discussion:
  
The way I see it there are 3 basic hardware setups we might use for video streaming.  They vary in complexity, price and quality.

## Cost

 Option #1: $55
- USB Webcam with UVC support [an example](http://www.ebay.com/itm/380403845897?_trksid=p2055119.m1438.l2649&ssPageName=STRK%3AMEBIDX%3AIT)  ~$9
- Processor with at least 2 usb master busses [an example](http://www.amazon.com/Raspberry-Pi-Model-512MB-Computer/dp/B00LPESRUK/ref=sr_1_2?ie=UTF8&qid=1417996290&sr=8-2&keywords=raspberry+pi) ~$37
- WIFI Adapter [an example](http://www.amazon.com/Edimax-EW-7811Un-150Mbps-Raspberry-Supports/dp/B003MTTJOY/ref=sr_1_1?ie=UTF8&qid=1417996375&sr=8-1&keywords=micro+wifi+adapter) ~$9
    
Option #2:  $67
- USB Webcam with UVC support [an example](http://www.ebay.com/itm/380403845897?_trksid=p2055119.m1438.l2649&ssPageName=STRK%3AMEBIDX%3AIT)  ~$9
- Processor with only 1 usb master bus [an example](http://www.ebay.com/itm/Olimex-iMX233-OLINUXINO-Micro-linux-computer-ARM926J-alternative-to-Raspberry-Pi-/271109788909?pt=UK_BOI_Electrical_Components_Supplies_ET&hash=item3f1f6714ed) ~$39
- High Speed Serial to WIFI Adapter [an example](http://www.ebay.com/itm/WIFI232-B-RS232-To-WIFI-UART-Serial-To-Wireless-Control-Module-Adapter-Convertor-/251293811560?pt=LH_DefaultDomain_0&hash=item3a82474368) ~$19
- Note: It might be possible to reconfigure this option with an I2C camera (640x480, but only $6) and a USB WIFI adapter $9.  However, this would require low level I2C programing and ~10Mbps bandwidth vs. the 1-2Mbps above.
    
Option #3: $62-$89
- IP Camera [an example](http://www.ebay.com/itm/161287999991?_trksid=p2055119.m1438.l2649&ssPageName=STRK%3AMEBIDX%3AIT) ~$36
      Note: Less expensive options are available pcb with lens $27
- Wireless Router [an example](http://www.ebay.com/itm/Smallest-150mbps-Portable-Mini-WIFI-Repeater-USB-Wireless-Wifi-Router-Black/271636327987?rt=nc) ~$16
- Any Ethernet capable processor [an example](http://www.amazon.com/Raspberry-Pi-Model-512MB-Computer/dp/B00LPESRUK/ref=sr_1_2?ie=UTF8&qid=1417996290&sr=8-2&keywords=raspberry+pi) ~$37
       Note:  An arduino with an ethernet shield would be much less expensive <$10, but also less capable
       
## Advantages

Option 1: Least cost, largest user base, fairly easy to implement
Option 2: Hardware much more open making it easier to design your own boards, possibily most difficult to implement
Option 3: Able to use low power processor, high video bandwidth which could make much better video quality.  
