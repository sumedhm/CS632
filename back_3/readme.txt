node.c node1.c node2.c are 3 nodes. We can add more nodes by creatin more files.

Edit value of MAX, this will be number of nodes(3 by default).
Edit value of ID, make sure to set ID for all nodes different.
The node with min ID value will be elected a leader since this is an implementation of ALL THE WAY algorithm(variant).
In array neighbor[], enter immediate neighbor, then next to immediate neighbor and so on.

To run compile the c files, and execute them on different terminals.
Enter 1 to start election in any one of the nodes, and election will be done, and all nodes will print final leader.
