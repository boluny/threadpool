# threadpool
A simple thread pool implementation using C++ 14 features.

## Table of Contents

- [Tutorial](#tutorial)
- [Portability](#portability)
- [TODOs](#todos)
- [License](#license)

## Tutorial
Include both "block_queue.h" and "threadpool.h" in your project is enough.

You can refer to [test.cpp](../master/test.cpp) for samples. 

if you want to build the repo:
```bash
$ git clone https://github.com/boluny/threadpool.git [your_dir]

$ cd [your_dir]

# you can choose to build in source tree alternatively
$ mkdir build && cd build

$ cmake ..

$ make
```


## Portability
It should work on Windows, Linux, Mac OS and etc as it utilize CMake and codes from only STL.

However, there is some configuration work for CMake when I have to make the build work.



## TODOs 
- Add lock free queue to test performance between memory order and mutex.
- More configuration part like queue wait time, number of threads in pool.
- More tests. 


## License
GPL3 
