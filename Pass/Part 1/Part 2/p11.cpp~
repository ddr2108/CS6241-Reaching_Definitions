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
			int count= 0;
			int count2 = 0;

			//Get loopinfo
  			LoopInfo &LI = getAnalysis<LoopInfo>();

			std::vector<Instruction*> instructionLists;
			std::vector<StringRef> def;
			std::vector<unsigned> lineNum;
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
				if (i->getOpcode()==28 && N && i->getOperand(1)->getName()!=""){
					def.insert(def.end(), i->getOperand(1)->getName());
					lineNum.insert(lineNum.end(), Line);
					count2++;
				}else if (N && i->getName()!=""){
					def.insert(def.end(), i->getName());
					lineNum.insert(lineNum.end(), Line);
					count2++;
				}
   			}
			
			int* reachDef = (int*)calloc(count*count2,sizeof(int));
			
			int curCount = -1;
			int prevK = 0;
			int k=0;
			int p = 0;
			for(p = 0;p<count;p++){
				Instruction* i = instructionLists[p];
				prevK = k;
	
				curCount++;		//go to next instruction
				unsigned Line;
				MDNode *N = i->getMetadata("dbg");
				if (N) {
					DILocation Loc(N);                     
					 Line = Loc.getLineNumber();
				}		

				if (N){

					if (curCount>0 ){
						//Copy previous reach def
						for (int j = 0; j<count2;j++){
							if (reachDef[(curCount-1)*count2+j] == 1){
								reachDef[curCount*count2+j] = reachDef[(curCount-1)*count2+j]; 
							}
						}
					}

					//Get Line num
					for (k = 0; k<count2;k++){
						if(lineNum[k]>Line){
							k--;
							break;
						}
					}

					for (int m = k; m>prevK;m--){

						if(reachDef[curCount*count2+m]==0){
							reachDef[curCount*count2+m]=1;
							//Check if need to get rid of def
							for (int z = m-1; z>=0;z--){
								if (reachDef[curCount*count2+z]==1){
									if(def[m].equals_lower(def[z])){
										reachDef[curCount*count2+z]=0;
									}
								}
							}
						}else{
							break;
						}
					}




				}else if (curCount>0){		//if no line number, copy previous row

					for (int j = 0; j<count2;j++){
						reachDef[curCount*count2+j] = reachDef[(curCount-1)*count2+j]; 
					}
				}

				//Loop Test
				Instruction* newInstr = instructionLists[p];
				BasicBlock* newBlock = newInstr->getParent();
				Loop* newLoop = LI.getLoopFor(newBlock);
				if (newLoop!=NULL){
					//After
					Instruction* newInstr2 = instructionLists[p+1];
					BasicBlock* newBlock2 = newInstr2->getParent();
					Loop* newLoop2 = LI.getLoopFor(newBlock2);
					if (newLoop2!=newLoop){
						for (int a = 0; a<count;a++){
							Instruction* newInstr3 = instructionLists[a];
							BasicBlock* newBlock3 = newInstr3->getParent();
							Loop* newLoop3 = LI.getLoopFor(newBlock3);
							if (newLoop3==newLoop){
								for (int b = 0; b<count2;b++){
									if(reachDef[p*count2+b]==1){
										reachDef[a*count2+b]=1;
									} 
								}
errs()<<"asd\n";
								p=a;
								//break;
	
							}
						}
					}
				}
			}

/*
			//Loops
			for (int z = 0; z<count;z++){
				Instruction* newInstr = instructionLists[z];
				BasicBlock* newBlock = newInstr->getParent();
				Loop* newLoop = LI.getLoopFor(newBlock);
				if (newLoop!=NULL){
					//After
					Instruction* newInstr2 = instructionLists[z+1];
					BasicBlock* newBlock2 = newInstr2->getParent();
					Loop* newLoop2 = LI.getLoopFor(newBlock2);
					if (newLoop2!=newLoop){
						for (int a = 0; a<count;a++){
							Instruction* newInstr3 = instructionLists[a];
							BasicBlock* newBlock3 = newInstr3->getParent();
							Loop* newLoop3 = LI.getLoopFor(newBlock3);
							if (newLoop3==newLoop){
								for (int b = 0; b<count2;b++){
									if(reachDef[z*count2+b]==1){
										reachDef[a*count2+b]=1;
									} 
								}	
							}
						}
					}
				}
							
			}

*/
			for (int z = 0; z<count;z++){
				errs()<<*instructionLists[z]<<"\n";
				for (int y = 0; y< count2; y++){
					if (reachDef[z*count2+y]==1){
						errs()<<"-------"<<lineNum[y]<<"-"<<def[y]<<"\n";
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

