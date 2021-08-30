<h1 align="center">Gear</h1>
<h3 align="center">Spur Gear created in OpenGL with C++ and C for linux systems.</h3>
<p align="center"> 
  <img src="https://img.shields.io/badge/OpenGL-blue"/>
  <img src="https://img.shields.io/badge/FreeGLUT-blue"/>
  <img src="https://img.shields.io/badge/gcc g++-blue"/>
</p>
<br/>

## Objective
<p align="justify"> 
  <a>The objective of this project is to create a simple (and very poorly optimized) system to understand how the base of opengl works with c++ and c for 3D environment.</a>
</p>

## Setup
<p align="justify"> 
 <a>To setup your system just use git clone command, all the files are included in this repository. To do it, use:</a>
</p>

```shell
git clone https://github.com/victorkich/CarRacer/
```

<a>All of requirements is show in the badgets above, but if you want to install all of them, execute the following line of code:</a>
</p>

```shell
sudo apt-get install build-essential mesa-utils freeglut3-dev
```

<p align="justify"> 
 <a>For test in your own computer, just compile using the following code:</a>
</p>

```shell
g++ main.cpp -lGL -lGLU -lglut -pthread -lz -lrt -lm -o client
g++ server.cpp -pthread -o server
```

<p align="justify"> 
 <a>And run it using the following code:</a>
</p>

```shell
./server $PORT$
./client localhost $PORT$
```

## Example

<p align="center"> 
  <img src="media/gear.gif" alt="Gear"/>
</p>  

<p align="center"> 
  <i>If you liked this repository, please don't forget to starred it!</i>
  <img src="https://img.shields.io/github/stars/victorkich/Gear?style=social"/>
</p>
