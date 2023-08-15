#include "kdtree.h"
#include "kd_internals.h"

/* -------- External Function Definitions -------- */
Point* driver(char *file_name, uint32_t *number_of_points)
{
    FILE *fp;
    fp = fopen (file_name, "r");
    if (fp == NULL)
    {
        printf ("Error: Error opening file %s\n", file_name);
        return NULL;
    }
    fscanf (fp, "%d", number_of_points);
    if (0 == (*number_of_points))
    {
        printf ("Error: No points to build in %s\n", file_name);
        return NULL;
    }
    printf ("Reading %d points\n", *number_of_points);
    Point *array = (Point *)malloc (sizeof (Point) * (*number_of_points));
    if (NULL == array)
    {
        printf ("Error: Memory allocation for the points array failed\n");
        return NULL;
    }
    uint32_t temp_index = 0;
    uint8_t points_per_row = 0;
    while (temp_index < (*number_of_points))    /* Reading csv rows per Point.*/
    {
        points_per_row = fscanf (fp, "%f,%f", &(array[temp_index].x),\
                                &(array[temp_index].y));
        if (DIMENSION != points_per_row)
        {
            printf("Error: Incorrect data, check .csv file");
            free (array);
            array = NULL;
            break;
        }
        array[temp_index].id = temp_index;
        temp_index = temp_index + 1;
    }
    fclose (fp);
    return array;
}

Node *kd_build(const Point *points, uint32_t n_points)
{
    if (NULL == points)
    {
        return NULL;
    }
    Node *list_of_nodes = (Node *)malloc (sizeof (Node) * n_points);
    if (NULL == list_of_nodes)
    {
        printf ("Error: Memory allocation for the tree building failed\n");
        return NULL;
    }
    for (int i = 0; i < n_points; i++)          /* Constructing array of Nodes*/
    {
        list_of_nodes[i].loc = points[i];
        list_of_nodes[i].left = NULL;
        list_of_nodes[i].right = NULL;
    }
    uint32_t first_index_of_list = 0;
    uint32_t last_index_of_list = n_points - 1;
    uint8_t current_depth = 0;
    Node *kd_tree = NULL;
    kd_tree = kd_build_recurse (list_of_nodes, first_index_of_list,\
                               last_index_of_list,\
                               current_depth);  /* Call internal build recurse*/
    free (list_of_nodes);
    list_of_nodes = NULL;
    return kd_tree;
}

float kd_find_nearest(Node *kdtree, const Point *loc, Node **nearest)
{
    if (NULL == kdtree)
    {
        return -1;
    }
    float min_dist = UINT32_MAX;
    uint8_t depth = 0;
    min_dist = kd_nearest_recurse (kdtree, loc, min_dist, nearest,\
                                   depth);      /* Call internal find function*/
    printf ("By visiting [%d] nodes, found ", number_of_nodes_visited);
    printf ("nearest Node, (id = %d, x = %f, y = %f)\n",\
           (*nearest) -> loc.id, (*nearest) -> loc.x, (*nearest) -> loc.y);
    free_tree (kdtree);
    kdtree = NULL;
    return min_dist;
}

/* -------- Internal Function Definitions -------- */

void sort_points(Node *list, uint32_t n_points, uint8_t axis)
{
    if (AXIS_X == axis)                         /* Sort in x - plane.         */
    {
        qsort ((void *)list, n_points, sizeof (Node), compare_x);
    }
    else if (AXIS_Y == axis)                    /* Sort in y - plane.         */
    {
        qsort ((void *)list, n_points, sizeof (Node), compare_y);
    }
}

Node *kd_build_recurse(Node *list, int32_t min, int32_t max, uint8_t depth)
{
    if (min > max)                              /* Breaking condition.        */
    {
        return NULL;
    }
    uint8_t axis = depth % DIMENSION;
    uint32_t median = 0;
    median = find_median (&list[min], max - min + 1, axis);
    median = median + min;                      /* Offset to get correct index*/
    Node *tree = (Node *) malloc (sizeof (Node));
    if(NULL == tree)
    {
        printf("Error: Memory Allocation for recursive tree build failed\n");
        return NULL;
    }
    tree->loc = list[median].loc;
    tree->left = kd_build_recurse (list, min, median - 1,\
                                   depth+1);    /* Build left sub-tree.       */
    tree->right = kd_build_recurse (list, median + 1, max,\
                                    depth+1);   /* Build right sub-tree.      */
    return tree;
}

float kd_nearest_recurse(Node *tree, const Point *loc, float min_dist,\
                         Node **best, uint8_t depth)
{
    if (NULL == tree)                           /* Break condition.           */
    {
        return min_dist;
    }
    number_of_nodes_visited++;
    uint8_t axis = depth % DIMENSION;
    int8_t traverse_left_first = 0;
    float current_min_dist = distance (loc, tree);
    if (current_min_dist < min_dist)            /* Update min_dist if greater.*/
    {
        min_dist = current_min_dist;
        *best = tree;
    }
    if (AXIS_X == axis)                         /* Decide traversal sub-tree. */
    {
        Node temp = {*loc, NULL, NULL};
        traverse_left_first = compare_x (tree, &temp);
    }
    else if (AXIS_Y == axis)
    {
        Node temp = {*loc, NULL, NULL};
        traverse_left_first = compare_y (tree, &temp);
    }
    if (1 == traverse_left_first)               /* Travers left, then right.  */
    {                                           /* If new near point possible*/
        min_dist = kd_nearest_recurse (tree -> left, loc, min_dist, best,\
                                      depth+1);
        if (0 == is_far_from_bounded_box (tree, loc, min_dist, depth))
        {
            min_dist = kd_nearest_recurse (tree -> right, loc, min_dist, best,\
                                          depth+1);
        }
    }
    else                                        /* Travers right, then left.  */
    {                                           /* If new near point possible*/
        min_dist = kd_nearest_recurse (tree -> right, loc, min_dist, best,\
                                      depth+1);
        if (0 == is_far_from_bounded_box (tree, loc, min_dist, depth))
        {
            min_dist = kd_nearest_recurse (tree -> left, loc, min_dist, best,\
                                          depth+1);
        }
    }
    return min_dist;
}

void free_tree(Node *tree)
{
    if (NULL == tree)
    {
        return;
    }
    free_tree (tree -> left);
    free_tree (tree -> right);
    free (tree);
}

uint8_t is_far_from_bounded_box(Node *node, const Point *loc, float min_dist,\
                                uint8_t depth)
{
    uint8_t axis = depth % DIMENSION;
    uint8_t is_far = 0;
    if (AXIS_X == axis)
    {
        is_far = (abs((loc -> x) - (node -> loc.x))) > min_dist;
    }
    else if (AXIS_Y == axis)
    {
        is_far = (abs((loc -> y) - (node -> loc.y))) > min_dist;
    }
    return is_far;
}

void print_tree_post_order(Node* tree)
{
    if (tree == NULL)
    {
        return;
    }
    print_tree_post_order (tree -> left);
    print_tree_post_order (tree -> right);
    printf ("%-2d -> (%-4f, %-4f)\n" , tree -> loc.id, tree -> loc.x,\
            tree -> loc.y);
}

float distance(const void *a, const void *b)
{
    const float ax = ((Point *)a) -> x;
    const float ay = ((Point *)a) -> y;
    const float bx = ((Node *)b) -> loc.x;
    const float by = ((Node *)b) -> loc.y;
    float dist = pow ((bx - ax), 2) + pow ((by - ay), 2);
    return sqrt (dist);
}
