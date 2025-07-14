# Dyno The Dynamic Widget Pad, or simply, Dyno

A dual display for helpful desktop widgets with touchscreen capabilities powered by AI depth sensing. Undercity 2025!
-

Dyno is Undercity's only touchscreen display, with the goal of being able to control different functions (for example, Spotify.)  With different display modes capable of working with your music app and location-based weather, this project combines innovation, aesthetics, and practicality. Since we did not have access to capacitive-touch hardware during the hackathon, we improvised with a novel AI approach to depth sensing and finger detection. This approach is often able to pinpoint the user's exact place of touch and works well for our LED-powered "buttons." The technical capability of this board enables a practical approach to the user experience, prioritizing play/pause functions, weather readings based on where you are, as well as future hotkeys and macros.

# Advanced Capabilities, How it Works, and Why It's Worth Your Vote

As mentioned above, Dyno utilizes a heavily-trained AI model to sense your fingers' contact with the board without ever needing to use capacitive touch surfaces. The core of the project is the HUB75 64x32px 320x160mm RGB LED Matrix board, which was very difficult to work with due to the particular electrical connections and specific power supplies needed as well as the lack of documentation for firmware. It is powered by a 5V 3A (the board accepts 5V 4A max) power outlet brick, which transfers the power to the board through a stripped USB-A cable connected to a 4-pin female header. Communicating with a Raspberry Pi Pico through GPIO pins, the board displays messages and icons sent by a laptop to the microcontroller. Additionally, the laptop also interfaces with the microcontroller to send data (such as weather information and recognized button presses), which the microcontroller sends to the board. On top of that, a camera only rated for 1080p video powers all of the computer vision that fuels the touch detection.

# Electronics

The most difficult piece to work with is most definitely the HUB75 RGB LED matrix especially with its documentation. We spent a good amount of time just powering and turning on the display. Unfortunately, a very specific power supply - 5V and 2-4A (4A with full brightness) - is needed for this board, and you cannot power it through a microcontroller. Connections must be very securely soldered, and while the 3.3V logic can transfer data to the board, the board will flicker because it expects 5V. Unfortunately, we did not have access to another board as big as this one during the hackathon, so about half our time was spent ensuring that the board will light up the correct LEDs. The lack of materials, which we tried to make up through visiting various hardware shops around the city we were in, posed a significant challenge to our project.

We did not use a PCB; it would be inconvenient because of the spacing of our parts and the tools available to us. We made a schematic which also doubles as a convenient wiring diagram, however. Part sourcing and our changing schematic were a challenge that we learned to constantly deal with, though.

<img width="986" height="523" alt="image" src="https://github.com/user-attachments/assets/9cdec415-79bf-4b32-af99-e01da11e8a11" />

# CAD
The Case consists of a simple screen mount with space under it and a cable hole for routing. It was split in two due to the size of the print relative to the Bau A1 Mini build plate. Due to the print queue, I essentially just included the overall frame required to not crush the components.
OG Case (included LEDS and looked decent):
<img width="1095" height="873" alt="image" src="https://github.com/user-attachments/assets/df87a08a-3050-4777-8ece-a78e907365ba" />
 
Case 2.0 (framework, which is the laptop I need :D )
<img width="1554" height="1014" alt="image" src="https://github.com/user-attachments/assets/967534c1-5d5f-456c-9ac4-fefae9bc4e2d" />


# Software
Hard af lol. One of the most important peices to make and one of the hardest. Whether it's using computer vision to track approx. location of your finger on the matrix. Or whehter it's created a complex UI to render content, or even using psudo terminal commands to control the volume!

# Video Demo
https://youtube.com/shorts/ZZUqsPvcY2g?si=fNzoMuIu14DnfsCS
