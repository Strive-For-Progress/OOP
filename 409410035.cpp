#include <iostream>
#include <utility>
#include <vector>
#include <map>
using namespace std;
typedef unsigned int Uint;

const Uint INF=4294967295, Max_Nodes=1e5+10;
const short first = 0,second = 1;
class node {
public:
    map<Uint,Uint> Hash_old; //Restrict
    map<Uint,Uint> Hash_new; //Restrict
};

Uint nodes,dsts,links;   //Represent the number of nodes,destinations,links respectivly
vector <Uint> DstID(0); //Store destinations
vector <Uint> graph[Max_Nodes]; //Restrict
node vertex [Max_Nodes];
map<pair<Uint,Uint>,Uint> old_w; // Consider edges to be a value
map<pair<Uint,Uint>,Uint> new_w; // Consider edges to be a value

void Dijkstra(Uint root,map<pair<Uint,Uint>,Uint>&w ,short times) {

    vector<Uint> dist(nodes, INF);  //Ensure not less than any edge
    vector<bool> st(nodes, false); //Mean state which is selected or not

    dist[root]=0; //Start from the destination
    //Select the smallest key with smallest value
    for(Uint i=0;i<nodes;i++) {
        Uint key = 0;
        bool flag = false;
        for(Uint j=0;j<nodes;j++)
            if(st[j] == false && (flag == false||dist[key]>dist[j])) {
                 flag=true;
                 key=j;
            }
        st[key]=true;
        //Update its neighbors
        for(auto next : graph[key]) {
            Uint cost = w[{key,next}];
            //Avoid Overflow
            if (dist[next] - dist[key] > cost) {
                dist[next] = dist[key] + cost;
                if (times == first) vertex[next].Hash_old[root] = key;
                else if(times == second)  vertex[next].Hash_new[root] = key;
            } //Check when they are equal since the element in the vector not guarantee sorted in increasing order
            else if (dist[next] - dist[key] ==  cost) {
                if (times == first && vertex[next].Hash_old[root] > key) vertex[next].Hash_old[root] = key;
                else if(times == second && vertex[next].Hash_new[root] > key)  vertex[next].Hash_new[root] = key;
            }
        }
    }
}

void Print() {
    static short cnt = first ; // Record the times called by user
    if (cnt == first) {
        cnt = second;
        for (Uint i=0 ; i<nodes ;i++) {
            cout<<i<<'\n';
            for (Uint j = 0; j<dsts ; j++) {
                //Skip when its destination is itself
                if (i == DstID[j]) continue;
                cout<<DstID[j]<<" "<<vertex[i].Hash_old[DstID[j]]<<'\n';
            }
        }
        return;
    }
    for (Uint i=0 ; i<nodes ;i++) {
        bool flag = true;
        for (Uint j=0; j<dsts ; j++) {
            //Skip when its destination is itself or not be updated
            if (i == DstID[j] ||
                vertex[i].Hash_old[DstID[j]] == vertex[i].Hash_new[DstID[j]]) continue;
                if (flag) cout<<i<<'\n'; //run just once
                cout<<DstID[j]<<" "<<vertex[i].Hash_new[DstID[j]]<<'\n';
                flag = false;
        }
    }
}

int main () {
    cin>>nodes>>dsts>>links;
    //Record the destination
    for (Uint i=0,j ; i<dsts ; i++) {
        cin>>j;
        DstID.push_back(j);
    }

    Uint LinkID,Node1,Node2,oldW,newW;
    // Build the graph (opposite direction)
    for (Uint i=0; i<links;i++) {
        cin>>LinkID>>Node1>>Node2>>oldW>>newW;
        graph[Node1].push_back(Node2);
        graph[Node2].push_back(Node1);
        old_w[{Node1,Node2}] = old_w[{Node2,Node1}] = oldW;
        new_w[{Node1,Node2}] = new_w[{Node2,Node1}] = newW;
    }
    //Origin
    for (Uint i=0 ; i<dsts ; i++) Dijkstra(DstID[i],old_w,first);
    Print();
    //New
    for (Uint i=0 ; i<dsts ; i++) Dijkstra(DstID[i],new_w,second);
    Print();
    return 0;
}
