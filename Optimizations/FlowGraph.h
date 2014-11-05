//
//  TraverseBasicBlock.h
//  cpsl
//
//  Created by Steve Goodrich on 10/10/14.
//
//

#ifndef __cpsl__TraverseBasicBlock__
#define __cpsl__TraverseBasicBlock__

#include <stdio.h>
#include <memory>
#include <functional>
#include <set>
#include <map>

#include "AST/BasicBlock.hpp"

namespace cs6300 {


std::map<std::shared_ptr<cs6300::BasicBlock>, std::vector<std::shared_ptr<cs6300::BasicBlock>>> buildParentMap(std::shared_ptr<cs6300::BasicBlock> flowGraph);

void traverseFlowGraph(std::shared_ptr<cs6300::BasicBlock> sourceBlock, std::function<bool(std::shared_ptr<cs6300::BasicBlock>)>  action);
}

#endif /* defined(__cpsl__TraverseBasicBlock__) */
