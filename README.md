# CarTrackingAndCounting

This application aims to count the number of vehicules passing into a highway.
There are two counters : one for the vehicules going from the top to the bottom of the image (left side), and the other for
the vehicules that go the other way (right side).

The algorithm works by making a difference between two frames of the video. We then obtain a diff image, that we can ameliorate by
using morphological operations.

The user must do some manipulations in order to use this application :

1. Define the tracks
The user must define the boundaries of the tracks. He has to click twice to define a line that represent the first edge of the track, 
then again twice to define the other edge (see below)

+ A11          + A12     + A21          + A22 
|              |         |              |    
|              |         |              |
|              |         |              |
|      ↓       |         |      ↑       |
|              |         |              |
|              |         |              |
+ B11          + B12     + B21          + B22

In our example, the user must first click on A11 and B11 to trace the first edge of the left track, then complete this track by
clicking on A12 and B12.
To trace the edges of the right track he will have to click on A21, B21, A22 and B22

Thoses tracks are used to dismiss false negatives that could be caused by leaves or other objects moving outside of the highway.
With those tracks, we are sure that, if we have an object moving inside the tracks, then it is a vehicule.

When the tracks are ok, the cars are recognized : a red rectngle is drawned around them.

The next step for the user is to define a counting line.


2. Define a counting line
The counting line will be used to count any vehicules that cross it.
The user must define it by clicking on two points :

	|              |         |              |
	|      ↓       |         |      ↑       |
	|              |         |              |
 C +----------------------------------------------------+ D
	|              |         |              |
	|              |         |              |

On the above example, the user must click on C and D to define the counting line.


3. Adjust the precision for the counting line
The final step is to adjust a P parameter for the counting line. 
This parameter is located on a panel and is by default equal to zero (no counting).
Adjusting too high this parameter can lead to errors.

On the same panel where the P parameter is located, is another parameter C, which stands for center.
The C parameter has only two values (0 and 1). When C = 1, we can see the center of the objects and their coordinates.

