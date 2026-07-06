# Automatic-Glasses-Pusher
Hello dear fallouters!

We're the official french team of fallout, (vive les baguettes 🥖)

We are proud to present the best project of the entire event: the AUTOMATIC GLASSES PUSHER !  
Tired of constantly and manually adjust your glasses up your nose ? We are the solution you are looking for !  
Thanks to our incredible invention, you will never ever have to stop what you are doing to see as clear as possible !

Our special machine will detect the fall (ahaha get it) of your glasses and push it with our two strong servos with their custom special arms to safely put your glasses where they belong !


In addition to that, you will have a cool ARGB strip on your head to give you indication and to decorate your beautiful head :)  
It will also send a message to any slack channel you desire to alert your friends that our incredible invention has done his job and helped your in your amazing day!

# Zine page
A shiny poster to show off our baby!

<img src=zine.png width=550>

# The way it works
## Detecting the fall
To detect when you're glasses are too low, we setuped an ultrasonic sensor facing your cheek (that you can adjust by rotating the structure holding the sensor), then you'll have to calibrate the sensor by putting your glasses in both up and down position and then pressing the button on your head so the microcontroller can register the distance associated with both these positions. 

If you accidently move the sensor or remove the AGP (Automatic Glasses Pusher) you can just recalibrate by pressing the button more than 5s, the AGP will detect any anomaly and you'll have just to recalibrate :)
## Adjusting the glasses
Two SG90(s) servos are placed underneath your glasses (on both sides), and with it's curved arm  will gently push your glasses right up your noses, it is simple as it gets :)

Take a look at this amazing animation for better understanding!  
<img src=./Pictures/servos.gif width=400>
## Sending the slack message
When starting, the Seeed Xiao esp32-c3 will connect to an hotspot (LED will indicate when it's done) and thanks to an incoming webhook from slack, a bot will be able to send any message to any channel he's been allowed to ! 
## Electrically speaking
Everything is alimented by an external power bank that you can fit in your pockets so it'll last all day!  
Everything else is connected to the esp32 withtout an pcb cause it would have been overkilled, two 220µF capacitors has been added two the servos because it has a peak of current when starting.  
Otherwise everything is basic, input_pullup button, ws2812b led strip, HC-SR04 ultrasonic sensor etc... Nothing complicated to wire!

Here's a simplified wiring schematic of the project (without all the powering part):  
<img src=./Pictures/wiring.png width=500>
## CAD
The most complicated part was to design this kind of headset from so it'll fits all heads. We've been inspired by and headband form for the main body, then added branches kind of like a headset microphone to put it in front of you wihtout taking too much space ! 

Here's an amazing picture of our CAD :)

<img src=./Pictures/cad_face.png width=500>

# Special thanks

- A special thanks to, of course, everyone who organized this amazing event !
- Special thanks for the sponsor for making this event possible and giving so much stuff!
- Special thanks for Prakshab and Nathan for being cool roomates!
- Special thanks to the hot glue gun for being hot and sticky
- Special thanks to soup for being cute and fluffy
- Special thanks for all the sliding glasses of the world making our project alive
- Special thanks to "Mauvaise nouvelle" for making my mouse slide

Cya :)

UwU GanG


