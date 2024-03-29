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
#include <vector>

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

   			std::vector<Instruction*> instructionList;	//List of instructions
   			std::vector<Instruction*> instructionList2;	//List of instructions
   			std::vector<Instruction*> subList;	//List of instructions
			//keep doing analysis till no further changes
			do{
				changed = 0;	

				//Put each instruction into list			
				for(inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
	       				instructionList.insert(instructionList.end(), &*i);
	   			}

				//While still analyzing instructions
				while (!instructionList.empty()) {
					subList.clear();

					//Pull top most instruction
					Instruction *I1 = *instructionList.begin();
				     	instructionList.erase(instructionList.begin());
				
					//If there is an allocate instruction - find declartion of all variables
				     	if (I1->getOpcode()==26){	
						//Get all instructions that refer to this value
						for (Value::use_iterator UI = I1->use_begin(), UE = I1->use_end(); UI != UE; ++UI){
							 if (Instruction *I2 = dyn_cast<Instruction>(*UI)) {
								subList.insert(subList.begin(), I2);
							}
						}
					}

					//Go through uses of this variable
					ConstantInt* CI = NULL;
					while (!subList.empty()) {
						Instruction *I3 = *subList.begin();
				     		subList.erase(subList.begin());

						//Continue if given new values
						if(I3->getOpcode() == 28){ 

							Value *v = I3->getOperand(0);

							if((CI = dyn_cast<ConstantInt>(v))) {
								continue;
							}else{
								break;
							}
						}else if(I3->getOpcode() == 27){		//replace places with loads
							if (CI!=NULL){
								changed = 1;
								changes+=I3->getNumUses();
								I3->replaceAllUsesWith(CI);		//reaplce values where possible

								
								//remove deleted instructions from lists
								subList.erase(std::remove(subList.begin(), subList.end(), I3), subList.end());
								instructionList.erase(std::remove(instructionList.begin(), instructionList.end(), I3), instructionList.end());
								I3->eraseFromParent();		//remove instructions no longer needed

							}
						}else{
							break;		
						}

					}

				}	


				//Put each instruction into list			
				for(inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
		       			instructionList2.insert(instructionList2.end(), &*i);
		   		}

				//Get some analysis
	   			DataLayout *TD = getAnalysisIfAvailable<DataLayout>();
	   			TargetLibraryInfo *TLI = &getAnalysis<TargetLibraryInfo>();
	 
	   			while (!instructionList2.empty()) {
				    	Instruction *I4 = *instructionList2.begin();
				     	instructionList2.erase(instructionList2.begin());    // Get an element from the worklist...
	 
					//Now that made constants, perform constant folding where possible
	     				if (!I4->use_empty()){                 
	       					if (Constant *C = ConstantFoldInstruction(I4, TD, TLI)) {

			 				for (Value::use_iterator UI = I4->use_begin(), UE = I4->use_end(); UI != UE; ++UI){
			   					instructionList2.insert(instructionList2.end(), cast<Instruction>(*UI));
							}
		 
							// Replace all of the uses of a variable with uses of the constant.
							changes+=I4->getNumUses();
							I4->replaceAllUsesWith(C);
		 					changed = 1;
							// Remove the dead instruction.
							instructionList2.erase(std::remove(instructionList2.begin(), instructionList2.end(), I4), instructionList2.end());
							I4->eraseFromParent();
						}
					}
	 
	       			}
			}while(changed);			

			
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

