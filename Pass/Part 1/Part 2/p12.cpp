#define DEBUG_TYPE "p12"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Pass.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Target/TargetLibraryInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/DebugInfo.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include <set>

using namespace llvm;

namespace {
	
	struct p12 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p12() : FunctionPass(ID) {}

		//Run for each function
		virtual bool runOnFunction(Function &F){

			//////////////////////////////////////////////////////////////////
			int changed;			
			int changes = 0;
			int count= -1;
			int count2 = 0;

			std::vector<Instruction*> instructionLists;
			std::vector<StringRef> def;
			std::vector<int> lineNum;
			std::vector<unsigned> realLineNum;
			//Put each instruction into list			
			for(inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
				count++;
				instructionLists.insert(instructionLists.end(), &*i);
				
				//Get line number
				unsigned Line;
				MDNode *N = i->getMetadata("dbg");
				if (N) {
					DILocation Loc(N);                     
					 Line = Loc.getLineNumber();
				}
				//Store data about variables in list - include line number, variable name, and actual instruction
				if (i->getOpcode()==28 && N && i->getOperand(1)->getName()!=""){
					def.insert(def.end(), i->getOperand(1)->getName());
					lineNum.insert(lineNum.end(), count);
					realLineNum.insert(realLineNum.end(), Line);
					count2++;
				}else if (N && i->getName()!=""){
					def.insert(def.end(), i->getName());
					lineNum.insert(lineNum.end(), count);
					realLineNum.insert(realLineNum.end(), Line);
					count2++;
				}
   			}
			int* reachDef = (int*)calloc((count+1)*count2,sizeof(int));		//2d array of instructions and possible reach
			/////////////////////////////////////////////////////////////
			int ind;			
			int i,j;
			int *dist;
			BasicBlock* successorBlock;

			//Perform distance algorithm to find the distance between all the blocks

			//Get list of basic blocks			
			Function::BasicBlockListType &allblocks = F.getBasicBlockList();
			//Go through basic blocks
			ind = 0;			
			for (Function::iterator newBlock = allblocks.begin(); newBlock != allblocks.end(); newBlock++) {
				ind++;
			}
			//Allocate array
			dist = (int*) malloc(ind*ind*sizeof(int*));
			for (i = 0; i<ind; i++){
				//Init to infinite
				for(j =0; j<ind; j++){
					if (i==j){
						dist[i*ind + j] = 0;
					}else{
						dist[i*ind + j] = 1000000000;
					}
				}
			}

			//Fill array will all with distance 1;
			i = 0;
			for (Function::const_iterator firstBlock = allblocks.begin(); firstBlock != allblocks.end(); firstBlock++) {
				for (int k = 0; k < (firstBlock->getTerminator())->getNumSuccessors();k++){
					successorBlock = (firstBlock->getTerminator())->getSuccessor(k);	//Successor
					j =0;
					for (Function::const_iterator secondBlock = allblocks.begin(); secondBlock != allblocks.end(); secondBlock++) {	
						//Check if block is right
						if (successorBlock == secondBlock){
							dist[i*ind + j] = 1;
						}
						j++;
					}
				}
				i++;
			}

			//Perform the algorithm to find shortest distance
			for (int x = 0; x<ind ;x++){
				for (int y = 0; y<ind ;y++){
					for (int z = 0; z<ind ;z++){
						if (y!=z){

							if (dist[y*ind + z] > (dist[y*ind + x] + dist[x*ind + z])){ 
								dist[y*ind + z] = dist[y*ind + x] + dist[x*ind + z];
							}
						}
					}
				}
			}



			//////////////////////////////////////////////////////////////

      			PostDominatorTree& PDT = getAnalysis<PostDominatorTree>();
  			LoopInfo &LI = getAnalysis<LoopInfo>();
			DominatorTreeBase<BasicBlock> *dominatorTree;
			dominatorTree = new DominatorTreeBase<BasicBlock>(false);
			dominatorTree->recalculate(F);

			Instruction* newInstr = instructionLists[1];
			BasicBlock* newBlock = newInstr->getParent();
			for (int z = 0; z<=count;z++){					//go through instructions
				for (int x = 0; x<lineNum.size(); x++){			//go through defenitions
					int y = lineNum[x];				//FInd which insturction def is
					if (y>-1){
						//Y instruction
						Instruction* newInstr1 = instructionLists[y];
						BasicBlock* newBlock1 = newInstr1->getParent();
						int ind1 = 0;
						for (Function::const_iterator secondBlock = allblocks.begin(); secondBlock != allblocks.end(); secondBlock++) {
							if (secondBlock==(*newBlock1)){
								break;
							}
							ind1++;
						}	

						//Z instruction
						Instruction* newInstr2 = instructionLists[z];
						BasicBlock* newBlock2 = newInstr2->getParent();
						int ind2 = 0;
						for (Function::const_iterator thirdBlock = allblocks.begin(); thirdBlock != allblocks.end(); thirdBlock++) {
							if (thirdBlock==(*newBlock2)){
								break;
							}
							ind2++;
						}
						//Path from y to z?
						if ((ind1==ind2 && y<z)||(dist[ind1*ind+ind2]>0 && dist[ind1*ind+ind2]<1000000000)){		
							int flag = 0;
							//Weird edge case where variables are in the same block and also in loop
							if ((ind1==ind2 && y>z)){

								for (int q = x; q>=0; q--){
									if (lineNum[q]<=y && def[q].equals_lower(def[x])){
										if (instructionLists[lineNum[q]]->getParent()==newBlock){
											flag = 1;

										}
									}
								}
								if (flag==0){
									reachDef[z*count2+x]=1;
								}
							}else{
								reachDef[z*count2+x]=1;		//if there is a path, mark that there is a path
							}
						}

					}else{
						break;				
					}
					
				}
			}
			
			//Fix duplicates
			for (int p = 0; p<=count; p++){					//go through instructions (instruction studied)
				for (int m = 0; m<count2; m++){				//go through defs
					for(int n = 0; n<m; n++){			//go through defs reached already
						if (def[n].equals_lower(def[m]) && reachDef[p*count2+m]==1 && reachDef[p*count2+n]==1){
							//M block
							Instruction* newInstr1 = instructionLists[lineNum[m]];
							BasicBlock* newBlock1 = newInstr1->getParent();
							int ind1 = 0;
							for (Function::const_iterator firstBlock = allblocks.begin(); firstBlock != allblocks.end(); firstBlock++) {
								if (firstBlock==(*newBlock1)){
									break;
								}
								ind1++;
							}	
							
							//N block
							Instruction* newInstr2 = instructionLists[lineNum[n]];
							BasicBlock* newBlock2 = newInstr2->getParent();
							int ind2 = 0;
							for (Function::const_iterator secondBlock = allblocks.begin(); secondBlock != allblocks.end(); secondBlock++) {
								if (secondBlock==(*newBlock2)){
									break;
								}
								ind2++;
							}	
							
							//P block
							Instruction* newInstr3 = instructionLists[p];
							BasicBlock* newBlock3 = newInstr3->getParent();
							int ind3 = 0;
							for (Function::const_iterator thirdBlock = allblocks.begin(); thirdBlock != allblocks.end(); thirdBlock++) {
								if (thirdBlock==(*newBlock3)){
									break;
								}
								ind3++;
							}	
/*
							//if in the same block
							if ((newBlock1==newBlock2) && !(LI.getLoopFor(newBlock1)==LI.getLoopFor(newBlock2) && LI.getLoopFor(newBlock2)!=NULL)){
								reachDef[p*count2+n]=0;		//remove earlier def
							}
							
							//if ((newBlock1==newBlock2) && (LI.getLoopFor(newBlock1)==LI.getLoopFor(newBlock2) && LI.getLoopFor(newBlock2)!=NULL)){
							//	reachDef[p*count2+n]=0;		//remove earlier def
							//}
							
							if ((LI.getLoopFor(newBlock3)!=LI.getLoopFor(newBlock2) && LI.getLoopFor(newBlock3)!=NULL)){
								if (lineNum[n]<p && lineNum[m]<p)
									reachDef[p*count2+n]=0;		//remove earlier def
							}
							//Fix
							if ((LI.getLoopFor(newBlock1)==LI.getLoopFor(newBlock2) && LI.getLoopFor(newBlock2)!=NULL)){
								if (lineNum[n]<p && lineNum[m]>p){
									reachDef[p*count2+m]=0;		//remove earlier def
								}else if (lineNum[n]>p && lineNum[m]>p){
									//reachDef[p*count2+n]=0;		//remove earlier def
								}else if (lineNum[n]<p && lineNum[m]<p){
									reachDef[p*count2+n]=0;
								}		//remove earlier def
							}*/


							//Test for all kinds of cases
							if (p==lineNum[n]){			//We are comparing def to current instruct
								reachDef[p*count2+m]=0;		//remove other def	
							}else if (p==lineNum[m]){		//We are comparing def to current instruct
								reachDef[p*count2+n]=0;		//remove other def	
							}else if (newBlock1==newBlock2){
								//If same block
								//if same block as instruction being checked for reaching
								if (newBlock3==newBlock1){
									//Do tests based on line numbers to see what reach taht instrcution
									//keep instruction that occurs closest in time to studied instruction
									if (lineNum[n]<p && lineNum[m]>p){
										reachDef[p*count2+m]=0;		//remove 
									}else if (lineNum[n]>p && lineNum[m]>p){
										reachDef[p*count2+n]=0;   //remove
									}else if (lineNum[n]<p && lineNum[m]<p){
										reachDef[p*count2+n]=0;	//remove
									}	
								}else{
									reachDef[p*count2+n]=0;   //if not in same block as instruction studied
								}
							/*}
							else if (newBlock1==newBlock3){
								reachDef[p*count2+n]=0;   //if 
							}
							else if (newBlock1==newBlock3){
								reachDef[p*count2+m]=0;   //arlier def
							*/}else if (newBlock1!=newBlock2){
								//Do the domination chcks as per paper if blocks are different
								if (PDT.dominates(newBlock1, newBlock2) && PDT.dominates(newBlock3, newBlock1)){
									reachDef[p*count2+n]=0;		//remove earlier def
								}
								else if (PDT.dominates(newBlock2, newBlock1)){
									reachDef[p*count2+m]=0;		//remove earlier def
								}
								//If different blocks, try to remove earlier def
								else if (newBlock3==newBlock2 && lineNum[n]<=p){
									reachDef[p*count2+m]=0;		//remove earlier def
								}
								else if (newBlock3==newBlock1 && lineNum[m]<=p){
									reachDef[p*count2+n]=0;		//remove earlier def
								}else{
									//Checks to look for conditional statements in loops
									int countDist = 0;
									for (int s = 0; s<ind ;s++){
										if (dist[s*ind+ind3]==1){
											countDist++;
										}
									}
									//Try to reverse path to figure out which things to delete
									if (countDist==1){
										if (dist[ind1*ind+ind3]==1){
											reachDef[p*count2+n]=0;		//remove earlier def
										}else if (dist[ind2*ind+ind3]==1){
											reachDef[p*count2+m]=0;		//remove earlier def
										}
									}
								}

							
							}

						}
					}
				}
			}

			///////////////////////////////////////////////////////////////

			//Print Data
			for (int z = 0; z<=count;z++){
				errs()<<*instructionLists[z]<<"\n";
				for (int y = 0; y<lineNum.size(); y++){
					if (reachDef[z*count2+y]==1){
						errs()<<"-------"<<realLineNum[y]<<"-"<<def[y]<<"\n";
					}
				}
			}

			//exit(3);
			return 1;

		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
			AU.setPreservesCFG();
			AU.addRequired<TargetLibraryInfo>();
		  	AU.addRequired<LoopInfo>();		//add request	
      			AU.addRequired<PostDominatorTree>();	//init request
		}

	};
}
char p12::ID = 0;
static RegisterPass<p12> X("p12", "Part 1.2", true, true);

