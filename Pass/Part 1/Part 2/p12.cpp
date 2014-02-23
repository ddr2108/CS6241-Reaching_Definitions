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
			int* reachDef = (int*)calloc(count*count2,sizeof(int));
			/////////////////////////////////////////////////////////////
			int ind;			
			int i,j;
			int *dist;
			BasicBlock* successorBlock;

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

			//Perform the algorithm
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

			Instruction* newInstr = instructionLists[1];
			BasicBlock* newBlock = newInstr->getParent();
			for (int z = 0; z<=count;z++){
				for (int x = 0; x<lineNum.size(); x++){
					int y = lineNum[x];
					if (y<=z){
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
						if ((ind1==ind2)||(dist[ind1*ind+ind2]>0 && dist[ind1*ind+ind2]<1000000000)){	
							reachDef[z*count2+x]=1;
						}

					}else{
						break;				
					}
					
				}
			}
			
			//Fix duplicates
			for (int p = 0; p<=count; p++){
				for (int m = 0; m<count2; m++){
					for(int n = 0; n<m; n++){
						if (def[n].equals_lower(def[m]) && reachDef[p*count2+m]==1 && reachDef[p*count2+n]==1){
							//M block
							Instruction* newInstr1 = instructionLists[lineNum[m]];
							BasicBlock* newBlock1 = newInstr1->getParent();
							
							//N block
							Instruction* newInstr2 = instructionLists[lineNum[n]];
							BasicBlock* newBlock2 = newInstr2->getParent();
							
							//P block
							Instruction* newInstr3 = instructionLists[p];
							BasicBlock* newBlock3 = newInstr3->getParent();

							//if in the same block
							if ((newBlock1==newBlock2) && !(LI.getLoopFor(newBlock1)==LI.getLoopFor(newBlock2) && LI.getLoopFor(newBlock2)!=NULL)){
								reachDef[p*count2+n]=0;		//remove earlier def
							}
							
							if ((newBlock1==newBlock2) && (LI.getLoopFor(newBlock1)==LI.getLoopFor(newBlock2) && LI.getLoopFor(newBlock2)!=NULL)){
								reachDef[p*count2+n]=0;		//remove earlier def
							}
							
							//if in different blocks
							if (newBlock1!=newBlock2){
								if (PDT.dominates(newBlock1, newBlock2)){
									reachDef[p*count2+n]=0;		//remove earlier def
								}
								if (newBlock3==newBlock1){
									reachDef[p*count2+n]=0;		//remove earlier def
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

			exit(3);
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

