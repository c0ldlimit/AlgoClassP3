#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <iterator>
#include <string>

using namespace std;

typedef map < int, vector < int > > graph;
typedef pair< int, vector < int > > mapPair;

int random_0_to_n(int min, int max)
{
    srand ( time(NULL) );
    return min + (rand() % (int)(max - min + 1));
}

void print_graph(graph &v)
{
    graph::iterator J;
    vector<int>::iterator I;
    for (J=v.begin(); J!=v.end(); ++J)
    {
        cout << "Key: " << (*J).first << "::";
        for (I=(*J).second.begin();I!=(*J).second.end(); ++I)
        {
            cout << " " << *I;
        }
        cout << endl;
    }
}

int mergeNodes(graph &v, int node1, int node2, int maxNode)
{

    vector<int> incidentNodeNew;
    vector<int>::iterator it;

    for (it=v[node1].begin(); it!=v[node1].end(); ++it)
    {
        if (*it != node2)
        {
            incidentNodeNew.push_back(*it);
        }
    }

    for (it=v[node2].begin(); it!=v[node2].end(); ++it)
    {
        if (*it != node1)
        {
            incidentNodeNew.push_back(*it);
        }
    }

    // add new "merged" node
    int mergedKey = maxNode+1;
    v[mergedKey] = incidentNodeNew;

    // erase merged nodes
    v.erase(node1);
    v.erase(node2);

    // replace all other incident nodes in v with the new node nomenclature
    graph::iterator graphit;
    for (graphit=v.begin(); graphit!=v.end(); ++graphit)
    {
        if ((*graphit).first != node1 && (*graphit).first != node2)
        {
            for (it=(*graphit).second.begin(); it!=(*graphit).second.end(); ++it)
            {
                if (*it == node1 || *it == node2)
                {
                    *it = mergedKey;
                }
            }
        }
    }

    return mergedKey;

    // merge set of incident nodes excluding idx1 and idx2 - these are the vertices
    // that are incident to the new "merged" node
    // erase idx1 and idx2
}

int main()
{
    graph karger;

    ifstream infile("C:\\Documents and Settings\\c\\My Documents\\Downloads\\kargerAdj.txt");

    if (!infile) {
        cout << "There was a problem opening file "
             << " for reading."
             << endl;
        return 0;
    }
    cout << "Opened for reading." << endl;

    int val=0;
    vector<int> intVec;
    string s = "";
    while (infile.good()) {

        getline(infile,s);
        cout << s << endl;

        istringstream iss(s);
        int count = 0;
        int keyVal = 0;
        while (iss>>val)
        {
            if (count==0)
            {
                karger.insert( mapPair(val,intVec) );
                keyVal = val;
            }
            else
            {
                karger[keyVal].push_back(val);
            }
            //cout << val << endl;
            ++count;
        }

    }

    print_graph(karger);


    int maxidx = karger.size();
    int currSize = maxidx;
    int node1, node2;
    graph::iterator item = karger.begin();
    while(currSize>2)
    {
        item = karger.begin();
        advance( item, random_0_to_n(0,currSize-1) );
        node1 = (*item).first;
        node2 = (*item).second[0];
        cout << "Merging Nodes: " << node1 << " and " << node2 << endl;
        maxidx = mergeNodes(karger,node1,node2,maxidx);
        //print_graph(karger);

        // update size
        currSize = karger.size();
    }

    cout << "MIN CUT: " << karger[maxidx].size() << endl;

    return 0;

    // while node count > 2 keep merging nodes
    // randomly select two nodes and merge them
    // update maxIdx

    // the number of incident nodes left will be the min cut and should be the same.
}
