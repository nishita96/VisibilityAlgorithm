# Visibility Algorithm

The objective is the calculate the visible region (blue area) from a point Q (red dot) on a 2d plane having a set of non intersecting line segments (green line segments). 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigExpected.png" width="400" height="350">

## Our Output Video 

https://github.com/nishita96/VisibilityAlgorithm/assets/24264347/a9db5e29-fac3-4c8e-8475-f5d186ae7d48

### Pre Processing

- Transalte the origin to the point Q 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigPre1.png" width="300" height="250">

- Remove points collinear with the point Q as they will not affect the visibilty 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigPre2.png" width="300" height="250">

- Split the line segments passing through the positive X axis at the X axis into 2. As we need the visibilty algorithm to scan from 0 to 360 degrees.

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigPre3.png" width="300" height="250">

### Merging 

#### Vrays
- Rays originating at the point Q going to infinity passing through the end points of the line segments
- Have 4 components 
  - djhdghj

- Process individual line segment processing to get Vrays

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

- Sequential pre processing for each line segment.
- Sequentially merging each line segment into the resultant list of Vrays giving the visibility area. 
- Taking result to draw the output using Openframeworks library. 

## On GPU 

- Parallely pre processing all line segments to get pairs of Vrays (did not perform well due to high overhead, hence removed).
- Sequentially pre processing all line segments.
- Parallely merging line segments from bottom up (reduction sum method) to get resultant Vrays.

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigDivideAndConquer.png" width="200" height="150">


## Performance Comparison
Sequential CPU implementation vs reduction sum GPU implementation 

<img src="https://github.com/nishita96/VisibilityAlgorithm/blob/main/images/FigResultGraph.png" width="400" height="250">

## Setup for CPU

- Install [Openframeworks](https://openframeworks.cc/download/) 
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
[GPU Parallel Visibility Algorithm for a Set of Segments Using Merge Path](https://www.sciencedirect.com/science/article/pii/S1571066119300052?ref=pdf_download&fr=RR-2&rr=7c1e6026794ea044)
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

