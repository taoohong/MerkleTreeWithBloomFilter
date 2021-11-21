#include "../include/merkle_tree.h"
#include <sys/time.h>
#include <assert.h>
#include <random>

using namespace std;

#define MAX_STR_LEN 100
#define TEST_SET_NUM 10000

random_device rd;
mt19937 random(rd());

string strRand() {			// length: 产生字符串的长度
    char tmp;							// tmp: 暂存一个随机数
    string buffer;						// buffer: 保存返回值
    
    for (int i = 0; i < random()%MAX_STR_LEN; i++) {
        tmp = random() % 36;	// 随机一个小于 36 的整数，0-9、A-Z 共 36 种字符
        if (tmp < 10) {			// 如果随机数小于 10，变换成一个阿拉伯数字的 ASCII
            tmp += '0';
        } else {				// 否则，变换成一个大写字母的 ASCII
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    //cout << buffer << endl;
    return buffer;
}


void getMerkleProofTest()
{
    vector<Data*> src = {new Data{"1", 1}, new Data{"2", 1}, new Data{"3", 1}, new Data{"4", 1}, new Data{"5", 1}};
    MerkleTree* mt = new MerkleTree(src, true);
    Data d = {"5", 1};
    cout << "[INFO]: [";
    for(auto s:mt->getProof(&d))
    {
        cout << s << ", ";
    }
    cout << "]" << endl;
    delete mt;
}

void getBloomFilterVerificationTest()
{
    vector<Data*> src = {new Data{"1", 1}, new Data{"2", 1}, new Data{"3", 1}, new Data{"4", 1}, new Data{"5", 1}};
    MerkleTree* mt = new MerkleTree(src, true);
    Data d = {"7", 1};
    if(mt->verifyByBloomFilter(mt->getRoot(), &d))
    {
        cout << "[INFO]: verified success" << endl;
    }
    else cout << "[INFO]: verified failed" << endl;
    delete mt;
}

void efficienceTest() {
    vector<Data*> test(TEST_SET_NUM);

    for(int i = 0; i<TEST_SET_NUM; i++)
    {
        test[i] = new Data();
        test[i]->data = strRand();
        test[i]->size = test[i]->data.size();
    }
    MerkleTree *mt = new MerkleTree(test, true);
    
    Node *root = mt->getRoot();
   
    /* 程序开始之前计时start */
    struct timeval start_with_bloomfilter;
    struct timeval end_with_bloomfilter;
    unsigned long timer_with_bloomfilter;
    gettimeofday(&start_with_bloomfilter, NULL);
    for(int i=0; i<TEST_SET_NUM; i++) {
        mt->verifyByBloomFilter(mt->getRoot(), test[i]);
    }
    /* 程序块结束后计时end */
    gettimeofday(&end_with_bloomfilter, NULL);
    /* 统计程序段运行时间(unit is usec)*/
    timer_with_bloomfilter = 1000000 * (end_with_bloomfilter.tv_sec - start_with_bloomfilter.tv_sec) + end_with_bloomfilter.tv_usec - start_with_bloomfilter.tv_usec;

    /* 程序开始之前计时start */
    struct timeval start_without_bloomfilter;
    struct timeval end_without_bloomfilter;
    unsigned long timer_without_bloomfilter;
    gettimeofday(&start_without_bloomfilter, NULL);
    
    for(int i=0; i<TEST_SET_NUM; i++) {
        mt->getProof(test[i]);
    }
    /* 程序块结束后计时end */
    gettimeofday(&end_without_bloomfilter, NULL);
    /* 统计程序段运行时间(unit is usec)*/
    timer_without_bloomfilter = 1000000 * (end_without_bloomfilter.tv_sec - start_without_bloomfilter.tv_sec) + end_without_bloomfilter.tv_usec - start_without_bloomfilter.tv_usec;
    printf("[INFO]: verify data with bloomfilter takes %lf ms.\n", ((double)timer_with_bloomfilter)/1000);
    printf("[INFO]: verify data without bloomfilter takes %lf ms.\n", ((double)timer_without_bloomfilter)/1000);
    delete mt;
}

int main(void)
{
    getMerkleProofTest();
    cout << endl;
    getBloomFilterVerificationTest();
    cout << endl;
    efficienceTest();
    return 0;
}