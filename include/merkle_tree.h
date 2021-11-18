#ifndef _MERKLE_TREE_H_
#define _MERKLE_TREE_H_

#include <string>
#include <vector>
#include "md5.h"

using namespace std;

typedef struct data
{
    string data;
    size_t size;
}Data;

typedef struct node
{
    string hash;
    struct node* left;
    struct node* right;
    struct node* parent;
    bool isLeft;
    bool isRight;

    node():isLeft(false), isRight(false){}
}Node;


class MerkleTree
{
private:
    /* data */
    Node* root;
    vector<Node*> leafs;
    unsigned int levels = 0;
    size_t totalSize = 0;

    Node* createNode(Data data, Node* left, Node*right);

    Node* createNode(string s, Node* left, Node*right);

    Node* createNode(Node* origin_node);

    vector<Node*> generateLeafsList(vector<Data> datas);

    Node* generateMerkleTree(vector<Node*> leafs);
public:
    MerkleTree(vector<Data> datas);

    //setter
    void setRoot(Node* r){root = r;}
    void setLeafs(vector<Node*> &l){leafs = l;}
    void setLevels(unsigned int l){levels = l;}
    void setTotalSize(size_t s){totalSize = s;}

    //getter
    Node* getRoot() const{return root;}
    vector<Node*> getLeafs() const{return leafs;}
    unsigned int getLevels() const{return levels;}
    size_t getTotalSize() const{return totalSize;}

    /**
     * get merkle proof path
     * @param data
     **/ 
    vector<string> getProof(Data* data);
    ~MerkleTree();
};

#endif //!_MERKLE_TREE_H_
