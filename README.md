# Sentry






> Built in [Breadboard](https://breadboard.hackclub.com), a Hack Club program. This project took ~1.6 hours of work.



## What It Does

Smart study buddy for your desk!


## How It Works

The circuit is captured in `breadboard-project.json`, and the firmware that runs it is in the `firmware/` folder.


## How To Use It

How to use it:
- Power it on, the screen shows IDLE and your streak
- Sit down, and the PIR sensor detects you are there and screen. displays "Get ready..."
- Study for 25 minutes to log a streak session, LED shows: green=good, amber = ok, red=bad. Staying over a minute a way from your desk results in a drop in your score
- After the 25 minutes, it goes to a 5 minute break, and blue LED. You can press the button to skip the break
- Loops back to another session automatically
- No motion for 10 minutes results in sleep mode being activated. Any motion will lead to IDLE state being activated again.


## Demo

- **Simulate it live:** [https://breadboard.hackclub.com/share/168](https://breadboard.hackclub.com/share/168), runs the firmware in the Breadboard simulator
- **View the design:** [https://taniwankenobi.github.io/breadboard-plays/p/168/](https://taniwankenobi.github.io/breadboard-plays/p/168/)


## Schematic

The editor snapshot is in `breadboard-project.json`.


## Bill of Materials

| Part | Quantity |
| --- | --- |
| ESP32 | 1 |
| OLED display | 1 |
| PIR sensor | 1 |
| Light sensor | 1 |
| DHT22 sensor | 1 |
| Push button | 1 |
| RGB LED | 1 |
| Resistor | 3 |
| Buzzer | 1 |
| Breadboard | 1 |
| Jumper wires | 20 |


## Firmware

Firmware files are in the `firmware/` folder.


## Build Journal

Build journal entries are kept in [`journals.md`](journals.md).


---



*Made in [Breadboard](https://breadboard.hackclub.com) — 1.6h of work*



<p align="center"><img src="https://cdn.hackclub.com/019efae7-6857-75a2-8bc1-2618087b4eae/a%20bred%20tanuki%20(3).png" width="64" alt="Breadboard mascot" /></p>
