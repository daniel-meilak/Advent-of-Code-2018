#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"../../Utils/utils.h"

struct node{
    int num_children;
    int num_metadata;

    int value = 0;

    std::vector<int> metadata;
};

// forward function declarations
int build_tree(std::vector<node> &tree, const std::vector<int> &input, int &pos);

int main(){

    // read input into vector of int.
    std::vector<int> input = input_to_int(read_input("input", " "));

    // vector of nodes
    std::vector<node> tree;

    // starting pos
    int pos = 0;

    // build tree from input and find value
    int value = build_tree(tree,input,pos);

    // sum all metadata entries
    int sum = 0;
    for (const node &n : tree){
        for (const auto &meta : n.metadata){
            sum += meta;
        }
    }

    std::cout << "Answer (part 1): " << sum   << std::endl;
    std::cout << "Answer (part 2): " << value << std::endl;

    return 0;
}

int build_tree(std::vector<node> &tree, const std::vector<int> &input, int &pos){

    node n;
    // set number of children and metadata
    n.num_children = input[pos];
    n.num_metadata = input[pos+1];

    // move position in input forward
    pos += 2;

    // vector for children values
    std::vector<int> children;

    // create node for children and keep track of their values
    for (int i=0; i<n.num_children; i++){
        children.push_back(build_tree(tree,input,pos));
    }

    // read metadata and advance position in input
    for (int i=0; i<n.num_metadata; i++){
        n.metadata.push_back(input[pos]);
        pos++;
    }

    // if node has no children, value is sum of metadata
    if (n.num_children==0){
        for (const auto &meta : n.metadata){
            n.value += meta;
        }
    }
    // else use metadata as index for value of children
    else {
        for (const auto &meta : n.metadata){
            int index = meta-1;
            if (index>=0 && index<n.num_children){
                n.value += children[index];
            }
        }
    }
    
    // add node to tree
    tree.push_back(n);

    // return value of current node
    return n.value;
}