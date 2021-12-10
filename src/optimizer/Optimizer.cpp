#include "Optimizer.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include <iostream>

using namespace llvm;

void Optimizer::optimize(Module* theModule, std::string disableOpt, int rounds, bool funcDis, bool moduleDis) {
  // Create a pass manager to simplify generated module
  auto TheFPM = std::make_unique<legacy::FunctionPassManager>(theModule);

  if (!funcDis){
    if (!(disableOpt=="cpmtrp")){
      // Promote allocas to registers.
      TheFPM->add(createPromoteMemoryToRegisterPass());
    }

    if (!(disableOpt=="cicp")){
      // Do simple "peephole" optimizations
      TheFPM->add(createInstructionCombiningPass());
    }

    if (!(disableOpt=="crp")){
      // Reassociate expressions.
      TheFPM->add(createReassociatePass());
    }

    if (!(disableOpt=="cgp")){
      // Eliminate Common SubExpressions.
      TheFPM->add(createGVNPass());
    }

    if (!(disableOpt=="ccsp")){
      // Simplify the control flow graph (deleting unreachable blocks, etc).
      TheFPM->add(createCFGSimplificationPass());
    }

    if (!(disableOpt=="clicm")){
      // loop invariant code motion
      TheFPM->add(createLICMPass());
    }

    if (!(disableOpt=="cldp")){
      //deletes dead finite loops
      TheFPM->add(createLoopDeletionPass());
    }

    //-ru option is included to disable both loop rotating and unrolling, as they often work in concert
    if(!((disableOpt=="clrp") || (disableOpt=="ru"))){
      //rotates loop into do-while style
      TheFPM->add(createLoopRotatePass());
    }

    if(!((disableOpt=="clup") || (disableOpt=="ru"))){
      //unrolls loops 
      TheFPM->add(createLoopUnrollPass());
    }

    if(!(disableOpt=="sink")){
      //sinks common instructions into lowest successor
      TheFPM->add(createSinkingPass());
    }
  }

  if (!moduleDis){
    //module passes go here
  }

  // initialize and run simplification pass on each function
  TheFPM->doInitialization();
  for (int i=0; i<rounds; i++) {
    for (auto &fun : theModule->getFunctionList()) {
      TheFPM->run(fun);
    }
  }
}