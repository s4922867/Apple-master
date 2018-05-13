#Computing  for  Animation  1  Assignment

Miguel  Goncalves  s4922867



Apple  Game
----------



Controls
--------

  -  The  Right  and  Left  keys  move  the  basket  right  and  left

  -  R  restarts  the  game



Goal
----

The  player's  goal  is  to  get  the  highest  score  by  collecting  the  apples  with  the  basket



Game  Over
---------

every  time  an  apple  falls  off  screen  without  being  caught  by  the  basket  the  player  lose  10  Health.  once  the  players  health  reaches  0  the  game  ends



Dependencies
------------

Libraries  needed


|  Name  |  Version  |
|  ----  | ------|
|  C++  | 11|
|OpenGL|4.1/4.3|
|  NGL[(latest  version  available  here)](https://github.com/NCCA/NGL)  |  6  |

Classes
---------
| Class Name | Description |
|--|--|
| NGLScene | Contains the main functions and and things like collisions and the timer  |
| Apple | Draws and moves the apples on screen |
| Basket | Draws and moves the basket on screen |

Algorithms
-------------
the program first sets up all the apples and the basket on the scene then calls a function to draw them. there is a function that changes the basket's position depending on if the player presses the right and lleft keys. There is a timer that repeats every 30ms. when the timer reaches 30 it makes the apples increment downwards, it also checks if they collide with the basket, or if they failed to collide with the basket and fell off screen. if they fail to collide with the basket the player looses some health points, if it does collide with the basket the player gets +1 to their score and health. the higher the score the faster the apples fall down, the number of apples also increase. there are 8 apples that can be on screen at once. when the collide with the basket or fall off screen I simply change their position to the top of the screen again. and they keep falling. if the player reaches 0 health points then the game ends. then the player can see his score and the highest score, if the player scores higher than the highest score, the program overwrites the highest score. this saves on multiple openings of the program. the player can also press R to restart the game, which changes all the values like apple positions, score, speed, etc... to the ones they were when the player first runs the program.

Problems
------------
sometimes apples will clip with other apples, this is because when the difficulty increases new apples just start falling regardless if there are other apples that share the same x coordinates.

Conclusion
-------------
to conclude the game works, its satisfying enough to play and it gets more challenging to keep things interesting. even though there are some problems like the apple clipping i think it doesn't detract from the game in the most part. if i had the time id add a collision system for the apples with each other.
