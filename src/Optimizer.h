#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "AST.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/raw_ostream.h"

class Optimizer{
public:
 llvm::SmallVector<AST *> optimize(Program *Tree);
};

#endif