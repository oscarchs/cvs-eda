#include "Branch.h"
#include "StateNode.h"
#include "CVS.h"
#include "StringSplitter.h"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

void run(CVS * &project){
	string command;
	LL::StringSplitter splitter;
	while(true){
		cout << "\n cvs: ";
		getline(cin,command);
		splitter.set_string(command);
		splitter.split(' ');

		if (splitter[0] == "help"){
			cout << "create + name \n";
			cout << "show (in terminal)\n";
			cout << "graph (visual debug)\n";
			cout << "checkout + branchname\n";
			cout << "newversion (add new version in current branch)\n";
			cout << "updatefile\n";
		}

		else if(splitter[0] == "create"){
			if(splitter.size() > 1){
				project = new CVS(splitter[1]);
				//project->NewBranch("master_v0","develop");
				//project->NewBranch("master_v0","testing");
				cout << "proyecto "<< splitter[1]<<" creado\n";
			}
			else{
				cout << "create recibe 2 parametros\n";
			}
		}

		else if(splitter[0] == "branch"){
			if(splitter.size() == 3){
				StateNode ** tmp;
				if(project->FindState(splitter[2],tmp)){
						project->NewBranch(splitter[2],splitter[1]);
						cout << "branch "<<splitter[1]<< " creado desde "<<splitter[2]<<endl;
				}
				else{
					cout << "estado "<<splitter[2]<<" no encontrado"<<endl;
				}
			}
			else if(splitter.size() == 2){
				project->NewBranch(project->branches[project->current_branch]->current_state->name,splitter[1]);
				cout << "branch "<< splitter[1]<<" creado\n";
			}
			else{
				cout << "branch recibe 2 parametros\n";
			}
		}
		else if(splitter[0] == "newversion"){
			if(splitter.size() == 1){
				project->branches[project->current_branch]->NewVersion();
				cout << "new version added"<<" creado\n";
			}
			else{
				cout << "branch recibe 1 parametro\n";
			}
		}

		else if(splitter[0] == "delete"){
			if(splitter.size() == 2){
				project->Delete(splitter[1]);
				
				cout << "deleted "<<splitter[1]<<"\n";
			}
			else{
				cout << "delete recibe 2 parametro\n";
			}
		}

		else if(splitter[0] == "restore"){
			if(splitter.size() == 2){
				project->Restore(splitter[1]);
				
				cout << "restored "<<splitter[1]<<"\n";
			}
			else{
				cout << "restore recibe 2 parametro\n";
			}
		}
		else if(splitter[0] == "updatefile"){
			if(splitter.size() == 1){
				project->branches[project->current_branch]->UpdateFile();
				cout << "files in current branch updated\n";
			}
			else{
				cout << "updatefile recibe 1 parametro\n";
			}
		}

		else if(splitter[0] == "currentstate"){
			if(splitter.size() == 1){
				cout << project->branches[project->current_branch]->current_state->name<<endl;
			}
			else{
				cout << "currentstate recibe solo 1 parametro\n";
			}
		}

		else if(splitter[0] == "checkout"){
			if(splitter.size() > 1){
				project->	CheckOut(splitter[1]);
				cout << "cambiado a branch "<< splitter[1]<<endl;
			}
			else{
				cout <<"current branch: "<< project->branches[project->current_branch]->name<<endl;
			}
		}

		else if(splitter[0] == "show"){
			project->ShowProject();
		}

		else if(splitter[0] == "graph"){
			project->GenDot2();
			system("dot graph.dot -o graph.png -Tpng");
		}

		else if (splitter[0] == "exit"){
			break;
		}
		else{
			cout <<splitter[0]<< " is not a valid command\n";
		}


	}
}

int main(int argc, char* argv[]){
	
	CVS * project = new CVS("project");
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();

	project->NewBranch("master_v5","devel");

	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	
	project->CheckOut("devel");

	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	
	project->NewBranch("testing1");
	project->NewBranch("testing2");	
	
	project->CheckOut("testing1");	
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	

	project->CheckOut("testing2");	
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	project->branches[project->current_branch]->NewVersion();
	
	project->CheckOut("testing1");
	project->MergeBranch("testing2");
	project->CheckOut("devel");
	project->MergeBranch("testing1");
	project->CheckOut("master");
    project->MergeBranch("devel");
    project->branches[project->current_branch]->UpdateFile();
	project->GenDot2();
	project->Delete("testing2_v6");

	project->CheckOut("testing2");
	cout << project->branches[project->current_branch]->current_state->origin[1]->active;
	project->GenDot2();


	system("dot graph.dot -o graph.png -Tpng");


	run(project);

    return 0;
}
