#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*
  Each row in the table will have these fields
  dstip:	Destination IP address
  nexthop: 	Next hop on the path to reach dstip
  ip_interface: nexthop is reachable via this interface (a node can have multiple interfaces)
  cost: 	cost of reaching dstip (number of hops)
*/
class RoutingEntry
{
public:
  string dstip, nexthop;
  string ip_interface;
  int cost;
  int share_cost = 16;
  // 'share_cost' holds the cost that has to be shared with the neighbor.
};

// sorting routing table
class Comparator
{
public:
  bool operator()(const RoutingEntry &R1, const RoutingEntry &R2)
  {
    if (R1.cost == R2.cost)
    {
      return R1.dstip.compare(R2.dstip) < 0;
    }
    else if (R1.cost > R2.cost)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
};


struct routingtbl
{
  vector<RoutingEntry> tbl;
};

/*
  Message format to be sent by a sender
  from: 		Sender's ip
  mytbl: 		Senders routing table
  recvip:		Receiver's ip
*/
class RouteMsg
{
public:
  string from;              // I am sending this message, so it must be me i.e. if A is sending mesg to B then it is A's ip address
  struct routingtbl *mytbl; // This is routing table of A
  string recvip;            // B ip address that will receive this message
};

/*
  Emulation of network interface.   
  ip: 		Own ip
  connectedTo: 	An address to which above mentioned ip is connected via ethernet.
*/
class NetInterface
{
private:
  string ip;
  string connectedTo; //this node is connected to this ip

public:
  string getip()
  {
    return this->ip;
  }
  string getConnectedIp()
  {
    return this->connectedTo;
  }
  void setip(string ip)
  {
    this->ip = ip;
  }
  void setConnectedip(string ip)
  {
    this->connectedTo = ip;
  }
};

/*
  Struct of each node
  name: 	It is just a label for a node
  interfaces: 	List of network interfaces a node have
  Node* is part of each interface, it easily allows to send message to another node
  mytbl: 		Node's routing table
*/
class Node
{
private:
  //uncomment this plz
  // string name;
  vector<pair<NetInterface, Node *>> interfaces;

protected:
  string name;
  struct routingtbl mytbl;
  virtual void recvMsg(RouteMsg *msg)
  {
    cout << "Base" << endl;
  }
  bool isMyInterface(string eth)
  {
    for (int i = 0; i < interfaces.size(); ++i)
    {
      if (interfaces[i].first.getip() == eth)
        return true;
    }
    return false;
  }

public:
  void setName(string name)
  {
    this->name = name;
  }

  void addInterface(string ip, string connip, Node *nextHop)
  {
    NetInterface eth;
    eth.setip(ip);
    eth.setConnectedip(connip);
    interfaces.push_back({eth, nextHop});
  }

  void addTblEntry(string myip, int cost)
  {
    RoutingEntry entry;
    entry.dstip = myip;
    entry.nexthop = myip;
    entry.ip_interface = myip;
    entry.cost = cost;
    mytbl.tbl.push_back(entry);
  }
  void addNewTblEntry(string myip, string dstip, string nexthop, int cost)
  {
    RoutingEntry entry;
    entry.dstip = dstip;
    entry.nexthop = nexthop;
    entry.ip_interface = myip;
    entry.cost = cost;
    mytbl.tbl.push_back(entry);
  }
  void updateTblEntry(string dstip, string nexthop, int cost)
  {
    for (int i = 0; i < mytbl.tbl.size(); i++)
    {
      if (mytbl.tbl[i].dstip == dstip)
      {

        mytbl.tbl[i].nexthop = nexthop;

        mytbl.tbl[i].cost = cost;

        break;
      }
    }
  }
  void removeInterface(string node)
  {
    int removed = 0;
    for (int i = 0; i < interfaces.size(); i++)
    {
      if (node == interfaces[i].second->name)
      {
        interfaces.erase(interfaces.begin() + i);
        break;
      }
    }
  }
  vector<pair<NetInterface, Node *>> getInterfaces()
  {
    return this->interfaces;
  }
  string getName()
  {
    return this->name;
  }

  struct routingtbl getTable()
  {
    Comparator myobject;
    sort(mytbl.tbl.begin(), mytbl.tbl.end(), myobject);
    return mytbl;
  }

  void printTable()
  {
    Comparator myobject;
    sort(mytbl.tbl.begin(), mytbl.tbl.end(), myobject);
    cout << this->getName() << ":" << endl;
    for (int i = 0; i < mytbl.tbl.size(); ++i)
    {
      cout << mytbl.tbl[i].dstip << " | " << mytbl.tbl[i].nexthop << " | " << mytbl.tbl[i].ip_interface << " | " << mytbl.tbl[i].cost << endl;
    }
  }

  void sendMsg()
  {
    struct routingtbl ntbl;
    for (int i = 0; i < mytbl.tbl.size(); ++i)
    {
      ntbl.tbl.push_back(mytbl.tbl[i]);
    }

    for (int i = 0; i < interfaces.size(); ++i)
    {
      RouteMsg msg;
      msg.from = interfaces[i].first.getip();

      msg.recvip = interfaces[i].first.getConnectedIp();

      //Q3 part
      for (int j = 0; j < ntbl.tbl.size(); j++)
      {
        if (interfaces[i].second->isMyInterface(ntbl.tbl[j].nexthop))
        {
          ntbl.tbl[j].cost = ntbl.tbl[j].share_cost;
        }
      }
      msg.mytbl = &ntbl;
      interfaces[i].second->recvMsg(&msg);
    }
  }
};

class RoutingNode : public Node
{
public:
  void recvMsg(RouteMsg *msg);
};
