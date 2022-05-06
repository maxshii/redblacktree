/*
 * Program to implement binary search tree.
 * By: Max Shi
 * 4/21/2022
 */

#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>
using namespace::std;

struct node
{
  int data;
  bool red;
  node* parent;
  node* child1;
  node* child2;
};

bool isNum(char input[]);
int charToInt(char input[]);
void print(node* root, int count);
void add(node* &root, node* &current, node* parent, int value);
void remove(node* &root, node* parent, int value);
node* search(node* &root, int value);
node* findSuccessorWithTwoChildren(node* &parent, node* child2);
void balance(node* n);
node* getUncle(node* n);
node* rotateLeft(node* n);
node* rotateRight(node* n);

int main()
{
  node* tree = NULL;
  while (true) {
    cout << "Instructions: ADD, READ, SEARCH, REMOVE, PRINT, or QUIT\n";
		char input[80];
		cin.getline(input, 80, '\n');

    if(strcmp(input, "SEARCH") == 0)
    {
      cout << "Enter value to search: ";
      cin.getline(input, 80, '\n');
      if (isNum(input) == true) { //if input is a number
        if(search(tree, charToInt(input)) != NULL)
        {
          cout << input << " is in tree.\n";
        }
        else
        {
          cout << input << " is not in tree.\n";
  
        }
      }
    }
    else if(strcmp(input, "REMOVE") == 0)
    {/* TO BE IMPLEMENTED
      cout << "Enter value to remove: ";
      cin.getline(input, 80, '\n');
      if (isNum(input) == true) //if input is a number
      { 
        remove(tree, tree, charToInt(input));
      }*/
    }
    else if(strcmp(input, "PRINT") == 0)
    {
      print(tree, 0);
    }
    else if(strcmp(input, "QUIT") == 0)
    {
      break;
    }
    else if(strcmp(input, "ADD") == 0)
    {
      cout << "Enter value to add: ";
      cin.getline(input, 80, '\n');
      if (isNum(input) == true) { //if input is a number
  			add(tree, tree, tree, charToInt(input));
        cout << "Inserted " << input << ".\n\n";
  		} 
      else
      {
        cout << "Invalid input.";
      }
    }
    else if(strcmp(input, "READ") == 0)
    {
      cout << "Enter file name: ";
      cin.getline(input, 80, '\n');

      fstream file;
      file.open(input);
      if (file.is_open()) {
        int num;
        while (file >> num) { //read in numbers from file
          add(tree, tree, tree, num);
          cout << "Inserted " << num << ".\n\n";

        }
      } else {
        cout << "Error opening file.\n";
      }
    }
  }
}

//checks if a char array contains only numbers. Source: heap project.
bool isNum(char input[]) {
	int index = 0;
	int nonDigitCount = 0;
	while (true) {
		if (input[index] == '\0') { //break at end of cstring
			break;
		} else if (isdigit(input[index]) == 0) { //count number of non digits
			nonDigitCount++;
		}
		index++;
	}

	if (nonDigitCount > 0) { //not a number if there are any non digits
		return false;
	} else {
		return true;
	}
}

//converts char array to int. Source: heap project
int charToInt(char input[]) {
	int index = 0;
	int intArr[80];
	while (true) { //finds length of input and converts each char to int
		if (input[index] == '\0') {
			break;
		} else {
			intArr[index] = input[index] - '0';
		}
		index++;
	}

	int num = 0;
	for (int i = 0; i < index; i++) { //multiplies each int in array by the appropriate power of 10 and sums
		num = num + intArr[i] * pow(10, (index - 1 - i));
	}
	return num;
}

//recursively print in reverse inorder: right, node, left, and adds newlines and spaces to display tree
void print(node* root, int count)
{
  if(root != NULL) 
  {
    print(root->child2, count+1);
    for(int i = 0; i < count; i++)
      {
        cout << "    ";
      }
    if(root->red == true)
    {
      cout << "RED: ";
    }
    else
    {
      cout << "BLACK: ";
    }
    cout << root->data << endl;
    print(root->child1, count+1);
  }
}

//adds a node with value
void add(node* &root, node* &current, node* parent, int value)
{
  if(root == NULL) //if at end of tree
  {
    //create node
    node* n = new node();
    n->data = value;
    n->red = false;
    n->parent = NULL;
    n->child1 = NULL;
    n->child2 = NULL; 
    root = n;

    
  }
  else if(current == NULL)
  {
    node* n = new node();
    n->data = value;
    n->red = true;
    n->parent = parent;
    n->child1 = NULL;
    n->child2 = NULL; 
    current = n;

    node* x = current;
    if(parent->red == true) 
    {
      if(getUncle(x) != NULL)
      {
        if(getUncle(x)->red == true) //uncle is red
        { 
          while(x->parent != NULL) //color parent and uncle black, grandpa red. Repeat for grandpa as x.
          {
            if(x->parent->parent != NULL)
            {
              x->parent->red = false;
              if(getUncle(x) != NULL)
              {    
                getUncle(x)->red = false;
              }
              parent->parent->red = true;
              x = parent->parent;
            }
            else
            { 
              break;
            }
          }
          if(root->red == true) //color root black if it is red
          {
            root->red = false;
          }
        }
      }
      else //uncle is black
      {
        if(x->parent == x->parent->parent->child1) //l: parent is left child of gp
        {
          if(x == x->parent->child1) //ll: x is left child of parent
          {
            if(root == x->parent->parent) //if head is changed, update head
            {
              x = rotateRight(x->parent->parent);
              root = x;
              root->parent = NULL;
            }
            else
            {
              node* ggp = parent->parent->parent;
              if(ggp->child1 == parent->parent) //grandparent is greatgp's left child
              {
                x = rotateRight(x->parent->parent);

                //connect rotated subtree's root to the parent of prerotation subtree's root
                x->parent = ggp;
                ggp->child1 = x;
              }
              else //grandparent is greatgp's right child
              {
                x = rotateRight(x->parent->parent);
                x->parent = ggp;
                ggp->child2 = x;
              }
              
            }
            //swap colors of parent and grandparent of x before rotation
            bool color = x->red;
            x->red = x->child2->red;
            x->child2->red = color;
          }
          else //lr: x is right child of parent
          {
            if(root == x->parent) //if head is changed, update head
            {
              x = rotateLeft(x->parent);
              root = x;
              root->parent = NULL;

            }
            else
            {
              node* gp = parent->parent;
              if(gp->child1 == parent) //parent is gp's left child
              {
                x = rotateLeft(x->parent);

                //connect rotated subtree's root to the parent of prerotation subtree's root
                x->parent = gp;
                gp->child1 = x;
              }
              else //parent is gp's right child
              {
                x = rotateLeft(x->parent);
                x->parent = gp;
                gp->child2 = x;
              }
              
            }
            x = x->child1;
            //ll case
            if(root == x->parent->parent) //if head is changed, update head
            {
              x = rotateRight(x->parent->parent);
              root = x;
              root->parent = NULL;
            }
            else
            {
              node* ggp = parent->parent->parent;
              if(ggp->child1 == parent->parent) //grandparent is greatgp's left child
              {
                x = rotateRight(x->parent->parent);

                //connect rotated subtree's root to the parent of prerotation subtree's root
                x->parent = ggp;
                ggp->child1 = x;
              }
              else //grandparent is greatgp's right child
              {
                x = rotateRight(x->parent->parent);
                x->parent = ggp;
                ggp->child2 = x;
              }
              
            }
            //swap colors of parent and grandparent of x before rotation
            bool color = x->red;
            x->red = x->child2->red;
            x->child2->red = color;
          }
        }
        else //r: parent is right child of grandparent
        {
          if(x == x->parent->child2) //rr: x is right child of parent
          {
            if(root == x->parent->parent) //if head is changed, update head
            {
              x = rotateLeft(x->parent->parent);
              root = x;
              root->parent = NULL;
            }
            else //head is not changed
            {
              node* ggp = parent->parent->parent;
              if(ggp->child1 == parent->parent) //grandparent is greatgp's left child
              {
                x = rotateLeft(x->parent->parent);

                //connect rotated subtree's root to the parent of prerotation subtree's root
                x->parent = ggp;
                ggp->child1 = x;
              }
              else //grandparent is greatgp's right child
              {
                x = rotateLeft(x->parent->parent);
                x->parent = ggp;
                ggp->child2 = x;
              }
              
              
            }
           //swap colors of parent and grandparent of x before rotation
            bool color = x->red;
            x->red = x->child1->red;
            x->child1->red = color;
          }
          else //rl: x is right child of parent
          {
            if(root == x->parent) //if head is changed, update head
            {
              x = rotateRight(x->parent);
              root = x;
              root->parent = NULL;
            }
            else
            {
              node* gp = parent->parent;
              if(gp->child1 == parent) //parent is gp's left child
              {
                x = rotateRight(x->parent);
                //connect rotated subtree's root to the parent of prerotation subtree's root
                x->parent = gp;
                gp->child1 = x;
              }
              else //parent is gp's right child
              {
                x = rotateRight(x->parent);
                x->parent = gp;
                gp->child2 = x;
              }
            }

            x = x->child2;
            //rr case
           if(root == x->parent->parent) //if head is changed, update head
            {
              x = rotateLeft(x->parent->parent);
              root = x;
              root->parent = NULL;
            }
            else //head is not changed
            {
              node* ggp = parent->parent->parent;
              if(ggp->child1 == parent->parent) //grandparent is greatgp's left child
              {
                x = rotateLeft(x->parent->parent);

                //connect rotated subtree's root to the parent of prerotation subtree's root
                x->parent = ggp;
                ggp->child1 = x;
              }
              else //grandparent is greatgp's right child
              {
                x = rotateLeft(x->parent->parent);
                x->parent = ggp;
                ggp->child2 = x;
              }
              
              
            }
           //swap colors of parent and grandparent of x before rotation
            bool color = x->red;
            x->red = x->child1->red;
            x->child1->red = color;
          }
        }
      }
    }
  }
  else if(value > current->data) //go right subtree
  {
    add(root, current->child2, current, value);
  }
  else //go left subtree
  {
    add(root, current->child1, current, value);
  }
}

//searches tree and returns node of specified value
node* search(node* &root, int value)
{
  if(root == NULL) //node of specified value is not found
  {
    return NULL; 
  }
  else if(value == root->data) 
  {
    return root;
  }
  else if(value > root->data) //go down right subtree
  {
    search(root->child2, value);
  }
  else //go down left subtree
  {
    search(root->child1, value);
  }
}

//recursively searches tree and removes a node of the specified value
void remove(node* &root, node* parent, int value)
{
  if(root == NULL) //nothing in tree or reaches end
  {
    return;
  }
  else if(value == root->data) //finds a matching value
  {
    if(root->child1 == NULL && root->child2 == NULL) //node to delete is leaf
    {
      if (parent != root) //if there is more than one node in the tree
      {
        
         //delete the node
        delete root->child1;
        delete root->child2;
        delete root;
        
        if (parent->child1 == root) //find which child is correct
        {
            parent->child1 = NULL;
        }
        else 
        {
            parent->child2 = NULL;
        }

       
      }
      else //if there is only one node in the tree
      {
        //delete the node
        delete root->child1;
        delete root->child2;
        delete root;
        root = NULL;
      }     
    }
    else if(root->child1 != NULL && root->child2 != NULL) //node to delete has 2 children
    {
      node* succParent = root;
      node* succ = findSuccessorWithTwoChildren(succParent, root->child2); //finds successor and its parent
      root->data = succ->data; //copies data from succesor to root
      remove(succ, succParent, succ->data); //deletes successor
    }
    else if(root->child1 != NULL) //node to delete has only child 1
    {
      if(parent != root) //if not at top of tree
      {
       if (parent->child1 == root) //find which child is correct
        {
          parent->child1 = root->child1; //parent is linked to root's child
          delete root; 
        }
        else 
        {
          parent->child2 = root->child1;        
          delete root;  
        }
      }
      else
      {
        if (parent->child1 == root) //find which child is correct
        {
          root = root->child1; //root becomes child
          delete parent; //delete old root
        }
        else 
        {
          root = root->child1;        
          delete parent;  
        }
      }
    }
    else if(root->child2 != NULL) //node to delete has only child 2
    {
      if(parent != root) //if not at top of tree
      {
        if (parent->child1 == root) //find which child is correct
        {
          parent->child1 = root->child2;
          delete root;  
        }
        else 
        {
          parent->child2 = root->child2;        
          delete root;  
        }
      }
      else
      {
        if (parent->child1 == root) //find which child is correct
        {
          root = root->child1;
          delete parent;  
        }
        else 
        {
          root = root->child1;        
          delete parent;  
        }
      }
    }
  }
  else if(value > root->data) //goes down right subtree
  {
    remove(root->child2, root, value);
  }
  else //goes down left subtree
  {
    remove(root->child1, root, value);
  }
  
}

//finds the inorder successor of a node with 2 children. Uses the right child of the node(child2). Additionally sets the parent to the parent of the successor.
node* findSuccessorWithTwoChildren(node* &parent, node* child2)
{
  if(child2->child1 == NULL) //finds the leftmost node of the parent's right child 
  {
    return child2;
  }
  else 
  {
    parent = child2;
    findSuccessorWithTwoChildren(parent, child2->child1);
  }
}

void balance(node* n)
{
  if(getUncle(n)->red == true) //if uncle is red
  {
    
  }
  else //if uncle is black
  {
    
  }
}

//returns a pointer to the uncle of node n
node* getUncle(node* n)
{
  if(n->parent->parent->child1 == n->parent) //if grandparent child1 is parent, uncle is grandparent child2
  {
    return n->parent->parent->child2;
  
  }
  else //if grandparent child1 is not parent, uncle is grandparent child1
  {
    return n->parent->parent->child1;
  }
}

//rotates left
node* rotateLeft(node* n)
{
  node* x = n->child2;
  node* y = x->child1;
  x->child1 = n;
  n->child2 = y;
  n->parent = x; 
  if(y != NULL)
      y->parent = n;
  return(x);
}

//rotates right
node* rotateRight(node* n)
{
  node* x = n->child1;
  node* y = x->child2;
  x->child2 = n;
  n->child1 = y;
  n->parent = x;
  if(y != NULL)
      y->parent = n;
  return(x);
}