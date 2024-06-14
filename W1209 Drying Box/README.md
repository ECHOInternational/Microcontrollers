# Introduction
This is an inexpensive approach to temperature-controlled drying, with air circulation, in an enclosed space. This drying box was initially developed to dry moringa leaves for making moringa leaf powder. It works well for that purpose, enabling the drying of leaves at a desired temperature (e.g., between 35 and 50 degrees C) for the prevention of mold and minimization of heat-sensitive vitamin loss. The box is a good option for drying small quantities of leaves or seeds.The desired drying temperature can be set with push buttons on the temperature controller, so there is no need to upload programming code via a computer.

# How it works
The drying box is pictured below with its various components. Heat is generated with mini heat pads. Air circulation is provided with axial-style DC computer fans. When connected to power (via a DC adaptor) the fans and heat pads turn on. When the temperature reaches the desired/preset temperature, the heat pads shut off while the fans continue to run. When the temperature drops below the desired temperature, the heat pads turn on again. Thus, during drying the fans stay on constantly while the heat pads are switched on and off automatically to keep the temperature to within a few degrees of the desired temperature. 

![DryingBoxW1209](https://github.com/ECHOInternational/Microcontrollers/assets/69003593/898c4e29-8af6-4c41-9e38-8ff6ab2949eb)

Air enters through two small air intake ports behind a fan on each side of the box. Just below each fan is a mini heat pad. Thus, incoming air is quickly heated and circulated via the fans within the box. Heated air and water vapor from moisture in the leaves or seeds is vented out the top. It will work as long as the incoming air is cooler than the drying temperature. 

# Materials
1. An insulated box, such as the Styrofoam cooler pictured above. Inner dimensions are 36.8 cm long X 29.2 cm wide X 23.5 cm deep (roughly--with the lid on), which equals 25,252.16 cubic cm or 25.25 liters.
2. W1209 temperature controller or something similar. Both 5 and 12 volt options are available. The dryer shown above uses 12 volts. A [W1209 controller](https://www.aliexpress.us/item/3256805851910105.html?src=google&src=google&albch=shopping&acnt=708-803-3821&slnk=&plac=&mtctp=&albbt=Google_7_shopping&gclsrc=aw.ds&albagn=888888&isSmbAutoCall=false&needSmbHouyi=false&src=google&albch=shopping&acnt=708-803-3821&slnk=&plac=&mtctp=&albbt=Google_7_shopping&gclsrc=aw.ds&albagn=888888&ds_e_adid=&ds_e_matchtype=&ds_e_device=c&ds_e_network=x&ds_e_product_group_id=&ds_e_product_id=en3256805851910105&ds_e_product_merchant_id=109144340&ds_e_product_country=US&ds_e_product_language=en&ds_e_product_channel=online&ds_e_product_store_id=&ds_url_v=2&albcp=19623912707&albag=&isSmbAutoCall=false&needSmbHouyi=false&gad_source=1&gclid=CjwKCAjwmYCzBhA6EiwAxFwfgEj1_OyVOn2Clk6kr6kFfY08jwdATWD7-ReEFJdhNmse2NiaI06uTRoCcZAQAvD_BwE&aff_fcid=e80e6b8574cd4b178f86be2d633626d3-1717620333549-01390-UneMJZVf&aff_fsk=UneMJZVf&aff_platform=aaf&sk=UneMJZVf&aff_trace_key=e80e6b8574cd4b178f86be2d633626d3-1717620333549-01390-UneMJZVf&terminal_id=14bb5c891ddb4baebb0909fb32ee2f45&afSmartRedirect=y&gatewayAdapt=glo2usa) can be purchased for a dollar or two plus a few dollars for shipping.
3. Two 12-volt mini heat pads. Each heat pad for this dryer measured 100 X 50 mm in size. They were purchased from [AliExpress](https://www.aliexpress.us/item/3256803865462882.html?spm=a2g0o.order_list.order_list_main.45.113b1802ERYpJ6&gatewayAdapt=glo2usa) for less than $2 each plus about $3 shipping.
4. Wires (e.g. 22 awg thickness) for connecting components and connectors (twist connecters or screw terminals depending on your design preference)
5. Two 12-volt DC computer fans
6. A 12-volt DC power adapter (the one used for this dryer is rated for a current draw of 5 amps, to make sure it can power the heat pads and fans adequately, but you may be able to generate sufficient heat with less current). The adaptor used for this dryer came with a barrel jack screw terminal for easy connection of the W1209 power and ground wires to the DC power adapter.
7. Two short pieces of narrow-diameter PVC pipe for the vent. This dryer uses PVC with an inner diameter of about 12 mm.
8. Four short pieces of drip irrigation feeder tubing (or something similar) to serve as air intake ports, two behind each fan. The tubing of this dryer has an inner diameter of about 4 mm. 
9. Bolts and nuts for mounting the fans.
   
# Wiring
## Heat pads
Each 12-volt mini heat pad will have two wires coming out of it. These pads do not have polarity. Thus, for each heat pad, select one wire to be ground (-) and one to be power (+) and then be consistent in making all your wire connections. Since there are two heat pads, you will have two ground and two power wires. Connect the two ground wires together, along with a third wire. The third wire will connect with the ground wires of the fans and ultimately to the ground terminal of the W1209 temperature controller. 

Connect the two positive wires of the heat pads along with a third wire that will connect to the W1209 temperature controller (as explained further down).

## Fans
Each 12-volt fan will have a ground (-) and power (+) wire. Connect the two ground wires and a third wire together. The third wire will connect to the ground wires of the heat pad and ultimately to the GND terminal of the temperature controller.

## W1209 temperature controller
There are four screw terminals on the temperature controller: K0 K1 +12V, GND  K0 is for controlling the load that you want to power on and off based on temperature, the mini heat pads in this case. K1 should connect with K0. The +12 and GND terminals are for connecting the temperature controller to the power supply (12-volt DC adaptor) and fans (since they need to stay on and run regardless of temperature). Make connections, then, as follows:

|W1209 terminal|Connecting component | 
|--------------|--------------------------|
|K0            |Heat pad power (+)        |
|K1            |K0 (directly or by connecting both K0 and K1 to fan power (+)|
|+12V          |Power supply (+)  |
|+12v          |Fan power (+)     |
|GND           |Power supply (-) |
|GND           |Fan ground (-)   |

# Set the desired temperature
Instructions for operating the W1209 temperature controller are documented by [MagicDuino](http://magicduino.com/Images/ItemsMedia/File/7186.pdf).
