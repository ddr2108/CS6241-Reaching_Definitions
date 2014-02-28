#define DEBUG_TYPE "p11"
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
#include <set>

using namespace llvm;

namespace {
	
	struct p11 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p11() : FunctionPass(ID) {}

		//Run for each function
		virtual bool runOnFunction(Function &F){
			int changed;			
			int changes = 0;
			int count= -1;
			int count2 = 0;

			//Get loopinfo
  			LoopInfo &LI = getAnalysis<LoopInfo>();

			std::vector<Instruction*> instructionLists;
			std::vector<StringRef> def;
			std::vector<unsigned> lineNum;
			std::vector<unsigned> realLineNum;
			std::set<Loop*> loopsVisited;
			std::vector<StringRef> bbList;

			//Put each instruction into list			
			for(inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
				count++;
				instructionLists.insert(instructionLists.end(), &*i);
				bbList.insert(bbList.end(), i->getParent()->getName());
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
			
			int* reachDef = (int*)calloc((count+1)*count2,sizeof(int));
			
			int prevK = 0;
			int k=0;
			int p = 0;
			//go through each instruction
			for(p = 0;p<=count;p++){
				Instruction* i = instructionLists[p];
				prevK = k;
				
				//Get Line num
				for (k = 0; k<=count2;k++){
					if(lineNum[k]>p){
						k--;
						break;
					}
				}


				if (p>0){
					//Copy previous reach def
					for (int j = 0; j<count2;j++){
						if (reachDef[(p-1)*count2+j] == 1){
							reachDef[p*count2+j] = reachDef[(p-1)*count2+j]; 
						}
					}
				}
				//if it is hte last instruction in block
				if ((instructionLists[p]->isTerminator())){
					//Loop through scuccessors
					for (int r = 0; r<instructionLists[p]->getParent()->getTerminator()->getNumSuccessors();r++){

						BasicBlock *newBlock = instructionLists[p]->getParent()->getTerminator()->getSuccessor(r);
						Instruction* newInstr2 = newBlock->getFirstNonPHI();
						//First instruction in basic block should have the reach defenitions based on the 
						for (int y = 0; y<=count;y++){
							//Find the instruction in list
							if(instructionLists[y]==newInstr2 && bbList[y]==newBlock->getName())							{
 								//Start copying
								for (int j = 0; j<count2;j++){
errs()<<"asd\n"; 
										if (reachDef[p*count2+j] == 1){

											reachDef[y*count2+j]= reachDef[p*count2+j]; 
										}
								}
							}
						}
					}
				
				}

				
				//Add new defitions reachable
				for (int m = prevK+1; m<=k;m++){

					//if(reachDef[p*count2+m]==0){
						reachDef[p*count2+m]=1;
						//Check if need to get rid of def
						for (int z = count2; z>=0;z--){
							if (reachDef[p*count2+z]==1 && lineNum[z]!=p){

								if(def[m].equals_lower(def[z])){
									reachDef[p*count2+z]=0;
								}
							}
						}
					//}
				}

				//Loop for this instruction
				Instruction* newInstr = instructionLists[p];
				BasicBlock* newBlock = newInstr->getParent();
				Loop* newLoop = LI.getLoopFor(newBlock);
				//Keep track of loops to prevent doing infinite
				if (newLoop!=NULL && loopsVisited.find(newLoop) == loopsVisited.end()){
					//Next instruction Loop
					Instruction* newInstr2 = instructionLists[p+1];
					BasicBlock* newBlock2 = newInstr2->getParent();
					Loop* newLoop2 = LI.getLoopFor(newBlock2);
					//If the two instructions are in different loops
					if (newLoop2!=newLoop){
						//go through previous instructions
						for (int a = 0; a<p;a++){
							Instruction* newInstr3 = instructionLists[a];
							BasicBlock* newBlock3 = newInstr3->getParent();
							Loop* newLoop3 = LI.getLoopFor(newBlock3);
							//if it is in the loop, the copy reaching defention from this end to the beginning
							if (newLoop3==newLoop){
								loopsVisited.insert(newLoop);
								for (int b = 0; b<count2;b++){
									if(reachDef[p*count2+b]==1){
										reachDef[a*count2+b]=1;
									} 
								}
								//in case loop skipped
								for (int b = 0; b<count2;b++){
									if(reachDef[a*count2+b]==1){
										reachDef[(p+1)*count2+b]=1;
									} 
								}


								//Reset iterative where change was made
								p=a--;

								//Get Line num
								for (k = 0; k<count2;k++){
									if(lineNum[k]>p){
										k--;
										break;
									}
								}
							
								break;
	
							}
						}
					}
				}
			}

			for (int z = 0; z<=count;z++){
				errs()<<*instructionLists[z]<<"\n";
				for (int y = 0; y<count2; y++){
					if (reachDef[z*count2+y]==1){
						errs()<<"-------"<<realLineNum[y]<<"-"<<def[y]<<"\n";
					}
				}
			}

			return 1;

		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
			AU.setPreservesCFG();
			AU.addRequired<TargetLibraryInfo>();
		  	AU.addRequired<LoopInfo>();		//add request	
		}

	};
}
char p11::ID = 0;
static RegisterPass<p11> X("p11", "Part 1.1", true, true);


