# k-d_tree
k-d tree static/dynamic library
 - builds a k-d tree from a list of points.
 - searches the tree to find the node closest to a given (x, y) location.

Detailed Description
  This library finds the node nearest a given point (a random (x,y) point not belonging to the tree), by traversing the tree and calculating the distance from each node visited to the input point.  It keeps track of the bounding box of the region defined by each node in the tree and avoids visiting nodes that we already know are farther away than the current minimum distance.

To start, type "make" in the terminal, giving an executable. 
To test, Run ->

./basic your_file.csv x y



To generate a random dataset of "n_points" number of "x,y" coordinates, run ->

python3 generate_random_points.py output_file.csv n_points



For additional make recipes -
Type the following commands for more builds

		1. For creating shared library. Run -> make libshared
        Builds libkdtree as a shared library
  
		2. For creating static library. Run -> make libstatic
        Builds libkdtree as a static library
  
		3. For using shared library.    Run -> make shared
        Creates executable from shared library.
        To test, Run -> ./shared your_file.csv x y
        
		4. For using static library.    Run -> make static
        Creates executable from static library.
        To test, Run -> ./static your_file.csv x y

        5. To clean the directory. Run -> make clean


Files Description
+ **kdtree.h**: Header file defining the libtree API
+ **kdtree.c**: Implementation for APIs
+ **kd_internals.h**: Header file for the internal functions to implement in kdtree.c
+ **output_file.csv**: set of points input files to test with
+ **generate_random_points.py**: Helper script to generate sets of test point input files
+ **TreeTester**: Executable that will test your libtree.so alongside the solution and print PASS or FAIL
    + The TreeTester executable was compiled on Ubuntu 18.04 with gcc; any Ubuntu / Debian Linux derivative **should** be able to use it just fine.
