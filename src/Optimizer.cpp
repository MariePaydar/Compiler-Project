#include "Optimizer.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"

namespace opt {
class OptimizerVisitor : public ASTVisitor{
    llvm::StringSet<> Live; // StringSet to store live variables
    llvm::StringSet<> potentialVar; // StringSet to store variables that have the potential to be alive
    llvm::StringSet<> noLive;
    bool liveVar;
    llvm::SmallVector<AST *> liveCode;
public:

    // Constructor
    OptimizerVisitor(){
        llvm::StringRef result("result");
        Live.insert(result);
        liveVar = false;
    } 

    llvm::SmallVector<AST *> getLiveCode(){
        llvm::SmallVector<AST *> reversed;
        for (llvm::SmallVector<AST *>::reverse_iterator it = liveCode.rbegin(); it != liveCode.rend(); ++it) {
            reversed.push_back(*it);
        }
        return reversed;
    }
    
    // Visit function for Program nodes
    virtual void visit(Program &Node) override { 
        
        for (llvm::SmallVector<AST *>::reverse_iterator I = Node.rbegin(), E = Node.rend(); I != E; ++I)
        {
            // for (const auto &entry : Live) {
            //     llvm::outs() << entry.getKey() << " ";
            // }
            // llvm::outs() << "\n";

            (*I)->accept(*this);
            
            // llvm::outs() <<"check livevar in loop: "<< liveVar <<"\n";
            
            if (liveVar){

                liveCode.push_back(*I);
                
                for (const auto &entry : potentialVar) {
                    Live.insert(entry.getKey());
                }
            
            }

            potentialVar.clear();
            liveVar = false;
        }
    };

    // Visit function for Final nodes
    virtual void visit(Final &Node) override {
        if (Node.getKind() == Final::Ident) {
            if (Live.count(Node.getVal())) {
                liveVar = true;
            }
            potentialVar.insert(Node.getVal());
        }
    };

    // Visit function for BinaryOp nodes
    virtual void visit(BinaryOp &Node) override {
        Expr* right = Node.getRight();
        Expr* left = Node.getLeft();
        if (left)
            left->accept(*this);

        if (right)
            right->accept(*this);

    };

    // Visit function for Assignment nodes
    virtual void visit(Assignment &Node) override {
        
        // llvm::outs() << "in ass ";
        Final *dest = Node.getLeft();
        if (Live.count(dest->getVal())) {
            liveVar = true;
        }

        // llvm::outs() << "Check livevar in assignment: " <<liveVar <<"\n";

        if (liveVar){

            if (Node.getAssignKind() == Assignment::AssignKind::Assign){
                noLive.insert(dest->getVal());
                Live.erase(dest->getVal());
            }
            Expr *Right = Node.getRight();
            if (Right)
                Right->accept(*this);
        }

    };

    virtual void visit(Declaration &Node) override {
        // llvm::outs() << "in dec ";
        for (llvm::SmallVector<llvm::StringRef, 8>::const_iterator I = Node.varBegin(), E = Node.varEnd(); I != E; ++I) {
            if (Live.count(*I) || noLive.count(*I)){
                liveVar = true;
            }
        }
        if (liveVar){
            for (llvm::SmallVector<Expr *, 8>::const_iterator I = Node.valBegin(), E = Node.valEnd(); I != E; ++I){
                (*I)->accept(*this); 
            }
        }
    };

    virtual void visit(Comparison &Node) override {
        
    };

    virtual void visit(LogicalExpr &Node) override {
        
    };

    virtual void visit(IfStmt &Node) override {
        
    };

    virtual void visit(elifStmt &Node) override {
        
    };

    virtual void visit(IterStmt &Node) override {

    };
};
}

llvm::SmallVector<AST *> Optimizer::optimize(Program *Tree)
{
    // Create an instance of the OptimizerVisitor and run it on the AST to Optimize LLVM IR.
    opt::OptimizerVisitor *Optimizer = new opt::OptimizerVisitor();
    Tree->accept(*Optimizer); // Initiate the optimizing by traversing the AST using the accept function
    return Optimizer->getLiveCode();
};