//
// Created by arthur wesley on 7/1/21.
//

#ifndef SENTE_TREE_H
#define SENTE_TREE_H

#include <memory>
#include <unordered_set>

#include <pybind11/pybind11.h>

namespace py = pybind11;

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

        bool hasChild(const Type& toFind){
            for (const auto& child : children){
                if (child->payload == toFind){
                    return true;
                }
            }
            return false;
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
            if (not cursor->hasChild(payload)){
                // if the move isn't already a child node, insert it
                cursor = std::make_shared<TreeNode<Type>>(payload, cursor);
                cursor->parent->children.insert(cursor);
                depth++;
            }
            else {
                // if we already have this move, step down to it
                stepTo(payload);
            }
        }
        void insertNoStep(const Type& payload){
            // only insert if the payload doesn't already exist
            if (not cursor->hasChild(payload)) {
                cursor->children.insert(std::make_shared<TreeNode<Type>>(payload, cursor));
            }
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
        void stepDown(){
            if (not cursor->isLeaf()){
                cursor = *cursor->children.begin(); // step into whichever
                depth++;
            }
            else{
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

        void advanceToRoot(){
            cursor = root;
            depth = 0;
        }

        Type get() const{
            if (not cursor->isRoot()){
                return cursor->payload;
            }
            else {
                throw std::domain_error("at root of tree, no move available");
            }
        }

        std::unordered_set<std::shared_ptr<TreeNode<Type>>> getChildren() const{
            return cursor->children;
        };

        unsigned getDepth() const{
            return depth;
        }

        bool isAtRoot(){
            return cursor->isRoot();
        }
        bool isAtLeaf(){
            return cursor->isLeaf();
        }
        bool isChild(const Type& move){
            // can we find the move in the list of children
            return not cursor->children.find(move) == cursor->children.end();
        }

    private:

        std::shared_ptr<TreeNode<Type>> cursor;
        std::shared_ptr<TreeNode<Type>> root;
        unsigned depth;

    };

}

#endif //SENTE_TREE_H
