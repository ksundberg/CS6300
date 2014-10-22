#ifndef CS6300_MAXIMIZEBLOCKS_HPP
#define CS6300_MAXIMIZEBLOCKS_HPP

#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "../AST_INC/AST/BasicBlock.hpp"
#include "../AST_INC/AST/ThreeAddressInstruction.hpp"

namespace cs6300
{
    class NumParents {
    public:
      static std::shared_ptr<NumParents> instance(){
        if(!m_instance){
          m_instance = std::shared_ptr<NumParents>(new NumParents());
        }
        return m_instance;
      }
      void addParent(std::shared_ptr<BasicBlock> block);
      int getNumParents(std::shared_ptr<BasicBlock> block);
      void reset();
    private:
      static std::shared_ptr<NumParents> m_instance;
      std::map<std::string, int> parents;
      std::string getString(std::shared_ptr<BasicBlock> block);
    };

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

  void maximizeBlocks(std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>>);

  void traverse(std::shared_ptr<BasicBlock> block);
}


#endif

