//
// Created by arthur wesley on 7/1/21.
//

#ifndef SENTE_TREE_H
#define SENTE_TREE_H

#include <vector>
#include <memory>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace sente {
    namespace utils{

        template<typename Type>
        struct TreeNode{

            /**
             *
             * RootNode constructor
             *
             */
            TreeNode(){
                parent = nullptr;
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

            typename std::vector<std::shared_ptr<TreeNode>>::iterator findChild(const Type& toFind){
                return std::find_if(children.begin(), children.end(), [toFind](const std::shared_ptr<TreeNode>& child){
                    return child->payload == toFind;
                });
            }

            bool hasChild(const Type& toFind){
                return findChild(toFind) != children.end();
            }

            Type payload;
            std::shared_ptr<TreeNode> parent; // you do not own your parent
            std::vector<std::shared_ptr<TreeNode>> children; // you do own your children

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
                root->children.push_back(std::make_shared<TreeNode<Type>>(payload, root));
                depth = 1;
            }

            void insert(const Type& payload){
                if (not cursor->hasChild(payload)){
                    // if the move isn't already a child node, insert it
                    cursor->children.push_back(std::make_shared<TreeNode<Type>>(payload, cursor));
                    cursor = cursor->children.back();
                    depth++;
                    size++;
                }
                else {
                    // if we already have this move, step down to it
                    stepTo(payload);
                }
            }
            void insertNoStep(const Type& payload){
                // only insert if the payload doesn't already exist
                if (not cursor->hasChild(payload)) {
                    cursor->children.push_back(std::make_shared<TreeNode<Type>>(payload, cursor));
                    size++;
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
                    cursor = cursor->children[0]; // step into the first branch
                    depth++;
                }
                else{
                    throw std::domain_error("cannot infer child to step to (no children to step to)");
                }
            }
            void stepTo(const Type& value){
                if (cursor->hasChild(value)){
                    cursor = *cursor->findChild(value);
                    depth++;
                }
                else {
                    throw std::domain_error("could not step to child node: the desired value " + std::string(value) + " could not be located");
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

            unsigned getDepth() const{
                return depth;
            }
            unsigned getSize() const{
                return size;
            }

            /**
             *
             * get the sequence of items that lead to the cursor node
             *
             * @return
             */
            std::vector<Type> getSequence(){

                // copy the cursor
                auto temp = cursor;
                std::vector<Type> moves;

                while (not temp->isRoot()){
                    moves.push_back(temp->payload); // add the current item
                    temp = temp->parent; // step up
                }

                // reverse the vector
                return std::vector<Type>(moves.rbegin(), moves.rend());

            }

            std::vector<Type> getChildren() const {
                std::vector<Type> children;

                for (const auto& child : cursor->children){
                    children.push_back(child->payload);
                }

                return children;
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
            unsigned size;

        };

    }
}

#endif //SENTE_TREE_H
