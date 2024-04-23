#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <time.h>

using namespace std;

struct edge
{
  int to;
  int len;
  edge * next;
};


void Add(edge*& h, int _to, int _len)
{
  edge* temp = new edge;
  temp->to = _to;
  temp->len = _len;
  temp->next = h;
  h = temp;
}

int main()
{
  edge**G = new edge*[7];
  for(int i=0; i<7; i++) G[i] = nullptr;

  Add(G[0],1,18);   Add(G[1],0,18);
  Add(G[0],4,16);   Add(G[4],0,16);
  Add(G[0],5,17);   Add(G[5],0,17);
  Add(G[0],3,14);   Add(G[3],0,14);
  Add(G[4],1,18);   Add(G[1],4,18);
  Add(G[4],2,15);   Add(G[2],4,15);
  Add(G[4],6,10);   Add(G[6],4,10);
  Add(G[4],5,13);   Add(G[5],4,13);

  Add(G[2],1,12);   Add(G[1],2,12);
  Add(G[2],6,20);   Add(G[6],2,20);
  Add(G[6],5,12);   Add(G[5],6,12);
  Add(G[5],3,16);   Add(G[3],5,16);

  edge* walker = G[4];
  while(walker)
  {
    cout<<4<<"-"<<walker->to<<"   "<<walker->len<<endl;
    walker = walker->next;
  }

}
