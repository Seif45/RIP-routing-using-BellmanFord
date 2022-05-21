#include "node.h"
#include <iostream>

using namespace std;


int changes; //global value to notice when the algorithm converges and no more changes happened

void routingAlgo(vector<RoutingNode*> nd){
  changes = 1; //initial
  while (changes > 0){ //loop until no more changes aka converges
    changes = 0;
    for (int i =0 ; i < nd.size(); i++) { //for every node in the graph
      nd[i]->sendMsg(); //send message to all its neighbours
    }
  }
}


void RoutingNode::recvMsg(RouteMsg *msg) { //message received by the neighbours
  for (int i = 0; i < msg->mytbl->tbl.size(); i++) { //for every entry in the table of the message sender
    if (isMyInterface(msg->mytbl->tbl[i].nexthop)) { //if this route from the sender goes through me then ignore it
      continue;
    }
    RoutingEntry entry = msg->mytbl->tbl[i]; //store this entry with the same destination IP
    entry.nexthop = msg->from; //change the nexthop IP
    entry.ip_interface = msg->recvip; //change the interface IP
    entry.cost++; //increase the cost by 1
    bool flag = true;
    for (int j = 0; j < mytbl.tbl.size(); j++) { //for every entry in the table of the current neighbour
      if (mytbl.tbl[j].dstip == entry.dstip) { //if the destination matches (are the same)
        flag = false; //mark that a match is found
        if (entry.cost < mytbl.tbl[j].cost) { //if the route gives a less cost
          mytbl.tbl[j].nexthop=msg->from; //update the entry in the table
          mytbl.tbl[j].ip_interface=msg->recvip;
          mytbl.tbl[j].cost=entry.cost;
          changes++; //mark that a change has been made for convergence
          break;
        }
      }
    }
    if (flag){ //if the table doesn't have any entry that goes to that destination
      mytbl.tbl.push_back(entry); //then add this new route as an entry to the table
      changes++; //mark a change
    }
  }
}




