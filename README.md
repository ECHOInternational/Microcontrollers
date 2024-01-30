# Microcontroller Projects at ECHO

*by Tim Motis

This repository houses code for the programming of low-cost microcontrollers for agricultural applications at ECHO, a non-profit organization based in Southwest Florida dedicated to the needs of small-scale farmers in low-resource settings internationally. Microntroller-based devices are an inexpensive approach to real-time monitoring of environmental parameters affecting agricultural productivity. They do, however, require an investment of time to program and assemble. That is why programming and wiring information are presented for the various devices included in this repository. You will still need to do some fabrication to suit your own needs, and with materials available in your location, but hopefully the instructions and programming sketches will be of help to you. These projects will be most applicable to those with an interest in research at some level.   

Getting started is perhaps the most expensive part of the journey in working with microcontrollers. Aside from the microcontrollers and supporting components, you will need a soldering iron, solder, a wire-stripping tool that works well with thin (22-24 gauge) wire, breadboard for building and testing circuits, pin headers, and wires. Many electrical components come in packs or kits with multiple pieces, meaning one purchase will provide materials for more than one project. 

The information here assumes basic knowledge of programming with Arduino IDE software and soldering. There is an abundance of tutorial information online for these things. A few links for beginners are:
* Arduino IDE software download page (click on the operating system that matches your computer): https://www.arduino.cc/en/software
* Arduino IDE instructions for downloading software and links to other topics you will need to learn about (e.g., libraries and cores): https://www.arduino.cc/en/Guide
* Basic elements of a sketch (program): https://docs.arduino.cc/learn/programming/sketches
* Soldering guide: https://docs.arduino.cc/learn/electronics/soldering-basics
* Breadboard use: https://www.seeedstudio.com/blog/2020/01/06/how-to-use-a-breadboard-wiring-circuit-and-arduino-interfacing/
* Working with pin headers: https://learn.adafruit.com/how-to-solder-headers?view=all#male-headers; 

Once you get a circuit to work, the next challenge is to connect sensors and protect components in a way that makes sense for application in the field. I rely a lot on wooden enclosures, which seem to keep components cooler than plastic enclosures exposed to the hot sun. Metal sheeting or tin foil can be used for weatherproofing. As much as possible, pass sensor wires through conduits of some kind, such as PVC piping or drip irrigation supply tubing. For outdoor weather stations, incoming wires should be directed through the bottom of the enclosure so that if there are leaks, water will drain out the bottom, reducing the likelihood of ruining electrical components. 

There are, no doubt, improvements that could be made to the sketches in this repository. You are welcome to adapt the code in each sketch to your needs, and I am certainly open to suggestions.
