#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <time.h>
#include <string.h>

using namespace std;

struct edge
{
  int to;
  float len;
  edge * next;
};



void Add(edge*& h, int _to, float _len)
{
  edge* temp = new edge;
  temp->to = _to;
  temp->len = _len;
  temp->next = h;
  h = temp;
}




struct miasto{
  float x, y; // reprezentacja punktu na p≥aszczyünie
  string nazwa;
  edge *sasiedzi;

  miasto(string _nazwa, float _x, float _y)
    {
        x=_x;
        y=_y;
        nazwa = _nazwa;
        sasiedzi = nullptr;

    }
};


miasto* M[2318];

float dist(miasto A, miasto B)
{
  return sqrt(pow(A.x - B.x,2)+pow(A.y - B.y,2))*100;
}

void WypelnianieMiast()
{
  ifstream Wodczyt("Miasta5.txt");
  string wiersz;
  int i = 0;
  while (!Wodczyt.eof() && i < 2250)
    {
    getline(Wodczyt,wiersz);
    string str2 = wiersz.substr (0, wiersz.find('#'));
    wiersz = wiersz.substr (wiersz.find('#')+1);
    string str3 = wiersz.substr (0, wiersz.find('#'));
    wiersz = wiersz.substr (wiersz.find('#')+1);
    string str4 = wiersz.substr (0, 100);
    std::cout << str2 << '\n' << str3 << '\n' << str4 << '\n';
     M[i]= new miasto(str2,stof(str3),stof(str4));
     i++;
  }
  Wodczyt.close();
}


void WypelnianieKrawedzi()
{
  for(int i=0;i<2250; i++)
  { int z = 0;
    //cout<<M[i]->nazwa;
    for(int j=0;j<2250; j++)
    {
      float D = dist(*M[i],*M[j]);
      if (D<25 && i!=j)
      {
        Add(M[i]->sasiedzi, j, D);
      }
    }
  }
}

float E(int droga[], int ile)
{
  float s = 0;
  for(int i=0; i<ile-1; i++) s += dist(*M[droga[i]], *M[droga[i+1]]);
  s+= dist(*M[droga[0]], *M[droga[ile-1]]);
  return s;
}


void Wypisz(int droga[], int ile)
{
  for(int i=0; i<ile; i++) cout<<M[droga[i]]->nazwa<<", ";
}


int main()
{
  int num = 438;
  WypelnianieMiast();
  cout<<M[num]->nazwa<<"      "<<M[num]->x<<"      "<<M[num]->y<<endl;
  WypelnianieKrawedzi();
  cout << "Sasiedzi:"<<endl;
  edge*temp = M[num]->sasiedzi;
  while(temp) {cout<<M[temp->to]->nazwa<<", ";  temp = temp->next;}
}


