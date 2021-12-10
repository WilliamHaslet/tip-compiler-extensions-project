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
    //about .01s faster on licm test (0.53 vs 0.52 on 100000 iterations)
    //load.tip test: went from 0.36s unoptimized, to 0.15s optimized on 20000000 iterations
    //also showed significant instruction count decrease in base.tip
    if (!(disableOpt=="cpmtrp")){
      // Promote allocas to registers.
      TheFPM->add(createPromoteMemoryToRegisterPass());
    }

    //showed 0.04s improvement with relation to the big bang test we provided
    if (!(disableOpt=="cicp")){
      // Do simple "peephole" optimizations
      TheFPM->add(createInstructionCombiningPass());
    }

    //rep.tip test: showed decrease from 0.11s to 0.06s on 20000000 iterations
    if (!(disableOpt=="crp")){
      // Reassociate expressions.
      TheFPM->add(createReassociatePass());
    }

    if (!(disableOpt=="cgp")){
      // Eliminate Common SubExpressions.
      TheFPM->add(createGVNPass());
    }

    //these optimizations were being hidden by the ones below, but with respect to the initial 5 showed relatively good results
    //on unreach.tip with 20000000 iterations, it was 0.01s faster (0.06s vs 0.05s) when the optimization was applied
    if (!(disableOpt=="ccsp")){
      // Simplify the control flow graph (deleting unreachable blocks, etc).
      TheFPM->add(createCFGSimplificationPass());
    }

    //100000 iterations of licm.tip; 0.69s non-optimized, 0.53s optimized
    if (!(disableOpt=="clicm")){
      // loop invariant code motion
      TheFPM->add(createLICMPass());
    }

    //1000000 iterations of deadloop.tip; 0.26s non-optimized, 0.00s optimized
    //important note here is that along with doing their intended optimizations, the optimizations past this point will
    //sometimes eliminate dead loops due to rotation and unrolling
    //for this purpose, I tested the benefit of this pass with and without including the optimizations below this point
    if (!(disableOpt=="cldp")){
      //deletes dead finite loops
      TheFPM->add(createLoopDeletionPass());
    }

    //1000000 iterations of rotate.tip, 0.28s non-optimized, 0.14s optimized
    //-ru option is included to disable both loop rotating and unrolling, as they often work in concert
    if(!((disableOpt=="clrp") || (disableOpt=="ru"))){
      //rotates loop into do-while style
      TheFPM->add(createLoopRotatePass());
    }

    //1000000 iterations of unroll.tip, 0.16s non-optimized, 0.00s optimized
    if(!((disableOpt=="clup") || (disableOpt=="ru"))){
      //unrolls loops 
      TheFPM->add(createLoopUnrollPass());
    }

    //changes llvm bitcode, leads to less instructions (sink.tip)
    if(!(disableOpt=="sink")){
      //sinks common instructions into lowest successor
      TheFPM->add(createSinkingPass());
    }

    //in sink.tip, reduced number of basic blocks from 13 to 12 in perf() function; branching is expensive
    if(!(disableOpt=="jump")){
      //facilitates sinking of common expressions
      TheFPM->add(createJumpThreadingPass());
    }
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

  if (!moduleDis)
  {
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
    // 1.06 average over the three with optimization
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
  }

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
