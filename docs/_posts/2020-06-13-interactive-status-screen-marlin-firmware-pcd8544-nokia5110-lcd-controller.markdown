---
layout: post
title:  "Interactive Status Screen for Marlin Firmware: an experiment using PCD8544 Nokia 5110 LCD Controller"
date:   2020-06-13 16:01:14 -0300
categories: 3d
---

<a href="{{site.baseurl}}/assets/images/marlin-nokia-5110-interactive-status-screen.jpg"><img class="border" style="float: right; width: 300px;" src="{{site.baseurl}}/assets/images/marlin-nokia-5110-interactive-status-screen.jpg"></a> This is an interactive status screen for <a href="https://marlinfw.org">Marlin 3D Printer Firmware</a> that I integrated into Marlin for a DIY 3D Printer. It enables you to access and tweak the most useful printer functions right from the status screen. The top bar (first line) <img style="width:170px" src="{{site.baseurl}}/assets/images/marlin-nokia-5110-interactive-status-screen-top.jpg"> is a set of "buttons" to access Menu, Print from SD, Home all, Preheat 1, Preheat 2, Cooldown, and Filament menu.

The other items on the status screen are also interactive: Nozzle and Bed temperature, Feedrate/speed, Flow percentage, Printing fan velocity, Z, X, and Y move. It is prepared to show up to two nozzles on screen.

When printing, the items on screen rearrange to display the printing duration, the progress, a button for pause and another one for stop. The following video shows it in operation.

<figure class="border" style="float: right">
<iframe width="400" height="225" src="https://www.youtube.com/embed/8V7FHNmuf0I?autohide=1&showinfo=0&controls=0&modestbranding=1&version=3&showsearch=0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
<figcaption>The interactive screen in operation!</figcaption>
</figure>

It is small, you can say! I definitely agree. Compared to common graphical LCD used by default on Marlin, such as the 128x64 ones, the 84x48 pixels make it difficult to figure out how to put all this info on screen and also make these nano 8x8 icons. But you know, make it with what you have! These are the ones lying around me. I hope to buy or win a larger one and implement these features for it.

Although it is an experimental thing and there are some corners to shape, you can run it on your printer! The general instructions are:

- **Wire the LCD controller**: You need four IO pins for CE (chip enable), DC (data command), DAT or DIN (Data Input), and CLK (Clock). On AVR boards, it is recommended to use 1K ohm resistors on these lines. I also have some success wiring it to Hardware SPI on AVR boards, i.e., using the SPI bus (CLK and MOSI pins) and another two IO pins for CE and DC. However, I recommend you to do not use HARDWARE_SPI, as it almost always conflicts with other SPI related stuff on Marlin, for example, SD Cards. I had to reduce the SPI clock to SPI_QUARTER_SPEED for it to work on the SPI bus.
- **Define the pins used in the pins_YOURBOARD.h file**: Add the four defines below in the pins_YOURBOARD.h, where YOURBOARD is your printer board name. Check <a href="https://github.com/thborges/Marlin/blob/stm32bluepill/Marlin/src/pins/sanguino/pins_ANET_10.h">pins_ANET_10.h</a> or <a href="https://github.com/thborges/Marlin/blob/stm32bluepill/Marlin/src/pins/stm32f1/pins_BLUE_PILL.h">pins_BLUE_PILL.h</a> for a reference.

{%highlight c %}
#define SLCD_CLK_PIN  PIN?
#define SLCD_DAT_PIN  PIN?
#define SLCD_DC_PIN   PIN?
#define SLCD_CS_PIN   PIN?
{% endhighlight %}
- **Change Configuration.h** and include a line <code>#define NOKIA5110_LCD</code> near the LCD / Controller Selection section (around line 1800). You should comment the other display controller enabled.
- **Build and upload it to your printer! ;D**

The complete source code is available <a href="https://github.com/thborges/Marlin/tree/stm32bluepill">here</a>. The changes are mainly confined in the <a href="https://github.com/thborges/Marlin/tree/stm32bluepill/Marlin/src/lcd/small_lcd">small_lcd</a> folder, except by <a href="https://github.com/thborges/Marlin/commit/3a4ae390368d0f07c2b4621fe166704faf2ca5b4#diff-54ada0858fc4ed0c0dfc06aa12378c1d">small changes in ultralcd.cpp</a> and the inclusion of some lines in Conditionals_LCD.h. It is for Marlin 2.0 and supports both ARM and AVR boards.


{% include disqus.html %}
