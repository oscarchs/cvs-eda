#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include "StateNode.h"
#include "StringSplitter.h"


struct File{

	string name;
	string path;
	vector<string> current_status;
	LL::StringSplitter splitter;
	File(string path,string name);
	void GetCurrentStatus(StateNode * current_state);
	bool Compare(vector<string> file);
};

File::File(string path,string name){
	this->name = name;
	this->path = path;
	ofstream fl(this->path + this->name);
	fl << this->name<<endl;
}

void File::GetCurrentStatus(StateNode * current_state){
	stack<string> all_modifications;
	vector<string> current;
	StateNode ** tmp;
	tmp = &current_state;
	current_state->GetMods(all_modifications,tmp);
	
	while(!all_modifications.empty()){
		current.push_back(all_modifications.top());
		all_modifications.pop();
	}	
	this->current_status = current;
}


bool File::Compare(vector<string> file){
	if(file.size() != this->current_status.size()){
		return false;
	}
	for( int i=0; i < this->current_status.size(); i++){
		if(this->current_status[i] != file[i]){
			return false;
		}
	}
	return true;
}


#endif //FILE_H
/*
void File::GetCurrentStatus(StateNode * current_state){
	stack<string> all_modifications;
	StateNode ** tmp;
	tmp = &current_state;
	while( (*tmp) ){
		if((*tmp)->active){
			splitter.set_string((*tmp)->modifications_box[this->name]);
			splitter.split(' ');
			for(int i=0; i < splitter.size(); i++){
				all_modifications.push(splitter[i]);
			}
		}
		if((*tmp)->initial) break;
		tmp = &(*tmp)->origin[0];
	}

	vector<string> current;
	while(!all_modifications.empty()){
		if(all_modifications.top().at(0) == '+'){
			all_modifications.top().erase(0,1);
			current.push_back(all_modifications.top());
		}
		else{
			all_modifications.top().erase(0,1);
			for(int q=0; q < current.size(); q++){
				if (current[q] == all_modifications.top()){
					current.erase(current.begin()+q);
				}
			}
		}
		all_modifications.pop();
	}
	for(int i =0; i < current.size(); i++){
		for(int j=0; j < current[i].size(); j++){
			if(current[i][j] == '-'){
				current[i][j] = ' ';
			}
		}
	}
	this->current_status = current;
}
*/
