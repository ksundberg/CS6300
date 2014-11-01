#ifndef CS6300_VISITEDBLOCKS_HPP
#define CS6300_VISITEDBLOCKS_HPP

#include <vector>

#include "../../AST_INC/AST/BasicBlock.hpp"

namespace cs6300 {
  class VisitedBlocks {
  public:
    static std::shared_ptr<VisitedBlocks> instance(){
      if(!m_instance){
        m_instance = std::shared_ptr<VisitedBlocks>(new VisitedBlocks());
      }
      return m_instance;
    }
    bool isVisited(std::shared_ptr<BasicBlock> block);
    void reset();
  private:
    static std::shared_ptr<VisitedBlocks> m_instance;
    std::vector<std::string> m_blocks;
  };
}

#endif
