A proposed interactive and simple method that creates model variations by applying stretching on 3D models. The method replicates the geometric details and synthesizes extensions by applying texture synthesis techniques on surface details.

This work is done as part of my M.Sc. thesis at the [GrUVi lab](http://gruvi.cs.sfu.ca/) at the [Simon Fraser University](http://www.sfu.ca/), Canada.

![http://www.cs.sfu.ca/~iaa7/personal/stretch_idea.png](http://www.cs.sfu.ca/~iaa7/personal/stretch_idea.png)

## Tools needed ##
  * C++ and OpenGL
  * [Qt 4.7](http://qt.nokia.com/)
  * [libQGLViewer (2.3.9)](http://www.libqglviewer.com/)
  * [GLee](http://elf-stone.com/glee.php) or [GLEW](http://glew.sourceforge.net/)

## Some techniques & ideas implemented ##
  * [Implicit mesh fairing using curvature flow](http://portal.acm.org/citation.cfm?id=311535.311576)
  * Patch-based [texture synthesis](http://en.wikipedia.org/wiki/Texture_synthesis)
  * [Octree](http://en.wikipedia.org/wiki/Octree) for [ray tracing](http://en.wikipedia.org/wiki/Ray_tracing_(graphics))
  * [Bézier spline](http://en.wikipedia.org/wiki/B%C3%A9zier_spline) class
  * Undirected [graph class](http://en.wikipedia.org/wiki/Graph_(data_structure)) (with [Dijkstra's](http://en.wikipedia.org/wiki/Dijkstra's_algorithm) algorithm)

## Installation & usage ##
  * Developed on Windows, Linux port is experimental
  * Compile using MSVC 2008 with Qt add-in (qmake / Qt creator on Linux). Before that, compile the solver in the "Solver" directory using MSVC (make on Linux).
  * Example models are provided. Open a mesh and click 'SHIFT' at two different parts to select a region of interest. Then press 'ALT' and draw a curve to extend that part.

## Real-Time Editing Sessions ##
<a href='http://www.youtube.com/watch?feature=player_embedded&v=VAXm7Wm-R7c' target='_blank'><img src='http://img.youtube.com/vi/VAXm7Wm-R7c/0.jpg' width='425' height=344 /></a>

## Publication ##

Ibraheem Alhashim, Hao Zhang, and Ligang Liu, ["Detail-Replicating Shape Stretching,"](http://www.springerlink.com/content/e1812qvx45121783/) the Visual Computer, to appear, 2012.

**BibTeX**
```
@article {springerlink:10.1007/s00371-011-0665-9,
   author = {Alhashim, Ibraheem and Zhang, Hao and Liu, Ligang},
   affiliation = {Simon Fraser University, Burnaby, Canada},
   title = {Detail-replicating shape stretching},
   journal = {The Visual Computer},
   publisher = {Springer Berlin / Heidelberg},
   issn = {0178-2789},
   keyword = {Computer Science},
   pages = {1-14},
   url = {http://dx.doi.org/10.1007/s00371-011-0665-9},
   note = {10.1007/s00371-011-0665-9},
}
```