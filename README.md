# Dyno The Dynamic Widget Pad, or simply, Dyno

A dual display for helpful desktop widgets with touchscreen capabilities powered by AI depth sensing. Undercity 2025!
-

Dyno is Undercity's only touchscreen display powered by strong AI models. With different display modes capable of working with your music app and location-based weather, this project combines innovation, aesthetics, and practicality. Since we did not have access to capacitative-touch hardware during the hackathon, we improvised with a novel AI approach to depth sensing and finger detection. This approach is often able to pinpoint the user's exact place of touch and works well for our LED-powered "buttons." The technical capability of this board enables a practical approach to the user experience, prioritizing play/pause functions, weather readings based on where you are, as well as future hotkeys and macros.

# Advanced Capabilities, How it Works, and Why It's Worth Your Vote

As mentioned above, Dyno utilizes a heavily-trained AI model to sense your fingers' contact with the board without ever needing to use capacitive touch surfaces. The core of the project is the HUB75 64x32px 320x160mm RGB LED Matrix board, which was very difficult to work with due to the particular electrical connections and specific power supplies needed as well as the lack of documentation for firmware. It is powered by a 5V 3A (the board accepts 5V 4A max) power outlet brick, which transfers the power to the board through a stripped USB-A cable connected to a 4-pin female header. Communicating with a Raspberry Pi Pico through GPIO pins, the board displays messages and icons sent by a laptop to the microcontroller. Additionally, the laptop also interfaces with the microcontroller to send data (such as weather information and recognized button presses), which the microcontroller sends to the board. On top of that, a camera only rated for 1080p video powers all of the computer vision that fuels the touch detection.

# Electronics

The most difficult piece to work with is most definitely the HUB75 RGB LED matrix. Unfortunately, a very specific power supply - 5V and 2-4A (4A with full brightness) - is needed for this board, and you cannot power it through a microcontroller. Connections must be very securely soldered, and while the 3.3V logic can transfer data to the board, the board will flicker because it expects 5V. Unfortunately, we did not have access to another board as big as this one during the hackathon, so about half our time was spent ensuring that the board will light up the correct LEDs. The lack of materials - which we tried to make up through visiting various hardware shops around the city we were in - posed a significant challenge to our project.

We did not use a PCB; it would be inconvenient because of the spacing of our parts and the tools available to us. We made a schematic which also doubles as a convenient wiring diagram, however.

# CAD


# Software
