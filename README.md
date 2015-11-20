# MassSpringSystem


A variety of animations using mass-spring systems, including a mass on a spring, a chain of masses on springs, a cube of 'jell-o', and a piece of cloth.


To run, simply 'make', then run bin/
MassSpringSystem. I also included .cproject / .project files if you would prefer to run it in eclipse. Changing models can be done by right clicking and selecting the model you wish to render. You can also apply wind to any of the models from the right click menu.

# Wind

For the simulation of wind, I simply take a base force and add on sin functions multiplied by a constant. The sin function is what causes the 'pulses' of force. I believe that this adds to the realism. This force is added at every update step, along with gravity and the spring forces. Note that the cloth is by default 50x50 point masses; I tested as high as 100x100, but the amount that the simulation slows down at this point takes away from the feel.

# Notes

For the 'jello' cube, instead of just adding point masses in a grid for the faces, I decided to add point masses connected with springs in 'planes' throughout the model. This gives it a much more 'springy' feel, and I think it looks quite cool.  