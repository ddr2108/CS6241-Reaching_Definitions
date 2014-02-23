#define DEBUG_TYPE "p13"
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
	
	struct p13 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p13() : FunctionPass(ID) {}

		//Run for each function
		virtual bool runOnFunction(Function &F){
			int changed;			
			int changes = 0;
			int count= -1;
			int count2 = 0;

			//Get loopinfo
  			LoopInfo &LI = getAnalysis<LoopInfo>();

			std::vector<Instruction*> instructionLists;
			std::vector<StringRef> bbList;
			std::vector<StringRef> def;
			std::vector<unsigned> lineNum;
			std::vector<unsigned> realLineNum;
			std::set<Loop*> loopsVisited;
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

				if (p>0 ){
					//Copy previous reach def
					for (int j = 0; j<count2;j++){
						if (reachDef[(p-1)*count2+j] == 1){
							reachDef[p*count2+j] = reachDef[(p-1)*count2+j]; 
						}
					}
				}

				

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

				int change = 0;
	
				//Call Test
				Instruction* newInstr = instructionLists[p];
				BasicBlock *newBlock = newInstr->getParent();
				if (newInstr->isTerminator() && (((newBlock->getTerminator())->getNumSuccessors()>1)||newInstr->getOpcode()==2)){
					for (int x = 0; x<newBlock->getTerminator()->getNumSuccessors(); x++){
						BasicBlock* newBlock2 = newBlock->getTerminator()->getSuccessor(x);
						Instruction* newInstr2 = newBlock2->getFirstNonPHI();
						for (int y = 0; y<=count;y++){
							if(instructionLists[y]==newInstr2 && bbList[y]==newBlock2->getName()){
								for (int j = 0; j<count2;j++){

										if (reachDef[p*count2+j] == 1){
											if (reachDef[y*count2+j]!=1){ 
												reachDef[y*count2+j]= reachDef[p*count2+j]; 
												change = 1;
												}
										}
									}
									if (change==1){
										if (p>y){
											p = y;
										}
									}

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
				errs()<<*instructionLists[z]<<"-------"<<instructionLists[z]->getOpcode()<<"\n";
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
char p13::ID = 0;
static RegisterPass<p13> X("p13", "Part 1.1", true, true);

