/**
 *  red_black_tree.hpp
 *  An implementation of a red black tree.
 *
 *  @author Marquess Valdez
 *  @version 1.0
 */
#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include <cstddef> //for std::size_t
#include <vector>  //for std::vector

namespace mqs {

template <typename T>
class red_black_tree {
  private:
    class red_black_tree_node {
      public:
        T data;
        enum Color {red = 0, black = 1}; 
        Color color;
        red_black_tree_node *parent;
        red_black_tree_node *left;
        red_black_tree_node *right;

        // Default constructor
        red_black_tree_node() : color(red), parent(nullptr), left(nullptr), right(nullptr) {};
        // Construct a node with data, black by default since if a parent isn't known this will probably be used for the root.
        explicit red_black_tree_node(const T& d) : data(d), color(red), parent(nullptr), left(nullptr), right(nullptr) {}
        // Construct a node with data and parent, red by default since this will probably be used for an insertion not at the root.
        red_black_tree_node(const T& d, red_black_tree_node *p) : data(d), color(red), parent(p), left(nullptr), right(nullptr) {};
        // Construct a node where everything is known.
        red_black_tree_node(const T& d, Color c, red_black_tree_node *p, red_black_tree_node *l, red_black_tree_node *r) : data(d), color(red), parent(p), left(l), right(r) {};

        inline T getData()
        {
          return this->data;
        }

        inline bool isBlack()
        {
          return this->color == black;
        }

        inline bool isRed()
        {
          return this->color == red;
        }

        inline void setBlack()
        {
          this->color = black;
        }

        inline void setRed()
        {
          this->color = red;
        }

        inline red_black_tree_node* getParent()
        {
          return this->parent;
        }

        inline red_black_tree_node* getSibling()
        {
          red_black_tree_node* p = this->parent;
          if(p) {
            if(p->left == this) {
              return p->right;
            }
            return p->left;
          }
          return nullptr;
        }

        inline red_black_tree_node* getGrandparent()
        {
          if(this->parent) {
            return this->parent->parent;
          }
          return nullptr;
        }

        inline red_black_tree_node* getUncle()
        {
          red_black_tree_node* gp = this->getGrandparent();
          if(gp) {
            if(gp->left == this->parent) {
              return gp->right;
            }
            return gp->left;
          }
          return nullptr;
        }

        inline red_black_tree_node* getPredecessor()
        {
          red_black_tree_node* curr = this->left;
          while(curr->right) {
            curr = curr->right; 
          }
          return curr;
        }

        inline red_black_tree_node* getSuccessor()
        {
          red_black_tree_node *p = this->parent; 
          if(!p) {
            p = this; 
          }
          if(p->right) {
            p = p->right;
            while(p->left) {
              p = p->left;
            }
            return p;
          }
          red_black_tree_node *curr = this;
          while(p->left != curr) {
            curr = p;
            p = p->parent;
          }
          return p;
        }

        inline void rotateLeft()
        {
          if(this->right) {
            red_black_tree_node* nn = this->right;
            if(this->parent) {
              if(this->parent->left == this) {
                this->parent->left = nn; 
              } else {
                this->parent->right = nn;
              }
            }
            nn->parent = this->parent;
            this->parent = nn;
            this->right = nn->left;
            nn->left = this;
            if(this->right) {
              this->right->parent = this;
            }
          }
        }

        inline void rotateRight()
        {
          if(this->left) {
            red_black_tree_node* nn = this->left;
            if(this->parent) {
              if(this->parent->right == this) {
                this->parent->right = nn;
              } else {
                this->parent->left = nn;
              }
            }
            nn->parent = this->parent;
            this->parent = nn;
            this->left = nn->right;
            nn->right = this;
            if(this->left) {
              this->left->parent = this;
            }
          }
        }
       
        ~red_black_tree_node()
        {
          if(left) {
            delete left;
          }
          if(right) {
            delete right;
          }
        }
    };
    
    red_black_tree_node *root;
    size_t _size;

    int height(red_black_tree_node* node)
    {
      if(!node) {
        return -1;
      }
      return 1 + std::max(height(node->right), height(node->left));
    }

    void insert_repair(red_black_tree_node* node)
    {
      red_black_tree_node *parent = node->getParent();
      // Case 1: node is the root node, it must be set to black.
      if(!parent) {
        node->setBlack();
        root = node;
        return;
      }
      // Case 2: The parent of node is black, theres nothing to be done.
      if(parent->isBlack()) {
        return;
      }
      red_black_tree_node *grandparent = node->getGrandparent(), *uncle = node->getUncle();
      // Case 3: The parent is red and the uncle is black
      if(!uncle || uncle->isBlack()) {
        // Goal: Rotate parent into grandparent position
        // Part 1: If node is on the "inside" of the tree, we need to rotate it to the outside first.
        if(grandparent->left && node == grandparent->left->right) {
          parent->rotateLeft();
          node = node->left;
        } else if(grandparent->right && node == grandparent->right->left) {
          parent->rotateRight(); 
          node = node->right;
        }
        parent = node->getParent();
        grandparent = node->getGrandparent();
        // Part 2: Now we can rotate the parent into place by rotating in the correct direction.
        if(node == parent->left) {
          grandparent->rotateRight(); // Parent is the left child of grandparent, so we rotate the grandparent right.
        } else {
          grandparent->rotateLeft();  // Parent is the right child of grandparent, so we rotate the grandparent left.
        }
        // Finally, we make the parent and grandparent the appropriate colors and return true.
        parent->setBlack();
        grandparent->setRed();
        if(grandparent == root) {
          root = parent;
        }
        return;
      }
      //Case 4: The parent and uncle are both red
      parent->setBlack();
      uncle->setBlack();
      grandparent->setRed();
      insert_repair(grandparent);
    }

    void remove_repair(red_black_tree_node *node, red_black_tree_node *curr)
    {
      // curr hasn't been deleted yet so we use it here instead of node, since node isn't guaranteed to exist.
      red_black_tree_node *parent = curr->parent, *sibling = curr->getSibling();
      // Case 1: The node is the root: We just need to paint it black.
      if(!parent) {
        root = node;
        if(node) { // Edge case, the node that we deleted was the only node, we will reach this case and node won't exist.
          node->setBlack();
        }
        return;
      } 
      // Case 2: The nodes sibling is red.
      if(sibling && sibling->isRed()) {
        parent->setRed();
        sibling->setBlack();
        parent->rotateLeft();
        if(root->parent) {
          while(root->parent) {
            root = root->parent;
          }
        }
        // Since we've rotated, node has a new sibling and we update accordingly.
        if(node) {
          sibling = node->getSibling();
        } else {
          sibling = (parent->left ? parent->left : parent->right);
        }
      }
      // Case 3: The nodes parent is black, and both of siblings children is black.
      if(parent->isBlack() && sibling && (!sibling->left || sibling->left->isBlack()) && (!sibling->right || sibling->right->isBlack())) {
        sibling->setRed();                // We repaint the sibling red and recurse on the parent, since there is now
        remove_repair(parent, parent);    // one less black node when passing through parent.
        return;
      }
      // Case 4: The nodes parent is red, its sibling is black, and both its children are black
      if(parent->isRed() && sibling && sibling->isBlack() && (!sibling->left || sibling->left->isBlack()) && (!sibling->right || sibling->right->isBlack())) {
        parent->setBlack();   // By swapping colors, we've made up for the missing black node on the path of node, but
        if(parent->left) {
          parent->left->setRed();
        }
        if(parent->right) {
          parent->right->setRed();
        }
        //sibling->setRed();    // haven't effected the path through the sibling.
        return;
      }
      // Case 5: The sibling is non null and black...
      if(sibling && sibling->isBlack() && sibling->left && sibling->left->isRed()) {
        // With the node on its parents left side, and siblings right child black and its left child red.
        if(parent->left == node && (!sibling->right || sibling->right->isBlack()) && sibling->left && sibling->left->isRed()) {
          sibling->setRed();
          sibling->left->setBlack();
          sibling->rotateRight();
          if(root->parent) {
            while(root->parent) {
              root = root->parent;
            }
          } else if(parent->right == node && sibling->right && sibling->right->isRed() && (!sibling->left || sibling->left->isBlack())) {
            sibling->setRed();
            sibling->right->setBlack();
            sibling->rotateLeft();
            if(root->parent) {
              while(root->parent) {
                root = root->parent;
              }
            }
          }
          // Again, since we (possibly) rotated, we update the sibling.
          if(node) {
            sibling = node->getSibling();
          }
          else {
            sibling = (parent->left ? parent->left : parent->right);
          }
        }  
      }
      // Case 6: Sibling is not null and black, and its right child is red.
      if(sibling && sibling->isBlack() && sibling->right && sibling->right->isRed()) {
        sibling->color = parent->color;
        parent->setBlack();
        sibling->right->setBlack();
        if(parent->left == curr) {
          parent->rotateLeft();
          if(root->parent) {
            while(root->parent) {
              root = root->parent;
            }
          }
        } else {
          parent->rotateRight();
          if(root->parent) {
            while(root->parent) {
              root = root->parent;
            }
          }
        }
      }
    }


  public:
    red_black_tree() : root(nullptr), _size(0) {}

    explicit red_black_tree(std::vector<T>& data)
    {
      root = nullptr;
      _size = 0;
      for(T d : data) {
        insert(d);
      } 
    }

    /**
     *  Inserts a single piece of data into the tree.
     *  @param d the data to insert. 
     *  @returns true if new node was created and inserted and 
     *  false if a node with the data already existed in tree.
     */
    bool insert(const T& d)
    {
      // First we find the position of the node.
      red_black_tree_node *node = root, *parent = nullptr;
      bool lastLeft;
      while(node) {
        parent = node;
        if(node->data == d) { // A node with the data already exists.
          return false;       // So we return false;
        } else if(node->data > d) {
          node = node->left;
          lastLeft = true;
        } else {
          node = node->right; 
          lastLeft = false;
        }
      }
      // Then we create the node.
      node = new red_black_tree_node(d, parent);
      _size++;
      if(parent && lastLeft) { // And set the children properly. Notice, we check for existence of the parent, in case
        parent->left = node;   // this node is the root node.
      }
      else if(parent) {
        parent->right = node;
      }
      insert_repair(node);    // Repair balance to the tree.
      return true;
    }
    
    /**
     *  Removes a single piece of data from the tree.
     *  @param d the data to remove.
     *  @returns true if the node with data was removed and false if a node with
     *  data didn't exist in the tree.
     */
    bool remove(const T& d)
    {
      red_black_tree_node* curr = root;
      while(curr && curr->data != d) {
        if(curr->data > d) {
          curr = curr->left;
        } else {
          curr = curr->right;
        }
      }
      // If curr is null, there isn't a node with the data and we return false.
      if(!curr) {
        return false;
      }
      if(_size == 1) {
        delete curr;
        root = nullptr;
        _size--;
        return true;
      }
      // If there are two non-null children, replace with the value of inorder predecessor and delete the predecessor.
      if(curr->left && curr->right) {
        red_black_tree_node *predecessor = curr->getPredecessor();
        curr->data = predecessor->data;
        curr = predecessor;
      }
      if(curr->isRed()) {
        _size--;
        if(curr == curr->parent->left) {
          curr->parent->left = nullptr;
        } else {
          curr->parent->right = nullptr;
        }
        /*
        curr->left = nullptr;
        curr->right = nullptr;
        */
        delete curr;
        return true;
      }
      red_black_tree_node *child = (curr->right ? curr->right : curr->left);
      if(!child || child->isBlack()) {  // If the child is black, we need to perform some number of the 6 "double-back" transformations.
        remove_repair(child, curr); // Since multiple "repairs" may need to be done we use a seperate recursive function.
      }
      if(child) { // If the child wasn't a null node, we set the parent, and make sure its black.
        child->parent = curr->parent;
        child->setBlack();
      }
      // We replace the node with its child node.
      if(curr == root) {
        root = child;
      } else if(child != root && curr->parent->left == curr) {
        curr->parent->left = child;
      } else if(child != root) {
        curr->parent->right = child;
      }
      _size--;
      // Set currs children to null, since destructor would destroy its former children otherwise.
      curr->left = nullptr;
      curr->right = nullptr;
      // Finally, we delete the node.
      delete curr;
      return true;
    }

    /**
     *  Checks if data is in the tree.
     *  @param d the data to find.
     *  @returns true if node with data was found and false otherwise.
     */
    inline bool find(const T& d)
    {
      red_black_tree_node* curr = root;
      while(curr) {
        if(curr->data == d) {
          return true;
        }
        if(curr->data > d) {
          curr = curr->left;
        }
        else {
          curr = curr->right;
        }
      }
      return false;
    }

    /**
     *  @returns the size of the tree.
     */
    inline size_t size()
    {
      return _size;
    }

    /**
     *  @returns the height of the ree.
     */
    int height()
    {
      return height(root);
    }

    /**
     *  @returns a vector<pair<T,bool>> of the tree in-order where vector[i].second is true if the node was black.
     */
    std::vector<std::pair<T,bool>> dump()
    {
      std::vector<std::pair<T,bool>> out;
      red_black_tree_node *curr = root;
      while(curr) {
        if(!curr->left) {
          out.push_back(std::make_pair(curr->data, curr->isBlack()));
          curr = curr->right;
        } else {
          red_black_tree_node *pre = curr->left;
          while(pre->right && pre->right != curr) {
            pre = pre->right;
          }
          if(!pre->right) {
            pre->right = curr;
            out.push_back(std::make_pair(curr->data, curr->isBlack()));
            curr = curr->left;
          } else {
            pre->right = nullptr;
            curr = curr->right;
          }
        }
      }
      return out;
    }

    ~red_black_tree() {
      if(root) {
        delete root;
      }
    }
};

}

#endif
