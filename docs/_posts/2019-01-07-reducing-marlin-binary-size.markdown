---
layout: post
title:  "Reducing Marlin firmware binary size to fit in your 3D printer board memory"
date:   2019-01-07 18:44:14 -0200
categories: marlin
---

How to reduce the Marlin binary size? Here are a set of special compilation flags and options you can disable to save **up to 16.3 Kb** of memory for common AVR boards. Some build flags are also provided for ARM boards at the end. 

# Special compilation flags

Changing them is easy wether you are using Arduino IDE or PlataformIO. 

If you are using PlataformIO, it is easier. Just create a new build target in platformio.ini, copying the original config for your printer board and add the new flags:

{%highlight ini %}
build_unflags = -g -ggdb
build_flags   = ${common.build_flags} -fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax -mcall-prologues
{% endhighlight %}

In Arduino IDE, you should edit the file platform.local.txt for your board. It is inside the hardware folder, together with your other Arduino project folders, or in the Arduino instalation folder. Search for it.

Content to add in platform.local.txt:
{% highlight ini %}
compiler.c.extra_flags=-fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax -mcall-prologues
compiler.cpp.extra_flags=-fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax -mcall-prologues
compiler.c.elf.extra_flags=-Wl,--relax
{% endhighlight %}

Not that you should, but you can even try more aggressive optimizations supposed to change a little the printer execution time for some routines:

{%highlight sh %}
-finline-limit=3 -ffast-math
{% endhighlight %}

From these options, **inline-limit** and **mcall-prologues** can slightly interfere in execution velocity due to additional function calls. **Fast-math**, in turn, changes the way math is made in several ways, and I'm not sure it affects Marlin. You can check a list of modifications in using the fast-math flag in this [Stack Overflow answer](https://stackoverflow.com/questions/7420665/what-does-gccs-ffast-math-actually-do/22135559#22135559).

You can verify what each of these options does in avr-gcc man page or online at [avr-gcc manpage](http://ccrma.stanford.edu/planetccrma/man/man1/avr-gcc.1.html). Check also this list of [optimizations for AVR](https://p5r.uk/blog/2008/avr-gcc-optimisations.html).


Some of my sizes using PlatformIO to build Marlin 1.1.9 for a customized version of an Anet A8 with filament runout sensor, bed probe, graphical display, wifi and others:

| Size (bytes) | Options |
|--------------:|:---------|
| 125216 | Default flags |
| 124240 | -fno-tree-scev-cprop -fno-split-wide-types |
| 123324 | -fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax |
| 119730 | -fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax -mcall-prologues |
| 118872 | -fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax -mcall-prologues -finline-limit=3 -ffast-math |


# Updated sizes for Marlin 2.0

For Marlin 2.0, the same flags provided above apply. The binary size for the default Creality Ender 3, for example, are:

| Size (bytes) | Options |
|--------------:|:---------|
| 126340 (97.1% of 130048) | Default flags for sanguino1284p |
| 125782 | -fno-tree-scev-cprop -fno-split-wide-types |
| 124770 | -fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax |
| 120808 | -fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax -mcall-prologues |
| 119898 (92.2% of 130048) | -fno-tree-scev-cprop -fno-split-wide-types -Wl,--relax -mcall-prologues -finline-limit=3 -ffast-math |

For Ender 3, the flags were able to save **up to 6.4 Kb** for you to enable exceptional options!

# Ready to disable some unused options?

Disabling some options in Configuration.h and Configuration_adv.h can help you increase the space available for more exceptional options:

| Option | Description | Size recovered (up to) |
|--------|-------------|----------------|
| SLIM_LCD_MENUS | Remove extraneous LCD menus, such as those in advanced configurations menu (velocity, acceleration, etc). You still will be able to configure them through M gcode equivalent commands. | 4.7 Kb |
| ARC_SUPORT | Support to Arc movements. As far as I know, this is mostly used in CNC or laser cutters. The slicers I know can't output arc movements. | 3.3 Kb |
| LCD_INFO_MENU | A menu with printer information. | 1.7 Kb |
| SHOW_BOOTSCREEN, SHOW_CUSTOM_BOOTSCREEN, CUSTOM_STATUS_SCREEN_IMAGE | In my opinion, it loses some personality, but the three together can save up to: | 1.4 Kb |

# Build flags for Marlin 2.0 and ARM?

Recently, I built Marlin for a custom ARM board base on an STM32 F103CB (Also known as Blue Pill). All the compiler flags from above can be used, except by -mcall-prologues. However, they are not so beneficial as for AVR. 

The killer option for ARM is to use the nanolib, passing --specs=nano.specs to the linker. To do it, create a new file named add_nanolib.py in the root directory of marlin with the following content:

{% highlight python %}
Import("env")
env.Append(LINKFLAGS=["--specs=nano.specs"])
{% endhighlight %}

and add the extra_script option in platformio.ini for your board:
{% highlight python %}
extra_scripts = add_nanolib.py
{% endhighlight %}

The sizes below if for platform = ststm32, framework = arduino, and board = genericSTM32F103CB.

| Size (bytes) | build_flags |
|--------------:|:---------|
| 127688 (97.4% of 131072) | build_flags = !python Marlin/src/HAL/STM32F1/build_flags.py ${common.build_flags} -DSERIAL_USB |
| 126816 | ... -ffast-math |
| **108964** | ... add extra_scripts = add_nanolib.py |

Of course, you can also disable the options in Configuration.h and they should save up equivalent memory sizes. The SLIM_LCD_MENUS, for example, release even more memory on ARM, freeing 5.4 Kb!

{% include disqus.html %}
