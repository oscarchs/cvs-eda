#ifndef CVS_H
#define CVS_H

#include "StateNode.h"
#include "Branch.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>

struct CVS{
	string name;
	vector < Branch * > branches;
	int current_branch;
	StateNode * init;

	CVS(string name);
	void NewBranch(string name);
	void NewBranch(string statename, string branchname);
	bool FindState(string x,StateNode ** &tmp);
	void FindState2(string x,StateNode ** tmp, StateNode ** &tmp2);
	void DeleteBranch(string name);  // name can only be an initial branch state
	void DeleteState(StateNode ** tmp);
	void RestoreState(StateNode ** tmp);

	void Delete(string state);
	void Restore(string state);
    

    bool FindBranch(Branch ** &tmp,string branchname);
    void MergeBranch(string branchname);
	void UpdateBranches();
	void Status();
	void CheckOut(string branchname);
	void ShowBranches();
	void ShowProject();

	void GenDot();
	void routes(StateNode ** tmp,string &data);
	void GenDot2();

};

CVS::CVS(string name){
	this->name = name;
	Branch * nbranch = new Branch("master");
	branches.push_back(nbranch);
	current_branch = 0;
	init = branches[current_branch]->current_state;

}

void CVS::NewBranch(string name){
	Branch * nbranch = new Branch(branches[current_branch]->current_state,name);
	branches.push_back(nbranch);
}

void CVS::NewBranch(string statename, string branchname){
	StateNode ** tmp;
	if( FindState(statename,tmp) ){
		Branch * nbranch;
		nbranch = new Branch(*tmp,branchname);
		branches.push_back(nbranch);
	}
}

bool CVS::FindState(string x,StateNode ** &tmp){
	for(int i=0; i<branches.size(); i++){
		if ( branches[i]->FindState(x,tmp)){
			return 1;
		}
	}
	return 0;
}

void CVS::FindState2(string x,StateNode ** tmp, StateNode ** &tmp2){
	if(!(*tmp)){
		return;
	}
	if( (*tmp)->name  == x){
		tmp2 = tmp;
		return;
	}
	for(int i=0; i < (*tmp)->descendant.size();i++){
			FindState2(x,&(*tmp)->descendant[i],tmp2);
		}
}

void CVS::Delete(string state){
	StateNode ** tmp;
	if( FindState(state,tmp) ){
		//branches[current_branch]->current_state = (*tmp)->origin[0];
		DeleteState(tmp);
		UpdateBranches();
	}
}
void CVS::DeleteState(StateNode ** tmp){
	if(!(*tmp)){
		return;
	}
	//cout << (*tmp)->name <<endl;
	(*tmp)->active = 0;
	for(int i=0; i < (*tmp)->descendant.size();i++){
		DeleteState(&(*tmp)->descendant[i]);
	}
}

void CVS::Restore(string state){
	StateNode ** tmp;
	tmp = &init;
	StateNode ** tmp2;
	int index_acc;
	FindState2(state,tmp,tmp2);
	if((*tmp2)->old){
		cout << "handle this";
		tmp2 = &(*tmp2)->origin[0];
		DeleteState(&(*tmp2)->descendant[0]);
		for(int i=0; i< (*tmp2)->descendant.size(); i++){
			if((*tmp2)->descendant[i]->name == state){
				index_acc = i;
			}
		}
		swap((*tmp2)->descendant[0],(*tmp2)->descendant[index_acc]);
		RestoreState(&(*tmp2)->descendant[0]);
		(*tmp2)->descendant[0]->name.erase((*tmp2)->descendant[0]->name.end()-4,(*tmp2)->descendant[0]->name.end());
		(*tmp2)->descendant[index_acc]->name +="_old";
		(*tmp2)->descendant[index_acc]->old =1;
		(*tmp2)->descendant[0]->old = 0;
		UpdateBranches();
	}
	else{
		RestoreState(tmp2);
		UpdateBranches();
	}
//		FindState(state,tmp2);
//		cout << (*tmp2)->name;

}
void CVS::RestoreState(StateNode ** tmp){
	if(!(*tmp)){
		return;
	}
	(*tmp)->active = 1;
	for(int i=0; i < (*tmp)->descendant.size();i++){
		RestoreState(&(*tmp)->descendant[i]);
	}
}

void CVS::UpdateBranches(){
	StateNode ** tmp;
	for(int i=0; i< branches.size(); i++){
		branches[i]->UpdateCurrentState();
	}
}
void CVS::CheckOut(string branchname){
	for(int i=0; i< branches.size(); i++){
		if(branches[i]->name == branchname){
			current_branch = i;
		}
	}

}

bool CVS::FindBranch(Branch ** &tmp,string branchname){
	tmp = &(branches[0]);
	for(int i=0; i < branches.size(); i++){
		tmp = &(branches[i]);
		if(branches[i]->name == branchname){
			return true;
		}
	}
	return false;
}


void CVS::MergeBranch(string branchname){
	Branch ** tmp;
	if(FindBranch(tmp,branchname)){
		branches[current_branch]->NewVersion();
		branches[current_branch]->Merge(tmp);
	}
}

void CVS::Status(){


	//compara cada file * con su similar en el path
	//antes de comparar se debe hacer un get_current_status de cada file.
	// se devuelve un vector de Files modificados y/o nuevos
}



void CVS::ShowProject(){
	cout <<"/"<< this->name << endl;
	for(int i=0; i< branches.size(); i++){
		cout <<"  " << "/"<<branches[i]->name<<endl;
		cout <<"     "<< branches[i]->GetStatesInfo()<<endl;
	}
}

void CVS::GenDot(){
  string tmp ="digraph{\n\n";
  tmp += "rankdir=RL;\n\n";
  for(int i=0;i<branches.size();i++){
    tmp += branches[i]->GetStatesInfo();
    tmp += "\n";
  }
  tmp += "}\n";
  //cout << tmp;
  ofstream graph("graph.dot");
  graph<<tmp;
  return ;
}

void CVS::routes(StateNode ** tmp,string &data){
	if(!(*tmp)){
		return;
	}

	for(int i=0;i<(*tmp)->descendant.size();i++){
		if((*tmp)->descendant[i] != NULL){
			data += (*tmp)->name + "->" +(*tmp)->descendant[i]->name +'\n';	
			if (!(*tmp)->active){
			data += (*tmp)->name + " [style=dotted] ";
			}
			if (!(*tmp)->descendant[i]->active){
			data += (*tmp)->descendant[i]->name + " [style=dotted]";
			}
			data += "\n";
			routes(&(*tmp)->descendant[i],data);
		}
	}
}

void CVS::GenDot2(){
	StateNode ** tmp;
	tmp = &init;
	string data ="digraph{\n\n";
    data += "rankdir=RL;\n\n";
	routes(tmp,data);
    data += "}\n";
	//cout << data;
	ofstream graph("graph.dot");
  	graph<<data;
  	return ;
}


#endif // CVS_H
