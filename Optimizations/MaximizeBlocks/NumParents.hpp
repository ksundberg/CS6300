#ifndef CS6300_NUMPARENTS_HPP
#define CS6300_NUMPARENTS_HPP

#include <memory>
#include <map>

#include "../../AST_INC/AST/BasicBlock.hpp"

namespace cs6300{
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

}
#endif
