#define DEBUG_TYPE "p2"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

namespace {
	
	//Structure to hold stats about a function
	struct functionStats{
		int basicBlocks;
		int CFG;
		int singleLoop;
		int basicBlockLoop;
		int dominators;

		//Constructor
		functionStats(){
			basicBlocks = 0;
			CFG= 0;
			singleLoop= 0;
			basicBlockLoop = 0;
			dominators = 0;
		}
	};

	//Structure to hold individual stats about module
	struct individualStats{
		int min;
		int max;
		int total;
		int num;
		
		//Constructor
		individualStats(){
			min = 222222222;
			max = 0;
			total = 0;
			num = 0;
		}
		//Add new information
		void add(int newStat){
			//Comapre to min/max
			if (newStat>max){
				max = newStat;
			}
			if(newStat<min){
				min = newStat;
			}
			//Add new info
			total+=newStat;
			num++;
		}
		//Print information
		void print(){
			float avg = (float) total/num;
      		errs() << '\n';
			errs() << "min:" << min << '\n';
			errs() << "max:" << max << '\n';
			errs() << "avg:" << avg << '\n';
      		errs() << '\n';
		}

	};

	//Structure to hold stats about module
	struct moduleStats{
		individualStats basicBlocks;
		individualStats CFG;
		individualStats singleLoop;
		individualStats basicBlockLoop;
		individualStats dominators;	

		//Add other functions stats to module
		void add(functionStats newStats){
			basicBlocks.add(newStats.basicBlocks);
			CFG.add(newStats.CFG);
			singleLoop.add(newStats.singleLoop);
			basicBlockLoop.add(newStats.basicBlockLoop);
			dominators.add(newStats.dominators);	
		}
		//Print all information
		void print(){
			errs() << "Basic Blocks:";
			basicBlocks.print();
			errs() << "CFG:";
			CFG.print();
			errs() << "Single Loops:";
			singleLoop.print();
			errs() << "Basic Block Loops:";
			basicBlockLoop.print();
			
			errs() << "Dominators:";
			float avg = (float) dominators.total/basicBlocks.total;
      		errs() << '\n';
			errs() << "avg:" << avg << '\n';
      		errs() << '\n';
		}

		~moduleStats(){
			print();
		}
	};
	
	struct p2 : public FunctionPass {
		// Pass identification, replacement for typeid
		static char ID; 
		p2() : FunctionPass(ID) {}

		//Initialize moduleStats
		moduleStats fullStats;

		//Run for each function
		virtual bool runOnFunction(Function &F){

			functionStats newStats;		//hold stats about function
			
			//Get loopinfo
  			LoopInfo &LI = getAnalysis<LoopInfo>();

			//Get Dominator info
			DominatorTreeBase<BasicBlock> *dominatorTree;
			dominatorTree = new DominatorTreeBase<BasicBlock>(false);
			dominatorTree->recalculate(F);


			/////////////////////////////////DO BASIC BLOCK STUFF/////////////////////////////////////////////////
			//Get list of basic blocks			
			Function::BasicBlockListType &allblocks = F.getBasicBlockList();
			//Go through basic blocks
			for (Function::iterator newBlock = allblocks.begin(); newBlock != allblocks.end(); newBlock++) {
				//Count Basic Blocks
				newStats.basicBlocks++;

				//Check for basic block loops
				Loop* innerloop=LI.getLoopFor(newBlock);
				if(innerloop!=NULL){
					newStats.basicBlockLoop++;
				}

				//if is a loopheader, then count a loop
				if(LI.isLoopHeader(newBlock)==true){
					newStats.singleLoop++;
				}

				//Check for CFG
				newStats.CFG+=(newBlock->getTerminator())->getNumSuccessors();

				//Count Dominators
				for (Function::iterator repeatBlock = allblocks.begin(); repeatBlock != allblocks.end(); repeatBlock++) {
					if((dominatorTree->dominates(repeatBlock,newBlock))==true){
						newStats.dominators++;
					}
				}
			}


			//If there is actually a block, then add info				
			if (newStats.basicBlocks>0){
				fullStats.add(newStats);
			}

			return false; 
		}

		virtual void getAnalysisUsage(AnalysisUsage &AU) const {
		  AU.addRequired<LoopInfo>();		//add request
		}

	};
}

char p2::ID = 0;
static RegisterPass<p2> X("p2", "Part 2 Pass", true, true);

