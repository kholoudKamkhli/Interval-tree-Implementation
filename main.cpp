#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;

class Node
{
public:
    int Low;
    int High;
    int M;
    Node *Right;
    Node *Left;
    Node(int low,int high)
    {
        Low= low;
        High = high;
        Right=NULL;
        Left= NULL;
    }
    bool isLeaf()
    {
        return (this->Left==NULL and this->Right==NULL);
    }

};
class IntervalTree
{
public:
    Node * Root;
    IntervalTree()
    {
        Root=NULL;
    }
    Node **getInorderSuccessor(Node ** node)
    {
        while ((*node) and (*node)->Left!=NULL)
            node=&((*node)->Left);
        return node;
    }
    Node * insert(Node *& node)
    {
        return insert(Root,node);
    }
    Node * insert(int low, int high)
    {
        Node *node= new Node(low,high);
        return insert(Root,node);
    }
    Node * insert(Node*& root , Node *& node)
    {
        if(root==NULL)
        {
            root= node;
            root->M= node->High;
            return root;
        }

        if(node->Low< root->Low)
        {
            root->Left =insert(root->Left, node);
        }
        if(node->Low> root->Low)
        {
            root->Right =insert(root->Right,node);
        }
        if(node->Low == root->Low and node->High >root->High )
        {
            root->High=node->High;
        }
        if(root->M < node->High)
            root->M=node->High;
        return root;
    }
    void inorder(Node* root)
    {
        if (root == nullptr) {
            return;
        }

        inorder(root->Left);
        cout << root->Low << " "<<root->High<<endl;
        inorder(root->Right);
    }
    bool isEmpty()
    {
        return (this->Root==NULL);
    }
    //function to delete node from the tree
    void deleteNode( Node *& node)
    {
        //check if the node is a leaf node int the tree
        if(node->isLeaf())
        {
            //make the node equal to null value and return
            node=NULL;
            return;
        }
        //if the node's left is null then it has only one child and this child will take the place of the current node and the current node will be delete
        else if(node->Left==NULL)
        {
            // here we save the child in temp node
            Node *temp= node->Right;
            //make the node equal to it's child
            node =temp;
            return;
        }
        //if the node's left is null then it has only one child and this child will take the place of the current node and the current node will be delete
        else if(node->Right==NULL)
        {
            // here we save the child in temp node
            Node *temp= (node->Left);
            //make the node equal to it's child
            node =temp;
            return;
        }

        // if we reach here then the node has 2 childs, so we get the successor of the node
        Node **temp= getInorderSuccessor(&(node->Right));
        //make the node equal to its successor values(low,high,Max)
        node->High=(*temp)->High;
        node->Low=(*temp)->Low;
        node->M=(*temp)->M;
        if(!(*temp)->isLeaf()) {
            Node *temp1= (*temp)->Right;
            *temp=NULL;
            insert(temp1);

            return;
        }

        *temp=NULL;
    }
    //function to check if the interval overlaps with the tree's intervals
    bool checkOverlapping(Node*node , int low , int high){
        //condition to check overlapping
        if(node->Low<=high&&low<=node->High){
            //returns true if there is overlapping
            return true;
        }
        //returns false if there is no overlapping
        else return false;
    }
    int size=0;
    //function to search for all the intervals that overlap with the given interval
    void searchForInterval(Node*& root, int low, int high)
    {
        //if the tree is null then there is nothing to return
        if(root==NULL) {
            return;
        }
        // check overlapping with tree's node
        if(root != NULL && checkOverlapping(root,low,high))
        {
            size++;
            //print the overlapping interval
            cout<<"found "<<root->Low<<" "<<root->High<<endl;
            //store the node to delete it
            Node *temp= new Node(root->Low,root->High);
            //delete node that has the overlapping interval
            deleteNode(root);
            if(root !=NULL)
                //update the Max value of all nodes in tree after deletion
                updateMax(Root);
            searchForInterval(Root, low, high);
            //insert the nodes again after search is done
            insert(temp);
            return;
        }
        else if(size==0)
        {
            cout<<"NO Match found\n";
        }
        //if the left node's max is greater than the low of the current node then we need to search in the left tree
        if(root->Left!=NULL&&root->Left->M>=low)
        {
            searchForInterval(root->Left, low, high);
            return;
        }
            //if the left node's max is less than the low of the current node then we need to search in the right tree
            searchForInterval(root->Right, low, high);
            return;


    }
    //function to update the Max value of each interval in the tree after deletion
    void updateMax(Node *&node)
    {
        //if the node is a leaf node, then the max's value is equal to the high
        if(node->isLeaf())
        {
            node->M=node->High;
            return;
        }
        //if the node's right is not null then we need to update the max of right node and the current node's max will be equal to the max of current node max and right node's max
        if(node->Right!=NULL)
        {
            updateMax(node->Right);
            node->M=max(node->M, node->Right->M);
        }
        //if the node's left is not null then we need to update the max of left node and the current node's max will be equal to the max of current node max and left node's max
        if(node->Left!=NULL)
        {
            updateMax(node->Left);
            node->M=max(node->M, node->Left->M);
        }

    }


    friend class Node ;
};
int main() {
    IntervalTree tree;
    tree.insert(15,20);
    tree.insert(10,30);
    tree.insert( 17,19);
    tree.insert(5,20);
    tree.insert(12,15);
    tree.insert(30,40);
    tree.inorder(tree.Root);
    cout<<endl;
    tree.searchForInterval(tree.Root, 41, 47);
    cout<<endl;
    tree.searchForInterval(tree.Root, 50,51);
    cout<<endl;
    tree.searchForInterval(tree.Root, -100,11);
    cout<<endl;
    cout<<endl;


    IntervalTree tree2;
    tree2.insert(17,19);
    tree2.insert(5,8);
    tree2.insert( 4,8);
    tree2.insert(15,18);
    tree2.insert(7,10);
    tree2.insert(21,24);
    tree2.insert(16,22);
    tree2.inorder(tree2.Root);
    cout<<endl;
    tree2.searchForInterval(tree2.Root, 23 ,25 );
    cout<<endl;
    tree2.searchForInterval(tree2.Root, 1,2);
    cout<<endl;
    tree2.searchForInterval(tree2.Root, 7,10);
    cout<<endl;
    tree2.searchForInterval(tree2.Root, 3,12 );
    cout<<endl;
    tree2.searchForInterval(tree2.Root, 14,16 );
    cout<<endl;
    cout<<endl;


    IntervalTree tree3;
    tree3.insert(5, 10);
    tree3.insert(3, 8);
    tree3.insert( 10, 15);
    tree3.insert(16, 18);
    tree3.insert(9, 11);
    tree3.insert(1, 1);
    tree3.inorder(tree3.Root);
    cout<<endl;
    tree3.searchForInterval(tree3.Root, 0,5);
    cout<<endl;
    tree3.searchForInterval(tree3.Root, 5,10);
    cout<<endl;
    tree3.searchForInterval(tree3.Root, 10,20);
    cout<<endl;
    cout<<endl;



    IntervalTree tree4;
    tree4.insert(1,2);
    tree4.insert(3,5);
    tree4.insert( 6,7);
    tree4.insert(8,10);
    tree4.insert(12,16);
    tree4.insert(4,8);
    tree4.inorder(tree4.Root);
    cout<<endl;
    tree4.searchForInterval(tree4.Root, 0,5);
    cout<<endl;
    tree4.searchForInterval(tree4.Root, 6,20);
    cout<<endl;
    tree4.searchForInterval(tree4.Root, 4,2);
    cout<<endl;
    tree4.searchForInterval(tree4.Root,3,7);
    cout<<endl;
    tree4.searchForInterval(tree4.Root, 16,17);
    cout<<endl;
    cout<<endl;



    IntervalTree tree5;
    tree5.insert(1,5);
    tree5.insert(3,7);
    tree5.insert( 2,6);
    tree5.insert(10,15);
    tree5.insert(5,6);
    tree5.insert(4,100);
    tree5.inorder(tree5.Root);
    cout<<endl;
    tree5.searchForInterval(tree5.Root, 3,7);
    cout<<endl;
    tree5.searchForInterval(tree5.Root, 8,69);
    cout<<endl;
    tree5.searchForInterval(tree5.Root, 6,9);
    cout<<endl;
    cout<<endl;


    IntervalTree tree6;
    tree6.insert(16, 21);
    tree6.insert(8, 9);
    tree6.insert(25, 30);
    tree6.insert(5, 8);
    tree6.insert(15, 23);
    tree6.insert(17, 19);
    tree6.insert(26, 26);
    tree6.insert(0, 3);
    tree6.insert(6, 10);
    tree6.insert(19, 20);
    tree6.inorder(tree6.Root);
    cout<<endl;
    tree6.searchForInterval(tree6.Root, 200,300);
    cout<<endl;
    tree6.searchForInterval(tree6.Root, 15, 23);
    cout<<endl;
    cout<<endl;




    IntervalTree tree7;
    tree7.insert(-51, 11);
    tree7.insert(26, 68);
    tree7.insert(11, 100);
    tree7.insert(-97, 65);
    tree7.insert(-85, 18);
    tree7.insert(-31, -20);
    tree7.insert(-91, -6);
    tree7.insert(-17, 71);
    tree7.insert(-58, 37);
    tree7.insert(-50, -1);
    tree7.insert(11, 61);
    tree7.insert(6, 74);
    tree7.insert(13, 78);
    tree7.insert(-83, -62);
    tree7.insert(-80, 93);
    tree7.insert(-2, 84);
    tree7.insert(-62, -18);
    tree7.insert(-96, -53);
    tree7.insert(56, 91);
    tree7.insert(37, 79);
    tree7.inorder(tree7.Root);
    cout<<endl;
    tree7.searchForInterval(tree7.Root, 3,7);
    cout<<endl;
    tree7.searchForInterval(tree7.Root, 2, 7);
    cout<<endl;
    tree7.searchForInterval(tree7.Root, 0, 1);
    cout<<endl;
    cout<<endl;




    IntervalTree tree8;
    tree8.insert(5,8);
    tree8.insert(5,8);
    tree8.insert(5,8);
    tree8.insert(5,8);
    tree8.insert(5,8);
    tree8.insert(5,8);
    tree8.insert(5,8);
    tree8.inorder(tree8.Root);
    cout<<endl;
    tree8.searchForInterval(tree8.Root, 5,8);
    cout<<endl;
    tree8.searchForInterval(tree8.Root, 0,4);
    cout<<endl;
    cout<<endl;







    IntervalTree tree9;
    tree9.insert(0, 5);
    tree9.insert(5, 10);
    tree9.insert(10, 15);
    tree9.insert(15, 20);
    tree9.insert(16, 21);
    tree9.insert(8, 9);
    tree9.insert(25, 30);
    tree9.insert(5, 8);
    tree9.insert(15, 23);

    tree9.inorder(tree9.Root);
    cout<<endl;
    tree9.searchForInterval(tree9.Root, 77, 99  );
    cout<<endl;
    tree9.searchForInterval(tree9.Root, 5, 5);
    cout<<endl;
    tree9.searchForInterval(tree9.Root, 1000, 2000);
    cout<<endl;
    cout<<endl;





    IntervalTree tree10;
    tree10.insert(16, 21);
    tree10.insert(8, 9);
    tree10.insert(25, 30);
    tree10.insert(5, 8);
    tree10.insert(15, 23);
    tree10.insert(17, 19);
    tree10.insert(26, 26);
    tree10.insert(1000, 2000);
    tree10.insert(6, 10);
    tree10.insert(19, 20);
    tree10.inorder(tree10.Root);
    cout<<endl;
    tree10.searchForInterval(tree10.Root, 1000, 1001  );
    cout<<endl;
    cout<<endl;





    IntervalTree tree11;
    tree11.insert(20, 30);
    tree11.insert(40, 60);
    tree11.insert(70, 90);
    tree11.insert(60, 70);
    tree11.insert(40, 90);
    tree11.insert(80, 90);
    tree11.inorder(tree11.Root);
    cout<<endl;
    tree11.searchForInterval(tree11.Root, 50, 80  );
    cout<<endl;
    cout<<endl;







    IntervalTree tree12;
    tree12.insert(7, 8);
    tree12.insert(1, 4);
    tree12.insert(2, 3);
    tree12.insert(7, 12);
    tree12.insert(1, 1);
    tree12.insert(3, 4);
    tree12.insert(7, 7);
    tree12.insert(0, 2);
    tree12.insert(0, 2);
    tree12.insert(0, 3);
    tree12.insert(9, 12);
    tree12.inorder(tree12.Root);
    cout<<endl;
    tree12.searchForInterval(tree12.Root, 2, 3  );
    cout<<endl;
    cout<<endl;




    IntervalTree tree13;
    tree13.insert(2, 3);
    tree13.insert(0, 1);
    tree13.insert(0, 12);
    tree13.insert(0, 0);
    tree13.insert(0, 99);
    tree13.insert(5, 7);
    tree13.insert(6, 7);
    tree13.insert(13, 14);
    tree13.insert(5, 5);
    tree13.insert(5, 6);
    tree13.insert(6, 6);
    tree13.inorder(tree13.Root);
    cout<<endl;
    tree13.searchForInterval(tree13.Root, -10,-1  );
    cout<<endl;
    tree13.searchForInterval(tree13.Root, 0,5  );
    cout<<endl;
    tree13.searchForInterval(tree13.Root, 10,17 );
    cout<<endl;
    tree13.searchForInterval(tree13.Root, 21,69 );
    cout<<endl;
    cout<<endl;



    IntervalTree tree14;
    tree14.insert(5, 10);
    tree14.insert(4, 6);
    tree14.insert(10, 11);
    tree14.insert(5, 5);
    tree14.insert(10, 10);
    tree14.insert(12, 14);
    tree14.insert(0, 4);
    tree14.inorder(tree14.Root);
    cout<<endl;
    tree14.searchForInterval(tree14.Root, 1,1 );
    cout<<endl;
    tree14.searchForInterval(tree14.Root, 10,10);
    cout<<endl;
    tree14.searchForInterval(tree14.Root, 12,13);
    cout<<endl;
    cout<<endl;





    IntervalTree tree15;
    tree15.insert(15,20);
    tree15.insert(10,10);
    tree15.insert(30,20);
    tree15.insert(17,19);
    tree15.insert(30,5);
    tree15.insert(20,40);
    tree15.insert(12,15);
    tree15.insert(50,30);
    tree15.insert(40,25);
    tree15.inorder(tree15.Root);
    cout<<endl;
    tree15.searchForInterval(tree15.Root, 0,10 );
    cout<<endl;
    tree15.searchForInterval(tree15.Root, 5,20 );
    cout<<endl;
    tree15.searchForInterval(tree15.Root, 15,21);
    cout<<endl;
    tree15.searchForInterval(tree15.Root, 9,69 );
    cout<<endl;
    tree15.searchForInterval(tree15.Root, 6,9);
    cout<<endl;



    return 0;
}
