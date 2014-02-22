#define DEBUG_TYPE "p21"
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
#include <set>

using namespace llvm;

namespace {
	
	struct p21 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p21() : FunctionPass(ID) {}

		//Run for each function
		virtual bool runOnFunction(Function &F){
			int changed;			
			int changes = 0;

   			std::set<Instruction*> instructionList;	//List of instructions
   			std::set<Instruction*> subList;	//List of instructions
			//keep doing analysis till no further changes
			do{
				changed = 0;	
		
				//Put each instruction into list			
				for(inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
	       				instructionList.insert(&*i);
	   			}

				//While still analyzing instructions
				while (!instructionList.empty()) {
					//Pull top most instruction
					Instruction *I1 = *instructionList.begin();
				     	instructionList.erase(instructionList.begin());
				
					//If there is an allocate instruction - find declartion of all variables
				     	if (I1->getOpcode()==26){	
						//Get all instructions that refer to this value
						for (Value::use_iterator UI = I1->use_begin(), UE = I1->use_end(); UI != UE; ++UI){
							 if (Instruction *I2 = dyn_cast<Instruction>(*UI)) {
								subList.insert(I2);
							}
						}
					}

					ConstantInt* CI = NULL;
					while (!subList.empty()) {
						Instruction *I3 = *subList.begin();
				     		subList.erase(subList.begin());

						if(I3->getOpcode() == 28){ 

							Value *v = I3->getOperand(0);

							if((CI = dyn_cast<ConstantInt>(v))) {
								continue;
							}else{
								break;
							}
						}else if(I3->getOpcode() == 27){
							if (CI!=NULL){
								changed = 1;
								changes+=I3->getNumUses();
								I3->replaceAllUsesWith(CI);
								subList.erase(I3);
								instructionList.erase(I3);
								I3->eraseFromParent();
							}
						}else{
							break;		
						}

					}
					subList.clear();
				}	
			}while(changed);			

			changed = 0;
			instructionList.clear();

			//Put each instruction into list			
			for(inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
	       			instructionList.insert(&*i);
	   		}

   			DataLayout *TD = getAnalysisIfAvailable<DataLayout>();
   			TargetLibraryInfo *TLI = &getAnalysis<TargetLibraryInfo>();
 
   			while (!instructionList.empty()) {
			    	Instruction *I4 = *instructionList.begin();
			     	instructionList.erase(instructionList.begin());    // Get an element from the worklist...
 
     				if (!I4->use_empty()){                 // Don't muck with dead instructions...
       					if (Constant *C = ConstantFoldInstruction(I4, TD, TLI)) {

		 				for (Value::use_iterator UI = I4->use_begin(), UE = I4->use_end(); UI != UE; ++UI){
		   					instructionList.insert(cast<Instruction>(*UI));
						}
	 
						// Replace all of the uses of a variable with uses of the constant.
						changes+=I4->getNumUses();
						I4->replaceAllUsesWith(C);
	 
						// Remove the dead instruction.
						instructionList.erase(I4);
						I4->eraseFromParent();
					}
				}
 
       			}

			
			//New Version
       			errs()<<"changes: "<< changes<<"===================================\n";
			for(inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
       				errs()<<*i<<'\n';
   			}

			return 1;

		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
			AU.setPreservesCFG();
			AU.addRequired<TargetLibraryInfo>();
		}

	};
}
char p21::ID = 0;
static RegisterPass<p21> X("p21", "Part 3.1", true, true);

