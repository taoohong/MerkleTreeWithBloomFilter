#include "../include/merkle_tree.h"

MerkleTree::MerkleTree(vector<Data> datas, bool usingBF)
{
    this->usingBF = usingBF;
    this->leafs = generateLeafsList(datas);
    this->root = generateMerkleTree(this->getLeafs());
    cout << "merkle tree with " << totalSize << " nodes has been built..."  << endl;
}

MerkleTree::~MerkleTree()
{
    for(auto& node : leafs)
    {
        delete node;
    }
    cout << "merkle tree has been cleaned..." << endl;
};


Node* MerkleTree::createNode(Data data, Node* left, Node*right)
{
    Node* node = new Node();
    if (node == nullptr)
	{
		cout << "out of memory" << endl;
		exit(1);
	}
    node->left = left;
    node->right = right;
    node->hash = toolkit::MD5(data.data).hexdigest();
    //node->hash = data.data;
    return node;
}

Node* MerkleTree::createNode(string data, Node* left, Node*right)
{
    Node* node = new Node();
    if (node == nullptr)
	{
		cout << "out of memory" << endl;
		exit(1);
	}
    node->left = left;
    node->right = right;
    node->hash = toolkit::MD5(data).hexdigest();
    //node->hash = data;
    return node;
}

Node* MerkleTree::createNode(Node* origin_node)
{
    Node* node = new Node();
    if (node == nullptr)
	{
		cout << "out of memory" << endl;
		exit(1);
	}
    node->left = origin_node->left;
    node->right = origin_node->right;
    node->hash = origin_node->hash;
    node->bloom = origin_node->bloom;
    return node;
}

vector<Node*> MerkleTree::generateLeafsList(vector<Data> datas)
{
    int n = datas.size();
    vector<Node*>* leafs = new vector<Node*>(n);
    for(int i = 0; i<n; i++)
    {
        (*leafs)[i] = createNode(datas[i], NULL, NULL);
        if(usingBF)
        {
            (*leafs)[i]->bloom = (Bloom*)malloc(sizeof(Bloom));
            bloom_init((*leafs)[i]->bloom, bloom_entry, bloom_error);
            bloom_add((*leafs)[i]->bloom, datas[i].data.c_str(), datas[i].size);
        }
    }
    cout << "list of leafs has been created..." << endl;
    return *leafs;
}

Node* MerkleTree::generateMerkleTree(vector<Node*> leafs)
{
    int n = leafs.size();
    this->levels+=1;
    if(n == 1)
    {
        this->totalSize+=1;
        return leafs[0];
    }
    if(n%2) leafs.push_back(createNode(leafs[n-1]));
    n = n + (n % 2);
    int count = 0;
    vector<Node*> tmp_leafs(n/2);
    for(int i = 0; i<n; i+=2)
    {
        tmp_leafs[count] = createNode(leafs[i]->hash + leafs[i+1]->hash, leafs[i], leafs[i+1]);
        leafs[i]->isLeft = true;
        leafs[i+1]->isRight = true;
        tmp_leafs[count]->parent = nullptr;
        leafs[i]->parent = tmp_leafs[count];
        leafs[i+1]->parent = tmp_leafs[count];
        if(usingBF)
        {
            tmp_leafs[count]->bloom = (Bloom*)malloc(sizeof(Bloom));
            bloom_and(leafs[i+1]->bloom, leafs[i]->bloom, tmp_leafs[count]->bloom);
        }
        count ++;
    }
    this->totalSize+=n;
    return generateMerkleTree(tmp_leafs);
}

vector<string> MerkleTree::getProof(Data* data)
{
    if(root == NULL) return vector<string>();
    vector<string> proof;
    int n = leafs.size();
    string hash = toolkit::MD5(data->data).hexdigest();
    for(auto& node : leafs)
    {
        if(node->hash == hash)
        {
            Node* parent = node->parent;
            while(parent != NULL)
            {
                Node* other;
                if(node->isLeft) other = parent->right;
                else other = parent->left;
                proof.push_back(other->hash);
                parent = parent->parent; 
                node = parent;
            }
            return proof;
        }
    }
    return vector<string>();
}

bool MerkleTree::verifyByBloomFilter(Node* node, Data* data)
{
    if(NULL == data) return false;
    if(bloom_check(node->bloom, data->data.c_str(), data->size))
    {
        Node* left = node->left;
        Node* right = node->right;
        if(NULL == left && NULL == right)
        {
            return true;
        }
        else if(verifyByBloomFilter(left, data)) return true;
        else return verifyByBloomFilter(right, data);
    }
    return false;
}

