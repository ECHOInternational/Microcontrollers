
## Wiring

![TempHumMonitorWiring](https://user-images.githubusercontent.com/69003593/200435268-81bb1957-a4b3-4588-9a11-890ec4e6d296.jpg)
To avoid reverse polarity (connecting power[VCC/volts] poower to ground), it is best to stacy consistent with wire colors for ground and poweer wires. Notice that in the diagram above, ground wires are always black and wires to power (5 or 3 volts on the Wemos) are always red.

The 4-pack battery holder will hold 4 AA batteries in series, meaning that the positive end of one battery is connected to the negative side of the adjacent battery. With batteries connected in series, voltage of each battery is summed. Since the voltage of a new/fully charged AA battery is about 1.2 (for recharegable batteries) or 1.5 (for non-rechargeable batteries) volts, the power coming out of the red wire with all four batteries is over 5 volts (e.g., 1.2 X 4 = 4.6 volts). Thus, make sure to connect the red wire from the battery pack to 5 volts on the Wemos D1 Mini. If you connect the red wire on the battery pack to 3 volts on the Wemos D1 Mini, you could damage the sensor or microcontroller.


Battery holder
|Wires       |   	  |Wemos D1 Mini|
|:-----------|:----:|:----------:|
|GND	       |--->	|GND         |
|VCC	       |--->	|5v          |



Temperature/humidity sensor
|SHT31 sensor|   	  |Wemos D1 Mini|
|:-----------|:----:|:----------:|
|GND	       |--->	|GND         |
|VCC	       |--->	|3v         |
|SCL	       |--->	|D1         |
|SDA	       |--->	|D2         |

Reset wire (grey wire shown in the diagram) will connect D0 and reset (RST) on the Wemos D1 Mini. This wire is needed if you use the sketch/code for waking up the Wemos D1 Mini after a period of deep sleep.
