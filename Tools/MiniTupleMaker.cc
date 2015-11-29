#include "MiniTupleMaker.h"

MiniTupleMaker::MiniTupleMaker(TTree * const t) : tree_(t)
{
}

MiniTupleMaker::~MiniTupleMaker()
{
    for(auto& cf : cleanupVec_) cf();
}

void MiniTupleMaker::setTupleVars(const std::set<std::string> tv)
{
    for(auto& var : tv) tupleVars_[var] = std::make_pair(static_cast<void *>(nullptr), nullptr);
}

void MiniTupleMaker::initBranches(const NTupleReader& tr)
{
    for(auto& var : tupleVars_)
    {
        std::string type;
        tr.getType(var.first, type);

        if(type.find("vector") != std::string::npos)
        {
            throw "MiniTupleMaker::initBranches(...): Vector types are not allowed in MiniTuples!!!";
        }
        else
        {
            std::string varTupleType = "";
            if     (type.find("unsigned int")   != std::string::npos) 
            {
                prepVar<unsigned int>(tr, var.first, var.second);
                varTupleType = "/i";
            }
            else if(type.find("int")            != std::string::npos) 
            {
                prepVar<int>(tr, var.first, var.second);
                varTupleType = "/I";
            }
            else if(type.find("float")          != std::string::npos) 
            {
                prepVar<float>(tr, var.first, var.second);
                varTupleType = "/F";
            }
            else if(type.find("double")         != std::string::npos)
            {
                prepVar<double>(tr, var.first, var.second);
                varTupleType = "/D";
            }
            else if(type.find("unsigned short") != std::string::npos) 
            {
                prepVar<unsigned short>(tr, var.first, var.second);
                varTupleType = "/s";
            }
            else if(type.find("short")          != std::string::npos) 
            {
                prepVar<short>(tr, var.first, var.second);
                varTupleType = "/S";
            }
            else if(type.find("unsigned char")  != std::string::npos) 
            {
                prepVar<unsigned char>(tr, var.first, var.second);
                varTupleType = "/c";
            }
            else if(type.find("char")           != std::string::npos) 
            {
                prepVar<char>(tr, var.first, var.second);
                varTupleType = "/C";
            }
            else if(type.find("bool")           != std::string::npos) 
            {
                prepVar<bool>(tr, var.first, var.second);
                varTupleType = "/O";
            }
            else if(type.find("unsigned long")  != std::string::npos) 
            {
                prepVar<unsigned long>(tr, var.first, var.second);
                varTupleType = "/l";
            }
            else if(type.find("long")           != std::string::npos) 
            {
                prepVar<long>(tr, var.first, var.second);
                varTupleType = "/L";
            }
            else
            {
                throw "MiniTupleMaker::initBranches(...): Variable type unknown!!! var: " + var.first + ", type: " + type;
            }

            tree_->Branch(var.first.c_str(), var.second.first, (var.first + varTupleType).c_str());
        }
    }
}

void MiniTupleMaker::fill()
{
    for(auto& var : tupleVars_) var.second.second();

    tree_->Fill();
}