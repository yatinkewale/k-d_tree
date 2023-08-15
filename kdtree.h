#ifndef KDTREE_H
#define KDTREE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DEFAULT_ID  0               /* ID of the query point.                 */
#define DIMENSION   2               /* k number of dimensions in k-d tree.    */

#define AXIS_X      0               /* x - plane.                             */
#define AXIS_Y      1               /* y - plane.                             */


uint32_t number_of_nodes_visited;   /* variable to keep track of nodes visited\
                                       while searching for the nearest point\
                                       to the query point. */

typedef struct {
    float x;
    float y;
    uint32_t id;                    /* Index of point from input file.*/
} Point;

typedef struct {
    Point loc;
    void *left;
    void *right;
} Node;

/* @brief build a 2D K-D Tree from a list of points
 *
 * The nodes/points will be ID'd based on their position within the input array,
 * in the range [0, n_points - 1]
 *
 * @param points - The array of (x, y) points (AoS data format)
 * @param n_points - The number of (x, y) point pairs in 'points'
 * @return Pointer to the root node of the tree
 */
Node* kd_build(const Point *points, uint32_t n_points);

/* @brief Find the nearest tree node to the given 2D point
 *
 * @param kdtree - Root of the k-d tree to search
 * @param loc - Search point
 * @param nearest - Upon return, points to nearest node in tree
 * @return Distance from 'loc' to nearest node
 */
float kd_find_nearest(Node *kdtree, const Point *loc, Node **nearest);


/* @brief Reads csv file and builds array of 'Point' points
 *
 * @param file_name - Name of the file to be read for data set
 * @param number_of_points - total n-D points
 * @return array of 'Point' points
 */
Point* driver(char *file_name, uint32_t *number_of_points);

#endif /* KDTREE_H */
