#define DEBUG_TYPE "p3"
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
#include <set>

using namespace llvm;

namespace {
	
	struct p3 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p3() : FunctionPass(ID) {}

		//Run for each function
		virtual bool runOnFunction(Function &F){
			int refer = 0;			

   			std::set<Instruction*> instructionList;	//List of instructions
   			std::set<Instruction*> subList;	//List of instructions
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


						
						//If not definiton
						if(I3->getOpcode() != 28){ 
							//Get line number
							if (MDNode *N = I3->getMetadata("dbg")) {
								DILocation Loc(N);                     
								unsigned Line = Loc.getLineNumber();
								errs()<<Line<<"\n";
							}else{
								errs()<<*I3<<"\n";
							}
						}else if(I3->getOpcode() == 28 && I3->getOperand(0)==I1){
						//Get line number
							if (MDNode *N = I3->getMetadata("dbg")) {
								DILocation Loc(N);                     
								unsigned Line = Loc.getLineNumber();
								errs()<<Line<<"\n";
							}else{
								errs()<<*I3<<"\n";
							}
						}else{
							break;
						}

					}
					subList.clear();

			}
			
			return 1;

		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
			AU.setPreservesCFG();
		}

	};
}
char p3::ID = 0;
static RegisterPass<p3> X("p3", "Part 3", true, true);

