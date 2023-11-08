#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>
#define INT_MAX 100000
using namespace std;

typedef pair<int, int> Edge; //(neighbor, cost)

void backtrack(vector<int> &parent, int source, int destination, vector<int> &shortestPath) // Backtrack to find the shortest path
{
    stack<int> s;
    int i = destination;
    while (i != source)
    {
        s.push(i); // Push the nodes in the stack
        i = parent[i];
    }
    s.push(source);
    while (!s.empty()) // Pop the nodes from the stack
    {
        // cout << s.top() << " ";
        shortestPath.push_back(s.top()); // Push the nodes in the vector that will be returned
        s.pop();
    }
}

int uniformCostSearch(vector<vector<Edge> > &graph, int source, int destination, vector<int> &shortestPath)
{
    priority_queue<Edge, vector<Edge>, greater<Edge> > m_heap; // Min-heap
    vector<int> distance(graph.size(), INT_MAX);              // Initialize all the distances to infinity                                     
    distance[source] = 0; // Distance of source node from itself is 0
    vector<int> parent(graph.size(), -1); // Push the source node in the heap
    m_heap.push(make_pair(0, source));// Push the source node in the heap
    while (!m_heap.empty()) // While the heap is not empty
    {
        int current_node = m_heap.top().second;
        int cost = m_heap.top().first;
        m_heap.pop(); // Pop the node with the minimum cost

        if (current_node == destination) // If the destination node is reached
        {
            backtrack(parent, source, destination, shortestPath); // Backtrack to find the shortest path
            return cost;
        }

        if (cost <= distance[current_node]) // If the cost is less than the distance of the current node
        {
            for (int i = 0; i < graph[current_node].size(); i++) // For all the neighbors of the current node
            { 
                int adj_node = graph[current_node][i].first;
                int new_cost = cost + graph[current_node][i].second;
                if (new_cost < distance[adj_node]) // If the new cost is less than the distance of the neighbor+current node cost
                {
                    distance[adj_node] = new_cost;
                    m_heap.push(make_pair(new_cost, adj_node)); //update and push the neighbor node in the heap with the new cost
                    parent[adj_node] = current_node;
                }
            }
        }
    }

    return -1; // If the destination node is not reached
}

int main()
{
    int no_vertices, no_edges;
    cin >> no_vertices >> no_edges; // Input the number of vertices and edges
    int source, destination;
    cin >> source >> destination; // Input the source and destination nodes

    vector<vector<Edge> > graph(no_vertices + 1); 
    vector<int> shortestPath;

    for (int i = 0; i < no_edges; i++)
    {
        int src, dest, wt;
        cin >> src >> dest >> wt;
        graph[src].push_back(make_pair(dest, wt)); // Input the edges and their weights
    }

    int min_cost = uniformCostSearch(graph, source, destination, shortestPath); // Call the uniform cost search function

    if (min_cost != -1)
    {
        cout << min_cost << endl;
        cout << shortestPath[0];
        int k = 1;
        int j = shortestPath.size() - 1;
        while (k <= j)
            cout << "->" << shortestPath[k++]; // Print the shortest path
    }

    else
        cout << "No path found from source to destination\n";

    return 0;
}
