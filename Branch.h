#ifndef BRANCH_H
#define BRANCH_H

#include "StateNode.h"
#include "File.h"
#include <iostream>
#include <vector>
#include <string>

struct Branch{

	StateNode * origin; // si es null entonces es branch master
	StateNode * current_state; 
	StateNode * init_state;
	string name;
	vector<File*> files;

	int count_v=0;
	Branch(string name);
	Branch(StateNode * origin, string name);
	void Merge(Branch ** &other_branch);

	void NewState(string name); // a new version from current state 
	void AddState(StateNode * new_state);// add an existing state
	bool FindState(string x, StateNode ** &tmp);
	void UpdateCurrentState();
	
	string GetStatesInfo();

	void NewVersion();
	void UpdateFile();

	bool active=1;
};

Branch::Branch(string name){
	origin = NULL;
	current_state = new StateNode();
	current_state->initial = 1;
	init_state = current_state;
	current_state->name = name+"_v"+to_string(count_v);
	current_state->modification = name+"_v"+to_string(count_v);
	count_v+=1;
	File * n = new File("files/",name + "_file.txt");
	files.push_back(n);
	this->name = name;
//	ofstream fl (name+"_file.txt");
//	fl << current_state->name;
}

Branch::Branch(StateNode * origin, string name){
	this->origin = origin;
	current_state = new StateNode();//StateNode(origin,name+"_v"+to_string(count_v));
	current_state->StateNodeB(origin,name+"_v"+to_string(count_v));
	current_state->modification = name+"_v"+to_string(count_v);
	File * n = new File("files/",name + "_file.txt");
	files.push_back(n);
	count_v +=1;
	current_state->initial = 1;
	init_state = current_state;
	this->name = name;
}

void Branch::NewState(string name){
	StateNode * new_state = new StateNode(current_state,name);
	new_state->modification = name;
	current_state = new_state;

}

bool Branch::FindState(string x, StateNode ** &tmp){
	tmp = &current_state;
	while( (*tmp) ){
		if( (*tmp)->name == x ) return 1;
		if((*tmp)->initial) break;
		tmp = &(*tmp)->origin[0];
	}
	return 0;
}

void Branch::Merge(Branch ** &other_branch){
	current_state->origin.push_back((*other_branch)->current_state);
	//cout << current_state->name<<endl;
	//cout << (*other_branch)->current_state->name;
	//other_branch->current_state = this->current_state;
	(*other_branch)->AddState(current_state);
}
void Branch::NewVersion(){
	string nstatename = this->name + "_v"+to_string(this->count_v);
	NewState(nstatename);
	count_v+=1;
}

void Branch::AddState(StateNode * new_state){
	this->current_state->descendant.push_back(new_state);
	this->current_state = new_state;

}

void Branch::UpdateCurrentState(){
	StateNode ** tmp;
	tmp = &init_state;
	while((*tmp)){
		cout << (*tmp)->name;
		if(!(*tmp)->descendant[0] || !(*tmp)->descendant[0]->active){
			break;	
		}
		tmp = &(*tmp)->descendant[0];
	}
	current_state = *tmp;
}

void Branch::UpdateFile(){
	files[0]->GetCurrentStatus(this->current_state);
	fstream file;
	file.open ("files/"+this->name+"_file.txt");
	for(int i=0; i < files[0]->current_status.size(); i++){
		file << files[0]->current_status[i]<<endl;
	}
	file.close();

}

string Branch::GetStatesInfo(){
	StateNode ** tmp = &current_state;
	string info ="";
	string actnodes;
	while( (*tmp) ){
		if (!(*tmp)->active){
			actnodes += (*tmp)->name + " [style=dotted]\n";
		}
		info += (*tmp)->name;
		if((*tmp)->initial) break;
		info +=  "->";
		tmp = &(*tmp)->origin[0];
	}
	if (!!origin){
		info += "->";
		info += origin->name;
	}
	info += '\n';
	info += actnodes;
	return info;
}


#endif // BRANCH_H
