#define DEBUG_TYPE "p31"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

namespace {
	
	struct p31 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p31() : FunctionPass(ID) {}

		//Run for each function
		virtual bool runOnFunction(Function &F){
			
					errs() << "asd\n";
			for (Value::use_iterator i = F.use_begin(), e = F.use_end(); i != e; ++i){
					errs() << "asd\n";
				if (Instruction *Inst = dyn_cast<Instruction>(*i)) {
					errs() << "F is used in instruction:\n";
					errs() << *Inst << "\n";
			  }
			}
			
			return false; 
		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
		}

	};
}

char p31::ID = 0;
static RegisterPass<p31> X("p31", "Part 3.1", true, true);

