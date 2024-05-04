#include <bits/stdc++.h>
using namespace std;

// All the function Signatures. Functions are defined in bottom
bool checkOperatorNormal(char c);
bool checkKeyword(string s);
bool checkOperatorDouble(string s);
bool checkConstant(const string& str);
bool checkPunctuation(char c);
pair<bool,string> checkVariable(string s);
bool checkChar(char ch);

/*-------------------------------------------------Main Program-------------------------------------------------------*/
int main(){
    ifstream file("lex_input.txt");
    if (file.is_open()==false){ cerr << "File Reading Error!" << endl; return 0;}

    string datatype [] = {"char", "short", "int", "long", "float", "double", "bool", "enum"};
    map<string, int> varName;
    string sourceLine;
/*-------------------------------------------Checking for Errors-------------------------------------------------*/
    while (getline(file, sourceLine)){

        istringstream iss(sourceLine);
        string ele;
        vector<string>element;

        while(iss >> ele){
            element.push_back(ele);
        }

        for(int i = 0; i < element.size(); i++){
            if(element[i]=="#include" || element[i]=="using" || element[i]=="#define" || element[i].size()==0){
                element.clear();
                break;
            }

//////////////Validate Variable Name
            if(checkChar(element[i][0])){
                //Check Data-type or Variable
                bool datatypeflag = false;
                for(auto &type : datatype){

                    if(type == element[i]){

                        datatypeflag = true;
                        int sizee = element[i+1].size();

                        if(element[i+1][sizee-1] == ')'){
                            element[i+1].pop_back();
                            element[i+1].pop_back();
                        }

                        auto isvalidvar = checkVariable(element[i+1]);

                        if(isvalidvar.first == true){
                            varName[element[i+1]]++;
                            i++;
                            break;
                        }
                        else{
                            cout<<"\n\n"<<isvalidvar.second<<": "<<element[i+1]<<endl<<endl;
                            return 0;

                        }
                    }
                }
                if(checkKeyword(element[i])==true)    {break;}
                if(varName[element[i]]>0==true)    {break;}
                else if(!datatypeflag){
                    cout<<endl<<"\nElement    "<<element[i+1] <<"     not Declared in this Scope."<<endl<<endl;
                    return 0;
                }
            }
            else if(checkKeyword(element[i])==true){
                continue;
            }
        }
    }


/*-------------------------------------------There is no error, Creating Symbol Table-------------------------------------------------------*/

    vector<char> punctuations;
    vector<string> constants;
    vector<string> keywords;
    vector<string> operators;
    vector<string> identifiers;
    map<string, int> identiMap;

    ifstream symbolFile("lex_input.txt");
    if (symbolFile.is_open()==false){ cerr << "File Reading Error!" << endl; return 0;}

    while (getline(symbolFile, sourceLine)){
        istringstream iss(sourceLine);
        string ele;

        while (iss >> ele){
            if (ele == "#include" || ele == "using" || ele == "#define" || ele.size() == 0){
                break;
            }

            if(ele.back()==';'){
                punctuations.push_back(ele.back());
                ele.pop_back();
            }

            if (checkOperatorDouble(ele)==true){
                operators.push_back(ele);
            }
            else if (checkKeyword(ele)==true){
                keywords.push_back(ele);
            }
            else if (checkChar(ele[0])==true){
                if(identiMap[ele]<1){
                    identifiers.push_back(ele);
                    identiMap[ele]++;
                    if(ele.back() == ')'){
                        punctuations.push_back(ele[ele.size()-2]);
                        punctuations.push_back(ele.back());
                    }
                }

            }
            else if (checkConstant(ele)==true){
                constants.push_back(ele);
            }
            else{
                for (char c : ele){
                    if (checkPunctuation(c)==true){
                        punctuations.push_back(c);
                    }
                    else if (checkOperatorNormal(c)==true){
                        string op(1, c);
                        operators.push_back(op);
                    }
                }
            }
        }
    }

/*----------------------------------------------------------------------Print Symbol Table--------------------------------------------------------------------*/
    cout<< "\n\n------------------------------------------------Symbol Table------------------------------------------------"<<endl<<endl;
    cout << left << setw(28) << "  Keywords" << setw(24) << "Identifiers" << setw(22) << "Operators" << setw(27) << "Punctuations" << "Constants" << endl<<endl;

    for (size_t i = 0; i < max({keywords.size(), identifiers.size(), operators.size(), punctuations.size(), constants.size()}); ++i){

        cout << left << "  " <<setw(26) << (i < keywords.size() ? keywords[i] : "")
             << setw(24) << (i < identifiers.size() ? identifiers[i] : "")
             << setw(24)<< (i < operators.size() ? operators[i] : "")
             << setw(26) << (i < punctuations.size() ? string(1, punctuations[i]) : "")
             << (i < constants.size() ? constants[i] : "")<< endl;
    }

    return 0;
}

bool checkChar(char ch){ //Check Character or not
    if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){
        return true;
    }
    else{
        return false;
    }
}

bool checkOperatorNormal(char ch){// Check operator or not
    string operators = "+-*/%=><&|!^?:.";

    for(int i=0; i<operators.length(); i++){
        if(operators[i]==ch){
            return true;
        }
    }

    return false;
}

bool checkKeyword(string s){//Check a keyword or not
    string keyword[] ={"alignas", "alignof", "and", "and_eq", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "cin", "class", "compl", "const", "constexpr", "continue", "cout", "decltype", "default", "delete", "do", "double", "else", "endl", "enum", "explicit", "export", "extern", "false", "final", "float", "for", "friend", "goto", "if", "import", "insourceLine", "int", "long", "module", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "override", "private", "protected", "public", "register", "return", "short", "signed", "sizeof", "static", "static_assert", "struct", "switch", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "while", "xor", "xor_eq"};

    for(auto element : keyword){
        if(s==element){
            return true;
        }
    }

    return false;
}

bool checkOperatorDouble(string s){// Check a operator of double char or not
    string doubleOperators[] = {"==", ">>", "&&", "<<", ">=", "++", "+=", "-=", "*=", "--", "||", "/=", "!=", "<=", "%="};

    for(auto element : doubleOperators){
        if(s==element){
            return true;
        }
    }

    return false;
}

bool checkConstant(const string& str){// Check a constant element or not

    if(str[0] == '"' && str[str.size() - 1] == '"'){
        return true;
    }
    else if(str[0] == '\'' && str[str.size() - 1] == '\''){
        return true;
    }
    else if(all_of(str.begin(), str.end(), ::isdigit)){
        return true;
    }

    return false;
}

bool checkPunctuation(char ch){ //Check a punctuation or not
    string elements = "()[]{};:',.!&_/$#@|\"~^`";

    for(char c: elements){
        if(c==ch){
            return true;
        }
    }

    return false;
}

pair<bool,string> checkVariable(string s){ // Validate a Variable name
    string problem;

    if(s[0]>='0' && s[0] <= '9'){
        problem = "Variable Name can not contains digit at first. Wrong Declaration ";

        return make_pair(false,problem);
    }
    else if(checkKeyword(s)==true){
        problem = "Keywords can not be Variable Name. Wrong Declaration ";

        return make_pair(false,problem);
    }
    for(int i = 0; i < s.size(); i++){
        if(s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/' || s[i]=='%' || s[i]=='='){
            problem = "Variable Name can not contains operators. Wrong Declaration ";

            return make_pair(false,problem);
        }
        else if(((s[i]>='A' && s[i] <= 'Z')==false && (s[i]>='a' && s[i]<='z')==false) && ((s[i]>='0' && s[i] <= '9'))==false && s[i] != '_'){
            problem = "Variable Name can not contains Special Character. Wrong Declaration ";

            return make_pair(false,problem);
        }
    }


    return make_pair(true,problem);
}
