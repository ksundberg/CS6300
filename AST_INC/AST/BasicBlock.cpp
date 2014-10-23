#include "BasicBlock.hpp"
#include "SymbolTable.hpp"

#include <algorithm>
#include <iterator>

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::vector;
using std::to_string;
using cs6300::RegMeta;
using cs6300::BasicBlock;

string BasicBlock::getLabel()
{

  static size_t blockNumber = 0;
  if (label.empty())
    label = string("BB") + to_string(++blockNumber);
  return label;
}

void printSet(string str, set<int> s)
{
    if(!s.size()) return;

    cout << str << ":";

    for(auto i : s)
        cout << i << " ";
}

void BasicBlock::printInstructions(bool sets)
{
    for(auto i : instructions)
        cout << i << endl;

    if(branchOn)
        cout << " branchOn: " << branchOn << endl;

    if(sets)
    {
        printSet("dead", m.dead);
        printSet("used", m.used);
        printSet("alive", m.alive);
        if(m.dead.size() || m.used.size() || m.alive.size())
            cout << endl;
    }
}

void BasicBlock::initSets()
{
    m.clear();
    for(auto i : instructions)
    {
        auto t = meta(i);
        m.used.insert(t.used.begin(),t.used.end());
        m.dead.insert(t.dead.begin(),t.dead.end());
    }

    if(branchOn)
        m.dead.insert(branchOn);

    vector<int> t;
    set_difference(
            m.used.begin(), m.used.end(),
            m.dead.begin(), m.dead.end(),
            std::back_inserter(t));
    m.alive = set<int>(t.begin(), t.end());
}

RegMeta BasicBlock::meta(ThreeAddressInstruction tal)
{
    RegMeta m;
    switch(tal.op)
    {
        case ThreeAddressInstruction::LoadMemory:
            m.dead.insert(tal.dest);
            m.used.insert(tal.src1);
            break;
        case ThreeAddressInstruction::LoadMemoryOffset:
        case ThreeAddressInstruction::LoadValue: //LoadValue has constants
            m.dead.insert(tal.dest);
            break;
        case ThreeAddressInstruction::StoreMemory: //special case store memory dest is src1
            m.used.insert(tal.dest);
            m.used.insert(tal.src1);
            break;
        case ThreeAddressInstruction::AddValue:
            m.dead.insert(tal.dest);
            m.used.insert(tal.src1);
            break;
        case ThreeAddressInstruction::WriteStr: //none
            break;
        default:
            if(tal.dest)
                m.dead.insert(tal.dest);
            if(tal.src1)
                m.used.insert(tal.src1);
            if(tal.src2)
                m.used.insert(tal.src2);
    }

    return m;
}

void cs6300::BasicBlock::remap(std::map<int,int> m)
{
    for(auto& i : instructions)
    {
        switch(i.op)
        {
            case ThreeAddressInstruction::LoadMemoryOffset:
                if(i.dest && m.count(i.dest))
                    i.dest = m[i.dest];
                break;
            case ThreeAddressInstruction::LoadValue: //LoadValue has constants
                if(i.dest && m.count(i.dest))
                    i.dest = m[i.dest];
                break;
            case ThreeAddressInstruction::StoreMemory:
            case ThreeAddressInstruction::AddValue:
            case ThreeAddressInstruction::LoadMemory:
                if(i.dest && m.count(i.dest))
                    i.dest = m[i.dest];
                if(i.src1 && m.count(i.src1))
                    i.src1 = m[i.src1];
                break;
            case ThreeAddressInstruction::WriteStr: //none
                break;
            default:
                if(i.dest && m.count(i.dest))
                    i.dest = m[i.dest];

                if(i.src1 && m.count(i.src1))
                    i.src1 = m[i.src1];

                if(i.src2 && m.count(i.src2))
                    i.src2 = m[i.src2];
        }
    }
}
