#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>
using namespace std;

class Node{
    public:
        Node (string _name) : name(_name){

        }
        bool isChild(shared_ptr<Node> ptr){
            for(auto i = childs.begin(); i != childs.end(); i++){
                if((*i)->name==ptr->name){
                    return true;
                }
            }
            return false;
        }
        bool addChild(shared_ptr<Node> ptr){
            if(isChild(ptr)){
                return false;
            }
            childs.push_back(ptr);
            return true;
        }

        string name;
        vector<shared_ptr<Node>> childs;

};

struct CmpPtr {
    bool operator()(const shared_ptr<Node> lhs, const shared_ptr<Node> rhs) const
    {
        return lhs.get() < rhs.get();
    }
};

class Graph{
    public:
        Graph(){}
        ~Graph(){}
        vector<shared_ptr<Node>> nodes;
        bool nodeExist(string name){
            for(auto ptr = nodes.begin(); ptr != nodes.end(); ptr++){
                if((*ptr)->name == name){
                    return true;
                }
            }
            return false;
        }
        shared_ptr<Node> addNode(string name){
            auto res = make_shared<Node>(name);
            nodes.push_back(res);
            return res;
        }
        shared_ptr<Node> getNode(string name)const{
            for(auto ptr = nodes.begin(); ptr != nodes.end(); ptr++){
                if((*ptr)->name == name){
                    return *ptr;
                }
            }
            return nullptr;
        }
        bool addPath(string from, string to){
            shared_ptr<Node> fromNode = nullptr;
            shared_ptr<Node> toNode = nullptr;
            if(!nodeExist(from)){
                fromNode = addNode(from);
            }
            else{
                fromNode = getNode(from);
            }
            if(!nodeExist(to)){
                toNode = addNode(to);
            }
            else{
                toNode = getNode(to);
            }

            return fromNode->addChild(toNode);

        }


        pair<std::string, list<std::string>> bfs(shared_ptr<Node> from, const set<string> & to)const{
            list<string> path;

            // if(to.find(from->name) != to.end()){
            //     return pair<string, list<string>>(from->name, path);
            // }

            queue<shared_ptr<Node>> qu;
            qu.push(from);

            //vector<bool> visited (nodes.size(), false);
            set<shared_ptr<Node>, CmpPtr> visited;
            map<shared_ptr<Node>, shared_ptr<Node>, CmpPtr> prev;

            shared_ptr<Node> currentNode = nullptr;
            while(!qu.empty()){
                currentNode = qu.front();
                qu.pop();

                if(to.find(currentNode->name) != to.end()){
                    while(currentNode != from){
                        path.push_back(currentNode->name);
                        currentNode = prev.at(currentNode);
                    }
                    path.push_back(currentNode->name);
                    return pair<string, list<string>>(from->name, path);
                }

                for(auto it = currentNode->childs.begin(); it != currentNode->childs.end(); it++){
                    if(visited.find(*it) != visited.end()){
                        continue;
                    }
                    qu.push(*it);
                    visited.insert(*it);
                    prev.emplace(*it, currentNode);
                }


            }
            //not found !
            return pair<string, list<string>>(from->name, {});
        }
};


 
class CDelivery
{
public:
	// ctor
    CDelivery(){

    }
    ~CDelivery(){

    }
	// dtor
 
	// add connection from -> to, fluent interface
	CDelivery & addConn ( const string & from, const string & to );
 
	// returns a map containing shortest path to each customer from any of the depots
	// map key is the customer name, value is a list of nodes on the shortest path (depot -> customer)
	// if there is no path, the value is an empty list
	map<string, list<string>> serveCustomers ( const set<string> & customers, const set<string> & depots ) const;
 
    Graph graph;
private:
	// todo
    
};
 
CDelivery & CDelivery::addConn ( const string & from, const string & to )
{
	// todo
    //graph.addPath(from, to);
    graph.addPath(to, from);
    return *this;
}
 
map<string, list<string>> CDelivery::serveCustomers ( const set<string> & customers, const set<string> & depots ) const
{
	// todo
    map<std::string, list<std::string>> result;

    //for(auto i = customers.begin(); i != customers.end(); i++){
    for(auto custIt = customers.begin(); custIt != customers.end(); custIt++){
        //does customer exist?

        auto customer = graph.getNode(*custIt);
        auto pair = graph.bfs(customer, depots);
        result.emplace(pair.first, pair.second);

    }
    return result;
}
 
int main ()
{
	CDelivery t0;
	map<string, list<string>> r;
	t0.addConn("Austin", "Berlin");
	t0.addConn("Chicago", "Berlin");
	t0.addConn("Berlin", "Dallas");
	t0.addConn("Dallas", "Essen");
	t0.addConn("Essen", "Austin");
	t0.addConn("Frankfurt", "Gyor");
	t0.addConn("Gyor", "Helsinki");
	t0.addConn("Helsinki", "Frankfurt");
 
	r = t0.serveCustomers(set<string>{"Berlin", "Gyor"}, set<string>{"Essen", "Helsinki"});

    // for(auto i : r){
    //     cout << i.first << endl;
    //     for(auto j : i.second){
    //         cout << "-" << j << endl;
    //     }
    // }

	assert ( r == (map<string, list<string>>{ {"Berlin", {"Essen", "Austin", "Berlin"}}, {"Gyor", {"Helsinki", "Frankfurt", "Gyor"}} }) );
 
	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
	t0.addConn("Chicago", "Helsinki");
 
	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
	t0.addConn("Berlin", "Chicago");
 
	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Austin", "Berlin", "Chicago", "Helsinki", "Frankfurt", "Gyor"}} }) );
	t0.addConn("Essen", "Frankfurt");
 
	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Dallas", "Essen", "Frankfurt", "Gyor"}} }) );
 
    cout << "OK" << endl;
    
	return EXIT_SUCCESS;
}