#include "kdtree.h"

#define NUMBER_OF_COMMAND_LINE_ARGUMENTS 4

int main(int argc, char **argv)
{
    if (NUMBER_OF_COMMAND_LINE_ARGUMENTS != argc)
    {
        printf("Error: Incorrect Arguments\nUsage\n");
        printf("./executable your_data_file.csv x_coordinate y_coordinate\n");
        exit(0);
    }
    char *file_to_read = argv[1];
    Point point_to_test = {atof(argv[2]), atof(argv[3]), DEFAULT_ID};/*Query  */
    uint32_t num_points = 0;
    Point *array_of_points = NULL;
    array_of_points = driver (file_to_read, &num_points);   /*Initialize Array*/
    Node *kd_tree = NULL;
    kd_tree = kd_build (array_of_points, num_points);       /*Build Tree      */
    free (array_of_points);
    array_of_points = NULL;
    float minimum_distance = 0.0;
    minimum_distance = kd_find_nearest (kd_tree, &point_to_test, &kd_tree);\
                                                            /*Get Nearest Node*/
    if (-1 != minimum_distance)
    {
        printf ("Its distance to (%f, %f) is %f\n", point_to_test.x,\
            point_to_test.y, minimum_distance);
    }
    else
    {
        printf("Fix above errors\n");
    }
    return 0;
}
