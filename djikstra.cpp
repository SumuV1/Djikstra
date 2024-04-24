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
    double len;
    edge* next;
};

void Add(edge*& h, int _to, double _len)
{
    edge* temp = new edge;
    temp->to = _to;
    temp->len = _len;
    temp->next = h;
    h = temp;
}

struct miasto {
    double x;
    double y; // reprezentacja punktu na płaszczyźnie
    string nazwa;
    edge* sasiedzi;
    bool odwiedzony;
    //int odwiedzony;
    double dystans;

    miasto(string _nazwa, double _x, double _y,bool _odwiedzony)
    {
        x = _x;
        y = _y;
        nazwa = _nazwa;
        sasiedzi = nullptr;
        odwiedzony=_odwiedzony;
        dystans=1000000;
    }
};

miasto* M[2284];

double dist(miasto A, miasto B)
{
    return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2)) * 100;
}

void WypelnianieMiast()
{
    ifstream Wodczyt("Miasta5.txt");
    string wiersz;
    int i = 0;
    while (!Wodczyt.eof() && i < 2284)
    {
        getline(Wodczyt, wiersz);
        string nazwa_miasta = wiersz.substr(0, wiersz.find('#'));
        wiersz = wiersz.substr(wiersz.find('#')+1);
        string koordynaty_x = wiersz.substr(0, wiersz.find('#'));
        wiersz = wiersz.substr(wiersz.find('#')+1);
        string koordynaty_y = wiersz.substr(0,5);
        //cout << nazwa_miasta << '\n' << koordynaty_x << '\n' << koordynaty_y << '\n';
        M[i] = new miasto(nazwa_miasta, stod(koordynaty_x), stod(koordynaty_y),false);
        i++;
    }
    Wodczyt.close();
}


void WypelnianieKrawedzi()
{
    for (int i = 0; i < 2284; i++)
    {
        int z = 0;
        //cout<<M[i]->nazwa;
        for (int j = 0; j < 2284; j++)
        {
            double D = dist(*M[i], *M[j]);
            if (D < 25 && i != j)
            {
                Add(M[i]->sasiedzi, j, D);
            }
        }
    }
}


//Do użycia po znalezieniu tablicy z dystansami między węzłami
//czyli do obliczeń PO algorytmie Djikstry
//Oblicza całkowity dystans między miastami

double Dystans(int droga[], int ile)
{
    double s = 0;
    for (int i = 0; i < ile - 1; i++)
        s += dist(*M[droga[i]], *M[droga[i + 1]]);

    s += dist(*M[droga[0]], *M[droga[ile - 1]]);
    return s;
}

//Wypisuje droge miedzy kazdym węzłem
void Wypisz(int droga[], int ile)
{
    for (int i = 0; i < ile; i++) cout << M[droga[i]]->nazwa << ", ";
}

int start_id=0;
int finish_id=0;
void Podaj_S_i_F(string start,string finish)
{
    bool exist_A;
    bool exist_B;
    while(start_id == 0 || finish_id == 0)
    {
        cout<<"Podaj Start: ";
        cin>>start;
        cout<<"Podaj Koniec: ";
        cin>>finish;

        for(int y=0;y<2284;y++)
        {
            if(M[y]->nazwa==start)
            {
                exist_A=true;
                start_id=y;
            }
            if(M[y]->nazwa==finish)
            {
                exist_B=true;
                finish_id=y;
            }
        }
    }

    if(exist_A!=true||exist_B!=true)
    {
        cout<<"Przynajmniej jedno z miast nie istnieje";
    }else{
        cout<<"\nMiasto Start: "<<M[start_id]->nazwa<<" ma ID "<<start_id;
        cout<<"\nMiasto Koniec: "<<M[finish_id]->nazwa<<" ma ID "<<finish_id;
        cout<<"\n";
    }
}
//indeks krawedzi potrzebny mi tylko do okreslenia sciezki powrotu
//na podstawie tego, do jakiego węzła porwadzi dana krawędź, sprawdz długość do tego miasta i wybierz najmniejszy wynik

//wyciagnij z walkera najmniejsza krawedz
//krawedz ma w sobie odnosnik do nastepnych krawedzi
//wertujesz po edge->next i sprawdzasz dlugosc len
//najm len jest brane pod uwage

//przemieszczenie musi byc zmiana lokalizacji walkera na nastepny wezel oraz zapisanie drogi

void Djikstra_v2(int start,int finish,int droga[])
{
    //1.nieodwiedzone węzły mają wartosc "bool odwiedzone=false"
    //odwiedzone będą miały wartość true oraz zapisywać ID węzła przed iteracją z przejściem do następnego węzła

    //2.Kazdy wezel ma dystans rowny 1000000, z racji ze nie wiem jak ustawic nieskonczonosc, ale mysle ze tyle styknie ;)

    int d=0;
    droga[d]=start;
    miasto* walker=M[start];
    walker->dystans=0;
    miasto* cel=M[finish];

    while(walker->nazwa!=cel->nazwa)
    {
        double najm_len_sasaiada=1000000;
        int id_najm_len_sasiada=0;
        cout<<"\nMiasto przed iteracja: "<<walker->nazwa;
        while(walker->sasiedzi)
        {
            //3.Dla walkera policz dystans do kazdego sasiada przypisujac mu ten dystans, jeśli jest mniejszy niz ten milion
            //dystans wezla na ktorym jest walker + dystans juz policzony
            if(walker->sasiedzi->len<M[walker->sasiedzi->to]->dystans)
            {
                //3.1 Aktualizacja drogi jesli jest mniejsza niz domyslna
                if(M[walker->sasiedzi->to]->dystans < 1000000)
                {
                    M[walker->sasiedzi->to]->dystans += walker->sasiedzi->len;
                }
                else if(M[walker->sasiedzi->to]->dystans == 1000000)
                {
                    M[walker->sasiedzi->to]->dystans = walker->dystans + walker->sasiedzi->len;
                }
                //jaki jest najm dystans zsumowany do miast sąsiadujących przez aktualne miasto <---
                //jakie jest ID tego miasta

                najm_len_sasaiada=M[walker->sasiedzi->to]->dystans;
                if(M[walker->sasiedzi->to]->dystans==najm_len_sasaiada)
                {
                    id_najm_len_sasiada=walker->sasiedzi->to;
                }
                //spr jakie jest id miasta do ktorego prowadzi najm dlugosc

                //przypisz najm krawedz i spr ktora to krawedz, bo potem moze byc inna niz ostatnia sprawdzana w pkt5 PO IDku
            }
            if (walker->sasiedzi->next != nullptr)
            {
                walker->sasiedzi=walker->sasiedzi->next;
            }else{
                break;
            }
        }

        //4. Oznaczenie węzła jako odwiedzonego
        walker->odwiedzony=true;

        //5. Wybór nast. węzła z nieodwiedzonych, mającego najm dystans
        if(M[id_najm_len_sasiada]->odwiedzony==false && M[id_najm_len_sasiada]->dystans==najm_len_sasaiada)
        {
            droga[++d]=walker->sasiedzi->to;
            walker=M[walker->sasiedzi->to];
            cout<<"\nMiasto po iteracji: "<<walker->nazwa<<endl;
        }
    }

    if(walker==cel)
    {
        cout<<"\nDotarlem do miasta "<<walker->nazwa<<endl;
    }else{
        cout<<"\nGdzieś mnie wywiało...";
    }
}

int main()
{
    string start;
    string finish;
    int droga[2284];

    //int num = 2037;//Warszawa
    //int num = 1294;//Opoczno
    int num = 2258;//Zarnow
    WypelnianieMiast();
    cout << M[num]->nazwa << "      " << M[num]->x << "      " << M[num]->y << endl;
    WypelnianieKrawedzi();
    cout << "Sasiedzi:" << endl;
    edge* temp = M[num]->sasiedzi;
    while (temp)
    {
        cout << M[temp->to]->nazwa << ", ";  temp = temp->next;
    }
    cout << "\n";
    Podaj_S_i_F(start,finish);
    //Djikstra(start_id,finish_id,droga);
    Djikstra_v2(start_id,finish_id,droga);
    //cout<<droga[0];
}








//______________________________________________________________________//
/*

void Djikstra(int start,int finish,int droga[])
{
    int d=0;
    droga[d]=start;
    miasto* walker=M[start];
    miasto* cel=M[finish];
    while(walker!=cel)
    {
        int id_next_miasta;
        //muszę sprawdzić na jednej krawędzi dlugosc i id miasta do ktorego porwadzi krawedz
        //potem przejść do następnej krawędzi i znów sprawdzic

        //while(walker->sasiedzi=walker->sasiedzi->next)
        while(walker->sasiedzi)
        {
            if(walker->sasiedzi->len<walker->dystans)//dystans do zmiany
            {
                walker->dystans=walker->sasiedzi->len;//dystans do zmiany
                id_next_miasta=walker->sasiedzi->to;
                //jakie id (to) ma krawedz o najm len
            }
            //walker->sasiedzi=walker->sasiedzi->next;
            if (walker->sasiedzi->next != nullptr)
            {
                walker->sasiedzi=walker->sasiedzi->next;
            }else{
                break;
            }
        }
        cout<<"Najblizsze miasto dla "<<walker->nazwa<<" to "<<M[id_next_miasta]->nazwa<<" majace ID "<<id_next_miasta<<" odlegle o "<<walker->dystans<<endl;//dystans do zmiany

        if(!M[id_next_miasta]->odwiedzony)
        {
            walker->odwiedzony=true;
            droga[++d]=id_next_miasta;
            walker=M[id_next_miasta];
        }
        cout<<"\n\nMiasto walkera pod koniec while'a: "<<walker->nazwa<<endl;
    }

    if(walker==cel)
    {
        cout<<"\nDotarlem do miasta "<<walker->nazwa<<endl;
    }else{
        cout<<"\nGdzieś mnie wywiało...";
    }
}
*/