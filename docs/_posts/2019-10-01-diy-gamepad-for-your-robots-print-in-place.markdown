---
layout: post
title:  "A DIY Wireless Gamepad for Your Robots: 3D Model, Circuit, and Firmware"
date:   2019-10-01 18:44:14 -0200
categories: robots
---

<a href="{{site.baseurl}}/assets/images/diyctrl_main.jpeg"><img class="border" style="float: right; width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_main.jpeg"></a>

This page describes a DIY, 3D printed, wireless gamepad designed for controlling an arm robot. It was developed under a technological research project¹ in Brazil, at the Federal University of Jataí - GO². The technological project aims to build very low-cost robots to use in educational activities in Brazilian public schools.

Although there are many readily available gamepad or similar wireless controllers, and their quality is a must, the cost of bringing several of them to enable a class activity didn't fit our budget. By the way, we also want to empower the students and encourage them to pursue STEM careers, and nothing better than presenting something they can create themselves in a maker lab!

# Summary of Characteristics

The gamepad has two axis, each one controlling four buttons disposed in the left, right, top, bottom positions. There are two additional buttons in the center and a feedback LED that flashes in every move. Its main board uses a small PIC processor, powered by two CR2032 3V button cells.

Main characteristics:

|--------------|:---------|
| Dimensions (WxDxH), without antenna | 127x60x40mm |
| Filament used to print | ~50g from ABS, PLA, PETG |
| Printed pieces | Main frame, two rounded buttons, and two axis handles |
| Battery | Two CR2032 in series, 3V each |
| Processor | PIC, Model 16F676P |

# The Print-In-Place 3D Model

<figure style="display: inline-flex;">
<img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_model.png">
<img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_model_pieces.png">
</figure>

The pictures above show a model of the "assembled" gamepad (left) and its internal pieces (right). It was designed to use no screws or glue when assembling, and also requires no support to print.

The following pieces should be printed:

* The external frame (in brown) and the axis "eye-balls", that together compose a <a href="">print-in-place</a> object (i.e., you print it in position, not separated),
* Two round buttons, and
* Two axis handles.

<iframe class="border" style="float: right" width="300" src="https://www.youtube.com/embed/uetGYyZBsZk?autohide=1&showinfo=0&controls=0&modestbranding=1&version=3&showsearch=0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

The most complex piece to print is the print-in-place frame. Sometimes the axis "eye-balls" fuse to the external frame, but you can successfully print it with 0.2 and 0.3mm layer height in ABS or PLA in a well calibrated printer. I've printed it successfully in a very customized Anet A8, thus it should be not that difficult in a better printer.

This <a href="{{site.baseurl}}/assets/stls/diyctrl_all.stl">.stl file</a> contains all needed objects in its correct orientation.

# The Circuit Board

<figure style="display: inline-flex;">
<img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_board.png">
<img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_schematic.png">
</figure>

The PCB is designed to fit the 3D printed frame. It has rounded sides and two holes of 26mm diameter like pictured above. The holes free the movement of the axis "eye-balls" making their pads touch the buttons.

The board has a pin header to program the PIC processor. It was designed to be programmed by high-voltage protocol, that is, you will provide ~10V DC on MCLR pin. What motivated us towards the choice of a PIC was only the price. For sure, an Atmega processor is a more popular choice nowadays, and have a simpler programming interface. However, it costs almost double the price where we live. We are evaluating other processor options even cheaper, like the ones of Padauk. If you have some indication, please comment below. We thank you!

This is the list of components you will need to make the board (Bill of Materials - BOM):

|-|-|
| 8 | 6×6x5.5mm Right Angle Micro Momentary Tactile Push Button, like this one <https://ebay.to/3bno2Ot> |
| 2 | 6x6x5mm Momentary Tactile Push Button, like this one <https://ebay.to/2wztAqt> |
| 1 | LED 3mm with long legs |
| 6 | 10 kΩ resistors, 1/4W was enough |
| 1 | 1 kΩ resistor, 1/4W was enough |
| 12x6cm | Single sided copper board to etch |
| 1 | 433 MHz RF module + antenna, like this one <https://ebay.to/2UgkPur> |
| 20cm | Enameled copper wire gauge xxx for antenna |

Of course, depending on your preferred way of making a board, you should add materials to etch and solder the components on it. An option is to order it pre-fabricated from companies like <a href="https://jlcpcb.com">JLC PCB</a> or <a href="https://www.pcbway.com">PCB Way</a>. Just use the provided Gerber files bellow when ordering.

* <a href="{{site.baseurl}}/assets/boards/diyctrl_eagle_board_files.zip">Eagle PCB files and Gerber files</a>
* <a href="{{site.baseurl}}/assets/boards/diyctrl_board.pdf">Board PDF, e.g., for tonner transfer if you haven't Eagle</a>.

Just to mention an alternative, you can use a two-wire cable (wired to GND and SIG) instead of the wireless module. This can be the case if wireless turns to be faulty for your application due to fidelity, poor signal, antennas, etc.


# The Software and Wireless Protocol

The wireless protocol is a pulse-period encoded system, based on the protocol designed in <https://www.romanblack.com/RF/cheapRFmodules.htm>.

In the following file there is the .c version of the firmware and a pre-compiled version using SDCC for PIC16F676P. There is also an Arduino project and a python file that you can use to transfer the firmware to the PIC. All the credit and origin of the files are given in the comments. I made very small changes compared to the original, mainly to make them work in my environment.

Of course, the receiver side, i.e., the "thing" that you want to control, need to receive and process the signal. Below there is also an example of receiving the signal in an Atmega processor.

* <a href="{{site.baseurl}}/assets/code/diyctrl_firmware.zip">Firmware and stuff needed to program it on PIC</a>
* <a href="">Arduino example on the receiver side</a>

# Additional Assembled Photos for Reference

<figure style="display: inline-flex;">
<a href="{{site.baseurl}}/assets/images/diyctrl_printer.jpeg"><img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_printer.jpeg"></a>
<a href="{{site.baseurl}}/assets/images/diyctrl_internal.jpeg"><img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_internal.jpeg"></a>
<a href="{{site.baseurl}}/assets/images/diyctrl_acid_bath.jpeg"><img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_acid_bath.jpeg"></a>
</figure>
<figure style="display: inline-flex;">
<a href="{{site.baseurl}}/assets/images/diyctrl_back_board.jpeg"><img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_back_board.jpeg"></a>
<a href="{{site.baseurl}}/assets/images/diyctrl_back.jpeg"><img class="border" style="width: 300px;" src="{{site.baseurl}}/assets/images/diyctrl_back.jpeg"></a>
<div style="margin: 5px; padding: 5px; width: 300px;"/>
</figure>




# The Joy of Seeing it in Action and Recruiting Future STEM Scientists!

All the hard work trough this complex pieces of design and software deserves a reward! This gamepad is used in science fairs in our university. We encourage boys and girls of any age to use it and complete a challenge with ping-pong balls. See it in action in the videos below!

# Points for Improvement

Although there are many, the ones we like to highlight are:

* Power on-off. Although the PIC processor is programmed to sleep, a power on-off switch will be better than removing the battery when not in use.
* Turns axis pads adjustable. The contact of the axis pads with PCB buttons can skip presses sometimes, depending on the pressure applied by the user. Adding little holes for small screws in the pads will allow to tune the contact and make it less faulty.
* Improve the battery slot. The actual PCB was designed with a button cell slot in mind. However, it turns out to be difficult to find one in local market. Maybe a slot in the controller frame can be crafted and simpler contacts can be soldered in the PCB to interface with the actual or another model of battery.

¹ Especificação e Construção de Protótipos Funcionais de Kits Robóticos de Baixo Custo para uso em Processos de Ensino-Aprendizagem.

² Former Regional Jataí of Federal University of Goiás.

{% include disqus.html %}
