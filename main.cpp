/*
 * Program to implement red black tree.
 * By: Max Shi
 * 5/28/2022
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
void remove(node* &root, node* n, node* parent, int value);
void delBalance(node* &root, node* u, node* v);
node* search(node* &root, int value);
node* findSuccessorWithTwoChildren(node* &parent, node* child2);
void balance(node* &root, node* x, node* parent);
node* getUncle(node* n);
node* getSibling(node* n);
node* rotateLeft(node* n);
node* rotateRight(node* n);
void rotateHelp(node* &root, node* tip, node* n, node* nParent);


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
    {
      cout << "Enter value to remove: ";
      cin.getline(input, 80, '\n');
      if (isNum(input) == true) //if input is a number
      { 
        remove(tree, tree, tree, charToInt(input));
      }
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

    balance(root, current, current->parent);
    
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

void balance(node* &root, node* x, node* parent)
{

  if(parent != NULL)
  {
    if(parent->red == true) 
    {
      if(getUncle(x) == NULL || getUncle(x)->red == false) //uncle is black
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
      else //uncle is red
      {
        if(getUncle(x)->red == true) //uncle is red
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
              balance(root, x, x->parent);
            
          }
          if(root->red == true) //color root black if it is red
          {
            root->red = false;
          }
        }
        
      }
    }
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
void remove(node* &root, node* n, node* parent, int value)
{
  if(n == NULL) //nothing in tree or reaches end
  {
    return;
  }
  else if(value == n->data) //finds a matching value
  {
    if(n->child1 == NULL && n->child2 == NULL) //node to delete is leaf
    {
      if (parent != n) //if there is more than one node in the tree
      {
	      if(n->red == false) //n and its replacement are both black
	      {
	        delBalance(root, NULL, n); //balance with n's replacement as NULL
	      }
	      else //n is red and replacement is black(NULL)
	      {
    	    if(getSibling(n) != NULL)
    	    {
    		    getSibling(n)->red = true; //make sibling red
    	    }
	      }

  	  //remove n from tree
  	  if (parent->child1 == n) //find which child is correct
  	  {
  	    parent->child1 = NULL;
  	  }
  	  else
  	  {
  	    parent->child2 = NULL;
  	  }
	
      //delete the node
      delete n;
       
      }
      else //if there is only one node in the tree
      {
        //delete the node
        delete n->child1;
        delete n->child2;
        delete n;
        root = NULL;
      }     
    }
    else if(n->child1 != NULL && n->child2 != NULL) //node to delete has 2 children
    {
      node* succParent = n;
      node* succ = findSuccessorWithTwoChildren(succParent, n->child2); //finds successor and its parent 

      int temp = n->data;
      n->data = succ->data; //copies data from succesor to n
      succ->data = n->data;
     // n->red = succ->red;
      remove(root, succ, succParent, succ->data); //deletes successor
    }
    else if(n->child1 != NULL) //node to delete has only child 1
    {
      if(root != n) //if not at top of tree
      {
       if (parent->child1 == n) //find which parent child points to n
        {
          parent->child1 = n->child1; //parent is linked to n's child
          n->child1->parent = parent;
          delBalance(root, n->child1, n);
          delete n; 
        }
        else 
        {
          parent->child2 = n->child1;    
          n->child1->parent = parent;
          delBalance(root, n->child1, n);
          delete n;  
        }
      }
      else
      {
        root = n->child1;
        root->parent = NULL;
        root->red = false;
        delete n;
      }
    }
    else if(n->child2 != NULL) //node to delete has only child 2
    {
      if(root != n) //if not at top of tree
      {
        if (parent->child1 == n) //find which parent child points to n
        {
          parent->child1 = n->child2;
          n->child2->parent = parent;
          delBalance(root, n->child2, n);
          delete n;  
        }
        else 
        { 
          parent->child2 = n->child2;   
          n->child2->parent = parent;
          delBalance(root, n->child2, n);
          delete n;  
        }
      }
      else
      {
        root = n->child2;   
        root->red = false;
        root->parent = NULL;
        delete n; 
      }
    }
  }
  else if(value > n->data) //goes down right subtree
  {
    remove(root, n->child2, n, value);
  }
  else //goes down left subtree
  {
    remove(root, n->child1, n, value);
  }
}

//balance when deleting with v as node that will be deleted and u as node that will replace it
void delBalance(node* &root, node* u, node* v)
{
  bool uvBlack = false;
  if(u == NULL || u->red == false)
  {
    if(v->red == false)
    {
      uvBlack = true;
    }
  }

  if(uvBlack == true) //u and v are black
  {
    if(v == root)
    {
      return;
    }

    node* sibling = getSibling(v);
    if(sibling == NULL) //sibling NULL
    {
      //cout << v->data;
      delBalance(root, NULL, v->parent);
      return;
    }
    
    if(sibling->red == false) //v sibling is black
    {
      
      if(sibling->child1 == NULL || sibling->child1->red == false)
      {
        if(sibling->child2 == NULL || sibling->child2->red == false)  //sibling child both black
        {
          sibling->red = true; //make sibling red
          
          if(v->parent->red == false) //parent black
          {
            delBalance(root, NULL, v->parent); //recursion on parent
          }
          else //parent red
          {
            v->parent->red = false; //make parent black
          }
          return;
        }
      }
        
      //one or more of sibling child is red
      node* parent = sibling->parent;
      node* gp = parent->parent;
      if(sibling == sibling->parent->child1) //sibling on left
      {
        if(sibling->child1 != NULL && sibling->child1->red == true) //ll: sibling child1 is red
        {
          sibling->child1->red = sibling->red;
          sibling->red = sibling->parent->red;
          rotateHelp(root, rotateRight(parent), parent, gp);          
        }
        else //lr: sibling child 2 is red
        {
          //cout << "lr";
          sibling->child2->red = sibling->parent->red;
          //node* tip = rotateLeft(sibling);
          //cout << tip->data << parent->data << gp->data << endl;

          rotateHelp(root, rotateLeft(sibling), sibling, parent);
          
          //tip = rotateRight(parent);
          //cout << tip->data << parent->data << gp->data << endl;

          rotateHelp(root, rotateRight(parent), parent, gp);
        }        
      }
      else //sibling on right
      {
        if(sibling->child2 != NULL && sibling->child2->red == true) //rr: sibling child2 is red
        {
          sibling->child2->red = sibling->red;
          sibling->red = sibling->parent->red;
          //cout << parent->data << gp->data << endl;

          //node* tip = rotateLeft(parent);
          //cout << tip->data << parent->data << gp->data << endl;
          rotateHelp(root, rotateLeft(parent), parent, gp);          
        }
        else //rl: sibling child 2 is red
        {
          sibling->child1->red = sibling->parent->red;
          rotateHelp(root, rotateRight(sibling), sibling, parent);
          rotateHelp(root, rotateLeft(parent), parent, gp);
        }
      }
      parent->red = false;
      
    }
    else //sibling red
    {
      v->parent->red = true;
      sibling->red = false;

      node* parent = sibling->parent;
      node* gp = parent->parent;
      if(sibling == sibling->parent->child1) //sibling on left
      {
        rotateHelp(root, rotateRight(parent), parent, gp);
      }
      else //sibling on right
      {
        rotateHelp(root, rotateLeft(parent), parent, gp);
      }
      //if(u == NULL)
      //  cout << "uNULL";
      //print(root, 7);
      delBalance(root, u, v);
      
      
    }
  }
   else if(u->red == true || v->red == true) //if u or v are red
  {
    u->red = false; //u becomes black
  }
 
}

//returns sibling of a node
node* getSibling(node* n)
{
  if(n == NULL)
    {
      return NULL;
    }

  if(n->parent == NULL)
    {
      return NULL;
    }

  if(n == n->parent->child1) //n is parent child 1
    {
      return n->parent->child2; //return child2
    }
  else //n is parent child2
    {
      return n->parent->child1; //return child 1
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

//connects a rotated subtree to the rest of the tree. Uses the top of the rotated subtree.
void rotateHelp(node* &root, node* tip, node* n, node* nParent)
{
  if(nParent == NULL)
  {
    root = tip;
    root->parent = NULL;
  }
  else
  {
    tip->parent = nParent;
    if(n == nParent->child1) //sibling on left
    {
      nParent->child1 = tip; //connect nParent to tip
    }
    else
    {
      nParent->child2 = tip;
    }
  }
}
