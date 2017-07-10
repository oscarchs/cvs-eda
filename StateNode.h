#ifndef STATENODE_H
#define STATENODE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <mutex>
using namespace std;

struct StateNode{
	string name;
	vector< StateNode* > origin;
	vector< StateNode* > descendant;
	string modification;
	mutex m;

	StateNode();
	StateNode(StateNode * source,string name);
	void StateNodeB(StateNode * source,string name);
	StateNode(vector < StateNode * > source);

	void GetMods(stack<string> &modifications,StateNode ** tmp);
	bool initial=0;
	bool active=1;
	bool old=0;
};

StateNode::StateNode(){
	descendant.push_back(NULL);
}

StateNode::StateNode(StateNode * source,string name){
	descendant.push_back(NULL);
	origin.push_back(source);
	this->name = name;
	if (source->descendant[0] == NULL){
		source->descendant[0] = this;
	}
		if(!source->descendant[0]->active){
			source->descendant.push_back(this);
			source->descendant.back()->name = source->descendant[0]->name;
			source->descendant[0]->name += "_old"; 
			source->descendant[0]->old = 1;
			swap(source->descendant.back(),source->descendant[0]);
		}

	//source->descendant.push_back(this);
}

/*
		if(!source->descendant[0]->active){
			source->descendant.push_back(this);
			source->descendant[0]->active = -1;
			swap(source->descendant.back(),source->descendant[0]);
		}
*/
void StateNode::StateNodeB(StateNode * source,string name){
	origin.push_back(source);
	this->name = name;
	source->descendant.push_back(this);

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