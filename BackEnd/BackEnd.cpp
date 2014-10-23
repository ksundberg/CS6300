#include "BackEnd.hpp"
#include "AST/Program.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "AST/StringTable.hpp"
#include <set>
#include <fstream>
#include <algorithm>

using namespace std;

namespace{

  typedef shared_ptr<cs6300::BasicBlock> sBBlock;
  typedef shared_ptr<cs6300::Program> sProgram;
  typedef pair<sBBlock, sBBlock> blockPair;
  typedef vector<shared_ptr<cs6300::Statement>> statements;

  std::set<std::shared_ptr<cs6300::BasicBlock>> seenBlocks ;

  void emitMIPS(std::shared_ptr<cs6300::BasicBlock> block, std::ofstream &fout)
  {

    if (!block || (seenBlocks.find(block) != seenBlocks.end()))
      return;
    seenBlocks.insert(block);
    fout << block->getLabel() << ":\t" << std::endl;
    for (auto &&i : block->instructions)
    {
      fout << i;
    }
    if (block->branchTo)
    {
      fout << "\tbne $" << block->branchOn << ", $zero, " << block->branchTo->getLabel()
          << std::endl;
    }
    if (block->jumpTo)
    {
      fout << "\tj " << block->jumpTo->getLabel() << std::endl;
    }

    emitMIPS(block->branchTo,fout);
    emitMIPS(block->jumpTo,fout);
  }
  void emitMIPS(std::pair<std::shared_ptr<cs6300::BasicBlock>,
      std::shared_ptr<cs6300::BasicBlock>> cfg,
      std::ofstream& fout)
  {
    seenBlocks.clear();
    emitMIPS(cfg.first,fout);
  }
}

set<sBBlock> allBlocks(blockPair b)
{
    set<sBBlock> all;
    vector<sBBlock> todo;
    auto at = b.first;

    do
    {
        if(todo.size())
        {
            at = todo.back();
            todo.pop_back();
        }

        while(at)
        {
            if(!all.count(at))
                all.insert(at);

            if(at->branchTo && !all.count(at->branchTo))
                todo.emplace_back(at->branchTo);

            at = at->jumpTo;
        }
    }while(todo.size());

    return all;
}

void writeStringTable(std::ofstream &fout){
  auto string_table = cs6300::StringTable::instance()->getTable();
  for (auto s : string_table){
    fout << "SL" << s.second << ": \t.asciiz " << s.first << std::endl;
  }

}

void cs6300::writeMIPS(std::shared_ptr<IntermediateRepresentationProgram> program, std::string filename)
{
    //for(auto&& b : allBlocks(program->main))
        //b->printInstructions();
    locRegAlloc(program->main);
    std::ofstream fout(filename);
    fout << ".text" << std::endl << ".globl main" << std::endl << "main:"
        << "\tla $gp, GA" << std::endl << "\tori $fp, $sp, 0" << std::endl;

    emitMIPS(program->main,fout);

    for(auto&& f:program->functions)
    {
        emitMIPS(f.second,fout);
    }

    fout << ".data" << std::endl;
    writeStringTable(fout);

    fout << ".align 2" << std::endl;
    fout << "GA:" << std::endl;
}

struct RegColorNode
{
    int color;
    set<RegColorNode*> nodes;
    set<int> cant;
};

void cs6300::locRegAlloc(blockPair b)
{
    int count = 0;

    for ( auto&& v : allBlocks(b))
        v->initSets();

    //propogate block meta
    bool change;
    do
    {
        change = false;
        for(auto&& v : allBlocks(b))
        {
            if(pushUp(v, v->jumpTo))
                change = true;

            if(pushUp(v, v->branchTo))
                change = true;
        }
    } while(change);

    map<int,RegColorNode*> nodes;
    for(auto&& cur : allBlocks(b))
    {
        auto t = regDeps(cur);
        auto s = set<set<int>>(t.begin(),t.end());
        for(auto& v : s)
        {
            for(int reg : v)
            {
                for(int nreg : v)
                {
                    if(!nodes.count(nreg)) nodes[nreg] = new RegColorNode{-1};
                    nodes[reg]->nodes.insert(nodes[nreg]);
                }
            }
        }
    }

    map<int,int> regRemap;
    //color nodes
    for(auto& p : nodes)
    {
        if(p.second->color != -1)
        {
            continue; //already colored
        }

        for(int i =8;; i++)
        {
            if(!p.second->cant.count(i))
            {
                p.second->color = i;
                regRemap[p.first] = i;
                //cout << "remapping " << p.first << " to " << i << endl;
                for(auto& n : p.second->nodes)
                {
                    n->cant.insert(i);
                }
                break;
            }
            if(i==26)
            {
                cerr << "Too many colors tried" << endl;
                break;
            }
        }
    }

    for(auto&& v : allBlocks(b))
    {
        v->remap(regRemap);
    }
}

vector<set<int>> cs6300::regDeps(sBBlock b)
{
    vector<set<int>> res;

    set<int> prop = b->m.alive;
    for(auto i=b->instructions.rbegin(); i!=b->instructions.rend(); i++)
    {
        auto m = BasicBlock::meta(*i);
        prop.insert(m.used.begin(), m.used.end());
        vector<int> t;
        std::set_difference(
                prop.begin(), prop.end(),
                m.dead.begin(), m.dead.end(),
                back_inserter(t));
        prop = set<int>(t.begin(), t.end());
        res.insert(res.begin(), prop);
    }

    return res;
}

set<int> cs6300::allReg(sBBlock b)
{
    set<int> res;

    set<int> prop = b->m.alive;
    for(auto i=b->instructions.rbegin(); i!=b->instructions.rend(); i++)
    {
        auto m = BasicBlock::meta(*i);
        res.insert(m.used.begin(), m.used.end());
        res.insert(m.dead.begin(), m.dead.end());
    }

    return res;
}

bool cs6300::pushUp(sBBlock parent, sBBlock child)
{
    if(!child) return false;

    vector<int> t;
    set_difference(
            child->m.alive.begin(), child->m.alive.end(),
            parent->m.dead.begin(), parent->m.dead.end(),
            back_inserter(t));

    int origSize = parent->m.alive.size();
    parent->m.alive.insert(t.begin(), t.end());

    return origSize != parent->m.alive.size();
}
