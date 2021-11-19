#include "../include/merkle_tree.h"

using namespace std;

void test()
{
    vector<Data> src = {{"1", 1}, {"2", 1}, {"3", 1}, {"4", 1}, {"5", 1}};
    MerkleTree* mt = new MerkleTree(src, true);
    Data d = {"5", 1};
    cout << mt->verifyByBloomFilter(mt->getRoot(), &d) << endl;
    delete mt;
}

int main(void)
{
    test();
    return 0;
}