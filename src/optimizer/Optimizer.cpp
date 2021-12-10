#include <iostream>
#include "Optimizer.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/IPO.h"

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

  if (!(disableOpt=="clup")){
    // Simple loop unrolling
    TheFPM->add(createLoopUnrollPass());
  }

  auto ThePM = std::make_unique<legacy::PassManager>();

  // 30,000,000 runs, three times
  // 0.45 average over the three with optimization
  // 0.79 average over the three without optimization
  if (!(disableOpt=="dae")){
    ThePM->add(createDeadArgEliminationPass());
  }

  // 20,000,000 runs, three times
  // 0.52 average over the three with optimization
  // 0.54 average over the three without optimization
  if (!(disableOpt=="mfun")){
    ThePM->add(createMergeFunctionsPass());
  }

  // 1,000,000 runs, three times
  // 1.06333 average over the three with optimization
  // 1.07 average over the three without optimization
  if (!(disableOpt=="hcs")){
    ThePM->add(createHotColdSplittingPass());
  }

  // 30,000,000 runs, three times
  // 0.95 average over the three with optimization
  // 0.98 average over the three without optimization
  if (!(disableOpt=="pil")){
    ThePM->add(createPartialInliningPass());
  }

  // 1,000,000 runs, three times
  // 2.55 average over the three with optimization
  // 2.64 average over the three without optimization
  if (!(disableOpt=="funinline")){
    ThePM->add(createFunctionInliningPass());
  }

  // This made it slower by about 0.05 seconds over 50,000,000 runs
  /*if (!(disableOpt=="constprop")){
    ThePM->add(createIPSCCPPass());
  }*/

  // initialize and run simplification pass on each function
  TheFPM->doInitialization();
  for (int i=0; i<rounds; i++) {
    for (auto &fun : theModule->getFunctionList()) {
      TheFPM->run(fun);
    }
  }

  // run the IPO passes 
  ThePM->run(*theModule);
}
