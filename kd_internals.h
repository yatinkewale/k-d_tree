#ifndef KD_INTERNALS_H
#define KD_INTERNALS_H

#include "kdtree.h"

/* @brief Compare two nodes based on their x values
 * @return (-1, 0, 1) if a.x (<, =, >) b.x
 */
static int compare_x(const void *a, const void *b)
{
    const float ax = ((Node *)a)->loc.x;
    const float bx = ((Node *)b)->loc.x;
    return (ax > bx) ? 1 : ((ax < bx) ? -1: 0);
}

/* @brief Compare two nodes based on their y values
 * @return (-1, 0, 1) if a.y (<, =, >) b.y
 */
static int compare_y(const void *a, const void *b)
{
    const float ay = ((Node *)a)->loc.y;
    const float by = ((Node *)b)->loc.y;
    return (ay > by) ? 1 : ((ay < by) ? -1: 0);
}

/* @brief Calculates Euclidean distance between a point and a node
 *
 * @return float Euclidean distance
 */
float distance(const void *a, const void *b);

/* @brief Destroys the tree in post-order traversal
 *
 * @param tree - The pointer to first Node in the tree.
 */
void free_tree(Node *tree);

/* @brief Prints kd-tree Node ID's with Post-order traversal
 *
 * @param tree - The pointer to a Node in the tree.
 */
void print_tree_post_order(Node* tree);

/* @brief Check distance between point and bounded box to prune search of far points
 *
 * @param node - Pointer to a Node in the tree
 * @param loc - The query point
 * @param min_dist - Current Minimum Distance
 * @param depth - Current depth of the tree
 * @return - (0, 1) if point is (near, far) from bounding box
 */
uint8_t is_far_from_bounded_box(Node *node, const Point *loc, float min_dist,\
                                uint8_t depth);

/* @brief Sort a list of nodes in-place along chosen axis
 *
 * @param list - The list of nodes. The order of the nodes in the list may change
 * @param n_points - The length of 'list'
 * @param axis - The dimension (0: x, 1: y) to sort along
 */
void sort_points(Node *list, uint32_t n_points, uint8_t axis);

/* @brief Finds the median point in a list of nodes/points
 *
 * Upon return, all points in the list before the median index will be to the 'left' of
 * the median point (along the given axis), while all following points are to the 'right'
 *
 * @param list - The list of points. The order of the points in the list may change
 * @param n_points - The length of 'list'
 * @param axis - The dimension (0: x, 1: y) to find the median of
 * @return Index of the median point in 'list'
 */
static uint32_t find_median(Node *list, uint32_t n_points, uint8_t axis)
{
    sort_points(list, n_points, axis);
    return (n_points / 2);
}

/* @brief Recursive K-D Tree constructor
 *
 * At each level ('depth') the sub-tree is constructed for the nodes in range [min, max)
 *
 * @param list - The entire list of nodes to build a tree for
 * @param min - The first index of 'list' for which we are currently operating on
 * @param max - The index of 'list' beyond the current sub-tree (1-index of last node)
 * @param depth - Current tree depth (root == 0)
 * @return Pointer to the root node
 */
Node * kd_build_recurse(Node *list, int32_t min, int32_t max, uint8_t depth);

/* @brief Recursive K-D Tree search for nearest node
 *
 * At each node, this function shall return the current minimum distance, and set the 'best' node
 * to the current node if the current distance is smaller than the prior minimum distance
 *
 * @param tree - The current node of the tree being searched
 * @param loc - The point for which we are finding the nearest node
 * @param min_dist - The current minimum distance found
 * @param best - Pointer to the current nearest node
 * @param depth - Depth of the current tree node
 * @return The new best minimum distance
 */
float kd_nearest_recurse(Node *tree, const Point *loc, float min_dist, Node **best, uint8_t depth);

#endif /* KD_INTERNALS_H */
