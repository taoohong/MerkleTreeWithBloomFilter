#include "../include/merkle_tree.h"

using namespace std;

void test()
{
    vector<Data> src = {{"1", 1}, {"2",2}, {"3", 3}, {"4", 4}, {"5", 5}};
    MerkleTree* mt = new MerkleTree(src);
    Data d = {"5", 5};
    for(auto s : mt->getProof(&d))
    {
        cout << s << "  ";
    }
    delete mt;
}

int main(void)
{
    test();
    return 0;
}