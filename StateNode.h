#ifndef STATENODE_H
#define STATENODE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
using namespace std;

struct StateNode{
	string name;
	vector< StateNode* > origin;
	vector< StateNode* > descendant;
	string modification;

	StateNode();
	StateNode(StateNode * source,string name);
	StateNode(vector < StateNode * > source);

	void GetMods(stack<string> &modifications,StateNode ** tmp);
	bool initial=0;
	bool active=1;
};

StateNode::StateNode(){
	origin.push_back(NULL);
	descendant.push_back(NULL);
}

StateNode::StateNode(StateNode * source,string name){
	origin.push_back(source);
	this->name = name;
	if (source->descendant[0] == NULL){
		source->descendant.pop_back();
	}
	source->descendant.push_back(this);
	descendant.push_back(NULL);
}

StateNode::StateNode(vector < StateNode * > source){
	origin = source;
}

void StateNode::GetMods(stack<string> &modifications,StateNode ** tmp){
	if(!(*tmp)){
		return;
	}
	//cout << (*tmp)->name <<endl;
	modifications.push((*tmp)->modification);
	for(int i=0; i < (*tmp)->origin.size();i++){
		GetMods(modifications,&(*tmp)->origin[i]);
	}
}

#endif // STATENODE_H