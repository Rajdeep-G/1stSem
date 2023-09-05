import networkx as nx
import matplotlib.pyplot as plt
G = nx.DiGraph()

G.add_nodes_from(i for i in range(1,7))

# print(G.nodes())

G.add_edge(1,2,weight=30)
G.add_edge(2,4,weight=30)
G.add_edge(3,5,weight=30)
G.add_edge(5,6,weight=30)

G.add_edge(1,3,weight=40)
G.add_edge(2,5,weight=40)
G.add_edge(4,6,weight=40)

G.add_edge(1,4,weight=50)

# print(G.edges())


edge_wt={(u,v):d['weight'] for u,v,d in G.edges(data=True)}
pos=nx.spring_layout(G)

nx.draw(G,pos,with_labels=True,node_size=1000)
nx.draw_networkx_edge_labels(G,pos,edge_labels=edge_wt)

plt.show()

#print the cost of each path
for path in nx.all_simple_paths(G,source=1,target=6):
    wt=0
    for i in range(len(path)-1):
        wt+=G[path[i]][path[i+1]]['weight']
    print("paths-",path)
    print("cost-",wt)


print("\nShortest path")
print(nx.shortest_path(G,source=1,target=6,method='dijkstra',weight='weight'))
print("\nShortest path cost")
print(nx.shortest_path_length(G,source=1,target=6,method='dijkstra',weight='weight'))
