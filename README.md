#SFML Bouncing Shapes

This project was made to learn about SFML, a library used for the development of games. 
The program will read a config.txt file, and create shapes based on the specification sof the file, for example, 
```
Circle CGreen 100 100 -0.03 0.02 0 255 0 50
```
Will create a Circle shape, with the name CGreen, a X and Y position of (100, 100) a X and Y speed of (-0.03, 0.02), a RGB colour (0, 255, 0) and a radius of 50. 
Or 
```
Rectangle RRed 200 200 0.1 0.15 255 0 0 50 25
```
Will create a Rectangle shape, with the name RRed, a X and Y position of (200, 200) a X and Y speed of (0.1, 0.15), a RGB colour (255, 0, 0) a height of 50 and a width of 25px.

The config also specifies the size of the window and the font used for the text (the font must be located in the font directory in order to work).

 
##Demo
![](https://github.com/renfunny/SFML-Bouncing-Shapes/blob/main/SFML%20SHAPES.gif)
