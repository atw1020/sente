//
// Created by arthur wesley on 7/1/21.
//

#ifndef SENTE_TREE_H
#define SENTE_TREE_H

#include <unordered_set>

namespace sente_utils{

    template<typename Type>
    struct TreeNode{

        /**
         *
         * RootNode constructor
         *
         */
        TreeNode(){
            parent = std::shared_ptr<TreeNode<Type>>();
        }

        TreeNode(const Type payload, const std::shared_ptr<TreeNode>& parent){
            this->payload = payload;
            this->parent = parent;
        }

        bool isRoot(){
            return parent == nullptr;
        }
        bool isLeaf(){
            return children.empty();
        }

        Type payload;
        std::shared_ptr<TreeNode> parent; // you do not own your parent
        std::unordered_set<std::shared_ptr<TreeNode>> children; // you do own your children

    };

    template<typename Type>
    class Tree{
    public:

        Tree(){
            root = std::make_shared<TreeNode<Type>>(); // create the root
            cursor = root;
            depth = 0;
        }

        explicit Tree(const Type& payload){
            root = std::make_shared<TreeNode<Type>>(); // create the root
            root->children.insert(std::make_shared<TreeNode<Type>>(payload, root));
            depth = 1;
        }

        void insert(const Type& payload){
            cursor = std::make_shared<TreeNode<Type>>(payload, cursor);
            cursor->parent->children.insert(cursor);
            depth++;
        }
        void insertNoAdvance(const Type& payload){
            cursor->parent->children.insert(std::make_shared<TreeNode<Type>>(payload, cursor));
        }

        void stepUp(){
            if (not cursor->isRoot()){
                cursor = cursor->parent;
                depth--;
            }
            else {
                throw std::domain_error("cannot step up past root node");
            }
        }
        Type stepDown(){
            if (cursor->children.size() == 1){
                cursor = *cursor->children.begin(); // step into whichever
                depth++;
                return cursor->payload;
            }
            else if (cursor->isLeaf()){
                throw std::domain_error("cannot infer child to step to (no children to step to)");
            }
        }
        void stepTo(const Type& value){
            for (const auto & child : cursor->children){
                if (child->payload == value){
                    cursor = child;
                    depth++;
                    return;
                }
            }
        }

        void stepToRoot(){
            cursor = root;
            depth = 0;
        }

        Type get() const{
            if (not cursor->isRoot()){
                return cursor->payload;
            }
        }

        unsigned getDepth() const{
            return depth;
        }

        bool isAtRoot(){
            return cursor->isRoot();
        }
        bool isAtLeaf(){
            return cursor->isLeaf();
        }

    private:

        std::shared_ptr<TreeNode<Type>> cursor;
        std::shared_ptr<TreeNode<Type>> root;
        unsigned depth;

    };

}

#endif //SENTE_TREE_H
