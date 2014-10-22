#ifndef CS6300_BASIC_BLOCK_HPP
#define CS6300_BASIC_BLOCK_HPP

#include <memory>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "ThreeAddressInstruction.hpp"

namespace cs6300
{
struct RegMeta
{
    std::set<int> dead;
    std::set<int> used;
    std::set<int> alive;

    void clear()
    {
        dead.clear();
        used.clear();
        alive.clear();
    }
};

class BasicBlock
{

    public:
        std::vector<ThreeAddressInstruction> instructions;
        std::shared_ptr<BasicBlock> jumpTo;
        std::shared_ptr<BasicBlock> branchTo;
        std::vector<std::shared_ptr<BasicBlock>> parents;
        int branchOn;
        std::string getLabel();
        void printInstructions(bool sets=false);

        //Used for register allocation
        RegMeta m;
        void initSets();
        static RegMeta meta(ThreeAddressInstruction tal);
        void remap(std::map<int,int> m);
    private:
        std::string label;
};
}
#endif
