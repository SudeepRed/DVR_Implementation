#include "node.h"

#include <iostream>
#include <iterator>
#include <map>
vector<RoutingNode *> distanceVectorNodes;

void routingAlgo(vector<RoutingNode *> distanceVectorNodes);
const int inf = 100000007;
map<RoutingNode *, vector<RoutingNode *>> mp;

void addEdge(RoutingNode *u, RoutingNode *v)
{

	mp[u].push_back(v);
	//mp[v].push_back(u);
	//cout<<mp[distanceVectorNodes[i]][0]->getName()<<endl;
}
int main()
{
	int n; // number of nodes
	cin >> n;
	//adjacency List of nodes

	string name; //Node label
	distanceVectorNodes.clear();
	for (int i = 0; i < n; i++)
	{
		RoutingNode *newnode = new RoutingNode();
		cin >> name;
		newnode->setName(name);
		distanceVectorNodes.push_back(newnode);
		vector<RoutingNode *> temp;
		mp[newnode] = (temp);
	}
	cin >> name;
	/*
      For each node label(@name), it's own ip address, ip address of another node
      defined by @oname will be inserted in the node's own datastructure interfaces 
    */
	while (name != "EOE")
	{ //End of entries
		for (int i = 0; i < distanceVectorNodes.size(); i++)
		{
			string myeth, oeth, oname;
			if (distanceVectorNodes[i]->getName() == name)
			{
				//node interface ip
				cin >> myeth;
				//ip of another node connected to myeth (nd[i])
				cin >> oeth;
				//label of the node whose ip is oeth
				cin >> oname;

				for (int j = 0; j < distanceVectorNodes.size(); j++)
				{
					if (distanceVectorNodes[j]->getName() == oname)
					{
						/*
		  
		@myeth: ip address of my (distanceVectorNodes[i]) end of connection.
		@oeth: ip address of other end of connection.
		@distanceVectorNodes[j]: pointer to the node whose one of the interface is @oeth
	      */
						distanceVectorNodes[i]->addInterface(myeth, oeth, distanceVectorNodes[j]);
						//Routing table initialization
						/*
		@myeth: ip address of my (distanceVectorNodes[i]) ethernet interface.
		@0: hop count, 0 as node does not need any other hop to pass packet to itself.
		
	      */
						distanceVectorNodes[i]->addTblEntry(myeth, 0);
						addEdge(distanceVectorNodes[i], distanceVectorNodes[j]);
						break;
					}
				}
			}
		}
		cin >> name;
	}

	/* The logic of the routing algorithm should go here */
	routingAlgo(distanceVectorNodes);
}
