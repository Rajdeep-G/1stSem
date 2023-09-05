#include <stdio.h>
#include <stdlib.h>

struct sec
{
    int x;
    int y;
    int rad;
};
int visited[1001][1001];

int is_within_grid(int x, int y, int n, int m) // check if the point is within the grid
{
    return (x >= 0 && x <= n && y >= 0 && y <= m);
}

int is_within_rad_of_security(int pos_x, int pos_y, int sec_x, int sec_y, int sec_radius) // check if the point is within the radius of the security zone
{
    int distance_squared = (pos_x - sec_x) * (pos_x - sec_x) + (pos_y - sec_y) * (pos_y - sec_y);
    return distance_squared <= sec_radius * sec_radius;
}

int is_safe_from_security(int x, int y, int n, int m, struct sec *security_zones, int no_of_sec) // check if the point is safe from the security zone
{
    for (int i = 0; i < no_of_sec; i++) // for all security zones
    {
        if (is_within_grid(x, y, n, m) && is_within_rad_of_security(x, y, security_zones[i].x, security_zones[i].y, security_zones[i].rad))

            return 0;
    }

    // printf("\nposs-%d %d", x, y);
    return 1;
}

int traversal_all(int x, int y, int n, int m, struct sec *security_zones, int no_of_sec)
{

    if (!is_within_grid(x, y, n, m) || (visited[x][y]) || !is_safe_from_security(x, y, n, m, security_zones, no_of_sec)) // base cond
        return 0;

    visited[x][y] = 1;

    if (x == n) // if reached the other end
        return 1;

    int dx[] = {1, 0, 0, -1, 1, 1, -1, -1};
    int dy[] = {0, 1, -1, 0, 1, -1, 1, -1}; // all 8 points to be checked recursively

    for (int i = 0; i < 8; i++)
    {
        if (traversal_all(x + dx[i], y + dy[i], n, m, security_zones, no_of_sec)) // checking all 8 points
            return 1;
    }

    return 0;
}

int path_exists(int n, int m, int a, int b, int no_of_sec, struct sec *security_zones)
{

    for (int y = a; y <= b; y++) // any starting position in the y axis
    {
        if (traversal_all(0, y, n, m, security_zones, no_of_sec))
            return 1; // Return 1 if a valid path is found for any starting position
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("<./a.out><inputfile.txt\n");
        exit(1);
    }

    char filename[100];
    sscanf(argv[1], "%s", filename);

    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    int row, col, no_sec;
    fscanf(fptr, "%d %d %d", &row, &col, &no_sec);

    int grid[row][col];

    struct sec *sec_zones = (struct sec *)malloc(no_sec * sizeof(struct sec));

    for (int i = 0; i < no_sec; i++)
    {
        fscanf(fptr, "%d %d %d", &sec_zones[i].x, &sec_zones[i].y, &sec_zones[i].rad);
    }

    // for (int i = 0; i < no_sec; i++)
    // {
    //     printf("%d %d %d\n", sec_det[i].x, sec_det[i].y, sec_det[i].rad);
    // }
    int a = 0;   // Starting y-coordinate
    int b = col; // Ending y-coordinate

    if (path_exists(row, col, a, b, no_sec, sec_zones))
        printf("possible\n");
    else
        printf("impossible\n");

    free(sec_zones);
    fclose(fptr);

    return 0;
}
