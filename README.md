# algo-animator

An interactive program to visualize sorting algorithms.  
I also made a video about this project [Youtube](https://youtu.be/KvOI88s-FsU)

This project is inspired by - off course - the video by Timo Bingmann called 
[15 sorting algorithms in 6 minutes](https://www.youtube.com/watch?v=kPRA0W1kECg).

![Image 1](repo/1.png)
![Image 2](repo/2.png)


## Compile

```bash
mkdir build
cd build
cmake ..
make
```

*Currently I've only tested this project on Linux. However, I would like to prepare it so 
that it can be used on Mac and Windows. However, my cross-platform experience is almost 
non-existent. Maybe in the future I'll check how to do it*

## Run

To customize the program you can pass the following arguments. 

```bash
./algo_animator WINDOW_WIDTH WINDOW_HEIGHT RECTANGLE_WIDTH SPACE_BETWEEN_RECTANGLES
```

The default values are:

```bash
WINDOW_WIDTH = 1920
WINDOW_HEIGHT = 1080
RECTANGLE_WIDTH = 5
SPACE_BETWEEN_RECTANGLES = 1
```

Run the program in a window with width of 1920 pixels and height of 1080 pixels, 
fit the screen with rectangles with width of 50 pixels, and add a space between
rectangles of 3 pixels.

```bash
./algo_animator 1920 1080 50 3
```

![Image 3](repo/3.png)


## Examples

### Normal mode

Just run the program and press `ENTER`  
[Video.webm](https://github.com/denniscmartin/algo-animator/assets/66180929/f75366b4-b8ba-421e-8c1e-f43727a67b4f)

### Slow motion

Use `u` to increase the delay, then `ENTER` to run the algorithm   
*The delay is applied after each iteration*  
[Video.webm](https://github.com/denniscmartin/algo-animator/assets/66180929/b3bcde87-2680-4667-98be-3b3946712254)

### Pause mode

Press `ENTER` then `p`   
[Video.webm](https://github.com/denniscmartin/algo-animator/assets/66180929/6a3ff80c-56dc-4b1a-a811-e3649fe2edde)

### Sequential mode

Press `q` then press `ENTER` to visualize the algorithm step by step.   
[Video.webm](https://github.com/denniscmartin/algo-animator/assets/66180929/743c00d8-5236-437d-85ad-b139611175ef)

## Notes

This project has not been designed to compare the speed of the algorithms side by side. The main objective is the visualization of the algorithms for educational purposes.
