#include"../include/network.h"
#include<iostream>
using namespace std;
Network::Network()
{
    cout<<"Network constructor"<<endl;
}
Network::~Network()
{
    cout<<"Network destructor"<<endl;
}
void Network::start()
{
    cout<<"Network start"<<endl;
}