# cad
* simple programmatic 3D solid geometry
* representations for points, polylines, polygons, and meshes
* simple IR for describing operations and is good destination for programming languages

# needs libs
* geode
* gui
* portaudio, portmidi, opencv

# files
* cad.*: cad operations written directly against geode
* geom.*: wrapper around geode reps to be dynamic allow polymorphic construction
* read-eval.*: IR interpreter that read strings into and evals geoms
* app.*: gui front end and viewer

# examples:

* 1D

```cad "[[-5,5],[5,5],[5,-5]]"```

```cad "[[[-5,5,-5],[-5,-5,0],[5,-5,0],[5,5,0]]]"```

```cad "[[[-5,-5],[5,-5]],[[5,5],[-5,5]]]"```

```cad 'letter("A")'```

```cad 'text("0123456789")'```

* 2D


```cad "square(2)"```

```cad "circle(2)"```

```cad "square(2) \ square(1)"```

```cad "poly([-2,-2],[-1,0],[-2,2],[0,1],[2,2],[1,0],[2,-2],[0,-1])"```

```cad "poly([[-2,-2],[-1,0],[-2,2],[0,1],[2,2],[1,0],[2,-2],[0,-1]])"```

```cad "hull(square(5) | circle(6))"```

* 3D

```cad "sphere(2.6)"```

```cad "cube(2)"```

```cad "xrot(15,cube(2))"```

```cad "(xrot(15,cube(2)) | yrot(15,cube(2)) | zrot(15,cube(2)) | cube(2)) \ sphere(2.6)"```

```cad "((sphere(8) | extrude(20,circle(2))) \ sphere(7)) \ extrude(21,circle(1))"```

```cad "hull(revolve(xmov(4, circle(4))) | extrude(20, square(2)))"```

```cad "offset(3, cube(1))"```

```cad "hollow(1, cube(4))"```

* 1D -> 2D

```cad 'mag1(4,thicken(0.1,letter("A")))'```

```cad 'mag1(4,offset(0.1,letter("A")))'```

* 1D -> 3D

```cad "thicken(1,[[-5,5,-5],[-5,-5,0],[5,-5,0],[5,5,0]])"```

* 2D -> 3D

```cad "extrude(16,square(2) \ square(1))"```

```cad 'extrude(2,mag1(4,thicken(0.1,letter("A"))))'```

```cad 'extrude(2,mag1(4,offset(0.1,letter("A"))))'```

```cad 'extrude(2,mag1(2,thicken(0.05,text("0123456789"))))'```

```cad 'extrude(2,mag1(2,offset(0.05,text("0123456789"))))'```

```cad "revolve(xmov(6,poly([-2,-2],[-1,0],[-2,2],[0,1],[2,2],[1,0],[2,-2],[0,-1])))"```

```cad "revolve(xmov(4,poly([-1,-1],[1,-1],[0,1])))"```

```cad "revolve(xmov(8,circle(4)))"```

```cad "revolve(xmov(4,square(2) \ square(1)))"```

* 3D -> 2D

```cad 'slice(0,sphere(4) \ cube(2))'```

```cad "slice(0,revolve(xmov(4,square(2) \ square(1))))"```


# TODO

* auto specification of resolution for spheres, offset etc
* SVG reading/writing
* colors
* 3D mesh shadow instead of slice
* 3D->3D revolve/extrude
