#pragma once
/*
 * Created Date: 2021-11-17 09:20:03
 * Author: yuyangz
 */

#ifndef  PARSER_H
#define  PARSER_H

#include"../Lexical/Lexical.h"
#include <stack>
#include <map>
#include <sstream>


enum SYNTAX_STATE{
    SBEG,
    BEG,
    O,
    I,
    D,
    B,
    T,
    F,
    M,
    N,
    S,
    R,
    A,
    L,
    C,
    P,
    Q,
    U,
    V
};

enum RIGHT_ELEM_TYPE{
    STATE,
    TERMINATER
};

enum GOTO_ELEM_TYPE{
    MOVE,
    INDUCE,
    ACC,
    ERR
};

struct rightElem{
    RIGHT_ELEM_TYPE type;
    int index;
    bool operator<(const rightElem &r) const {
        bool typeless = (int)type < (int)r.type;
        bool indexless = type == r.type && index < r.index;
        return typeless || indexless;
    }
};

// struct formula{
//     SYNTAX_STATE left;
//     vector<rightElem> right;
// };

typedef multimap<SYNTAX_STATE, vector<rightElem>> formulas ;


struct projectRight{
    // SYNTAX_STATE left;
    vector<rightElem> right;
    int dotpos;
    enum TYPE look;
};

typedef multimap<SYNTAX_STATE, projectRight> projectSet;

struct DFAstate{
    int num;
    projectSet pSet;
};

struct actionElem{
    enum GOTO_ELEM_TYPE gotoElemType;
    int des;
};

struct analyzeProcess{
    stack<int> stateStack;
    stack<rightElem> symbolStack;
    vector<token> inputString;
};

typedef struct treeNode{
    rightElem value;
    vector<treeNode*> childs; 
}*analyzeTree;

const string stateNames[]={
    "SBEG",
    "BEG",
    "O",
    "I",
    "D",
    "C",
    "T",
    "F",
    "M",
    "N",
    "S",
    "R",
    "A",
    "L",
    "C",
    "P",
    "Q",
    "U",
    "V"
};

const formulas grammar={
    {
        SYNTAX_STATE::SBEG,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::BEG}
        }
    },
    {
        SYNTAX_STATE::BEG,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::I},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::O},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::M}
        }
    },
    {
        SYNTAX_STATE::O,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::O},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::D}
        }
    },
    {
        SYNTAX_STATE::O,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::O},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::F}
        }
    },
    {
        SYNTAX_STATE::O,
        {
        }
    },
    {
        SYNTAX_STATE::I,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::I},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::WELL},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::INCLUDE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::L_SHA_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::R_SHA_BRACKET}
        }
    },
    {
        SYNTAX_STATE::I,
        {
        }
    },
    {
        SYNTAX_STATE::D,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::D},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::TYPE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::DELIMETER}
        }
    },
    {
        SYNTAX_STATE::D,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::D},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::TYPE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ASSIGN},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::INTEGER},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::DELIMETER}
        }
    },
    {
        SYNTAX_STATE::D,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::D},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::TYPE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ASSIGN},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::FLOAT_VALUE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::DELIMETER}
        }
    },
    {
        SYNTAX_STATE::D,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::D},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::TYPE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::L_MID_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::R_MID_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ASSIGN},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::B},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::DELIMETER}
        }
    },
    {
        SYNTAX_STATE::D,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::D},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::CHAR},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::L_MID_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::R_MID_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ASSIGN},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::STRING_VALUE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::DELIMETER}
        }
    },
    {
        SYNTAX_STATE::D,
        {
        }
    },
    {
        SYNTAX_STATE::B,
        {
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::INTEGER},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::T}
        }
    },
    {
        SYNTAX_STATE::B,
        {
        }
    },
    {
        SYNTAX_STATE::T,
        {
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::DIVIDER},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::INTEGER},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::T}
        }
    },
    {
        SYNTAX_STATE::T,
        {
        }
    },
    {
        SYNTAX_STATE::F,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::F},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::TYPE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::L_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::R_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::L_BIG_BRACKET},
            {RIGHT_ELEM_TYPE::STATE, SYNTAX_STATE::N},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::R_BIG_BRACKET}
        }
    },
    {
        SYNTAX_STATE::F,
        {
        }
    },
    {
        SYNTAX_STATE::M,
        {
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::TYPE},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::MAIN},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::L_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::R_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::L_BIG_BRACKET},
            {RIGHT_ELEM_TYPE::STATE, SYNTAX_STATE::N},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::R_BIG_BRACKET}
        }
    },
    {
        SYNTAX_STATE::N,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::D},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S}
        }
    },
    {
        SYNTAX_STATE::N,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::D},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::R}
        }
    },
    {
        SYNTAX_STATE::S,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::A}
        }
    },
    {
        SYNTAX_STATE::S,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER,TYPE::DELIMETER},
        }
    },
    {
        SYNTAX_STATE::S,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::IF},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_BIG_BRACKET},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_BIG_BRACKET},
        }
    },
    {
        SYNTAX_STATE::S,
        {
            // {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::IF},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_BIG_BRACKET},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_BIG_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::ELSE},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_BIG_BRACKET},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_BIG_BRACKET}
        }
    },
    {
        SYNTAX_STATE::S,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::WHILE},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_BIG_BRACKET},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::S},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_BIG_BRACKET},
        }
    },
    {
        SYNTAX_STATE::S,
        {
        }
    },
    {
        SYNTAX_STATE::R,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::RETURN},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::DELIMETER},
        }
    },
    {
        SYNTAX_STATE::A,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::ASSIGN},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::DELIMETER},
        }
    },
    {
        SYNTAX_STATE::L,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::C}
        }
    },
    {
        SYNTAX_STATE::L,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::LOGIC},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::C}
        }
    },
    {
        SYNTAX_STATE::L,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::LOGIC},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::EXCLAINMATION},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::C}
        }
    },
    {
        SYNTAX_STATE::L,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::EXCLAINMATION},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::L}
        }
    },
    {
        SYNTAX_STATE::C,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::P}
        }
    },
    {
        SYNTAX_STATE::C,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::C},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::COMP},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::P}
        }
    },
    {
        SYNTAX_STATE::P,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::Q}
        }
    },
    {
        SYNTAX_STATE::P,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::P},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::ADDSUB},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::Q}
        }
    },
    {
        SYNTAX_STATE::Q,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::U}
        }
    },
    {
        SYNTAX_STATE::Q,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::Q},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::BIT},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::U}
        }
    },
    {
        SYNTAX_STATE::Q,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::WAVE},
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::Q}
        }
    },
    {
        SYNTAX_STATE::U,
        {
            {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::V}
        }
    },
    {
        SYNTAX_STATE::U,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::SELF},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::ID}
        }
    },
    {
        SYNTAX_STATE::U,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::SELF}
        }
    },
    {
        SYNTAX_STATE::U,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_SMA_BRACKET},
            {RIGHT_ELEM_TYPE::STATE, SYNTAX_STATE::L},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_SMA_BRACKET}
        }
    },
    {
        SYNTAX_STATE::V,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::ID}
        }
    },
    {
        SYNTAX_STATE::V,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::FLOAT_VALUE}
        }
    },
    {
        SYNTAX_STATE::V,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::INTEGER}
        }
    },
    {
        SYNTAX_STATE::V,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::CHARACTER}
        }
    },
    {
        SYNTAX_STATE::V,
        {
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::ID},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::L_MID_BRACKET},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::INTEGER},
            {RIGHT_ELEM_TYPE::TERMINATER, TYPE::R_MID_BRACKET}
        }
    }
};


string parserProcess(vector<token> &inputString, const vector<vector<int>> gotoTable, const vector<vector<actionElem>> actionTable);
void generateLRTable(vector<vector<int>> &gotoTable, vector<vector<actionElem>> &actionTable);
#endif