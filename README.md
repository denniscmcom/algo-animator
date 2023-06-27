# algo-animator

This project is inspired by - off course - the video by Timo Bingmann called [15 sorting algorithms in 6 minutes](https://www.youtube.com/watch?v=kPRA0W1kECg).

![Image 1](repo/1.png)
![Image 2](repo/2.png

## Compile

```bash
gcc main.c -lglut -lGL -lGLU -lm -I/usr/local/include/freetype2 -L/usr/local/lib -lfreetype
```

## To improve

Since `GLUT` is single-thread, I cannot call `glutPostRedisplay()` within `while` or `for loops` to redraw the screen in every 
step of the selected sorting algorithm. I guess the solution is multi-threading, so I can perform the sorting in the second thread
and post notifications to the main thread to redraw the screen.   

Since this is my first OpenGL project and just my second program in C, I am tired and I don't want to spend more time in this project, 
but maybe in the future I'll do it. Right now there are just two algorithms supported. I've implemented them in a way that every function
call is a single step of the sorting. So, storing the "state" outside the function, I can use `idle()` as the loop of the sorting algorithm.   

Let's say we selected `bubble sort`. Once you press `ENTER`, the process will be like this:

```c
run = true;
bubble_sort();
glutPostRedisplay;
bubble_sort();
glutPostRedisplay;
bubble_sort();
glutPostRedisplay;

// Continue until the array is sorted
run = false;
```
