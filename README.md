# Visibility Algorithm

The objective is the calculate the visible region (blue area) from a point Q (red dot) on a 2d plane having a set of non intersecting line segments (green line segments). 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigExpected.png" width="400" height="350">

## Our Output Video 

https://github.com/nishita96/VisibilityAlgorithm/assets/24264347/a9db5e29-fac3-4c8e-8475-f5d186ae7d48




individual line segment processing 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigSingleSegment.png" width="300" height="250">

Pair of line segments 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigPairSegments.png" width="300" height="250">


pairwise merging 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigPairResult.png" width="300" height="250">


Resultant area of visibility 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigAreaResult.png" width="300" height="250">


## Our test data 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigTestData.png" width="300" height="250">



## On CPU

serially merging 

## On GPU 

parallely merging 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigDivideAndConquer.png" width="200" height="150">


## Performance Comparison
Sequential CPU implementation vs reduction sum GPU implementation 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigResultGraph.png" width="400" height="250">

## Setup for CPU

- Install [https://openframeworks.cc/download/](#Openframeworks) 
- CPP development environment 
- Make project in it with the cloned repo

## Setup for GPU

- Install [CUDA](https://docs.nvidia.com/cuda/)
- CPP development environment 
- Clone the repo
- Build using make and Run

## Future Scope 
- The GPU result can also be incorporated to display the result using OpenFrameworks.
- It can process intersecting line and split them into 2 line segments each to process using the same algorithm. 

#### Reference paper

<!-- ![Screenshot of emptyExample](emptyExample.png)

### Learning Objectives -->

[comment]: <> (This example is the simplest possible openFrameworks app! It does nothing. )

<!-- ...Well, *almost* nothing. Although it may not be apparent, the emptyExample activates all of the default system states. (For example, it sets the default fill color to white; it just doesn't happen to draw anything with it.)

The emptyExample is great for making sure that your openFrameworks development environment is compiling properly. It can also be useful as a "starter template" for making simple programs. 

The emptyExample will help you understand what are the bare necessities of an openFrameworks program. In this regard, you can think of it as a "Hello World" for OF.  -->


<!-- ### Expected Behavior

When launching this app, you should see a light-gray screen. 

* There's no interaction. 
* There's nothing to see. 
* That's it.  -->

