1. How to run the program
-> run the ./run.sh script

2. Whats being done in my code ?
-> from the main function , createInitialGraph() is called which creates the initial graph
-> then the graph is passed to the function  ChooseLandmarks() which chooses the landmarks
-> 50 landmarks are choose based on highest degree and 50 are chosen randomly
-> partition the graph into these 100 clusters randomly and writing them into a file.

->5 threads are created for graphUpdateThread() which updates the graph by adding or removing edges 
based on a random probability
-> these edges add/remove are written into a file

->20 threads are created for pathFinderThread() which finds the shortest path.
->shortest path between 2 landmarks is found using djiakstra's algorithm
->the shortest path is being displayed on the terminal.
-> the same djiakstra will be repeated for the nodes (produced randomly)
(Note this above part for pathFinderThread() are commented out in mainfunction
in line 655-668) 