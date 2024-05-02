#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

enum type_of_lex {
    LEX_NULL,
    LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT,
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_WHILE, LEX_FOR, LEX_WRITE, LEX_STRING, LEX_STRUCT,
    LEX_GOTO, LEX_BREAK,
    LEX_FIN,
    LEX_BEGIN, LEX_END, LEX_SEMICOLON, LEX_COMMA, LEX_EQ, LEX_LPAREN, LEX_RPAREN, LEX_ASSIGN, LEX_LSS,
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_COLON,
    LEX_NUM,
    LEX_ID,
    LEX_STRING_DATA,
    POLIZ_GO, POLIZ_FGO, POLIZ_ADDRESS, POLIZ_LABEL, POLIZ_BUF, LEX_STRUCT_DEF
};

class Lex{
private:
    type_of_lex t_lex;
    long long v_lex;
    long num_row_in_file;
public:
    Lex(type_of_lex t = LEX_NULL, long long v = 0, long _num_row_in_file = 0);
    type_of_lex get_type() const;
    unsigned long get_value() const;
    friend std::ostream & operator<<(std::ostream &s, Lex l);
};

Lex::Lex(type_of_lex t, long long v, long _num_row_in_file): t_lex(t), v_lex(v), num_row_in_file(_num_row_in_file){}
type_of_lex Lex::get_type() const {
    return t_lex;
}
unsigned long Lex::get_value() const {
    return v_lex;
}

class Ident {
    std::string name;
    bool declare;
    type_of_lex type;
    bool assign;
    long long value;
    
public:
    Ident();
    Ident(const std::string n);
    bool operator==(const std::string& s) const ;
    bool operator==(const Ident& ID) const ;
    std::string get_name() const;
    bool get_declare() const;
    void put_declare();
    type_of_lex get_type() const;
    void put_type(type_of_lex t);
    bool get_assign() const;
    void put_assign();
    long long get_value() const;
    void put_value(long long v);
};

Ident::Ident(){
    declare = false;
    assign  = false;
}

Ident::Ident(const std::string n) {
    name    = n;
    declare = false;
    assign  = false;
}

bool Ident::operator==(const std::string& s) const {
    return name == s;
}

bool Ident::operator==(const Ident& ID) const {
    return name == ID.get_name();
}

std::string Ident::get_name() const {
    return name;
}
bool Ident::get_declare() const {
    return declare;
}
void Ident::put_declare(){
    declare = true;
}
type_of_lex Ident::get_type() const {
    return type;
}
void Ident::put_type(type_of_lex t) {
    type = t;
}
bool Ident::get_assign() const {
    return assign;
}
void Ident::put_assign() {
    assign = true;
}
long long Ident::get_value() const {
    return value;
}
void Ident::put_value(long long v) {
    value = v;
}

class Scanner{
private:
    FILE * fp;
    char c;
    long num_of_row_in_file;
    int look(const std::string buf, const char** list);
    void get_next_char_from_file();
    unsigned long put(const std::string & buf);
    friend std::ostream & operator<<(std::ostream &s, Lex l);
public:
    static std::vector<Ident> TID;
    static const char* TW [];
    static const char* TD [];
    std::vector<std::string> string_data;
    Scanner(const char* program);
    ~Scanner();
    Lex get_lex();
};
std::ostream & operator<<(std::ostream &s, Lex l);

Scanner::Scanner(const char* program){
    if (!(fp = fopen(program, "r")))  throw "can’t open file";
    num_of_row_in_file = 0;
}
int Scanner::look(const std::string buf, const char** list){
    int i = 0;
    while(list[i]){
        if(buf == list[i])
            return i;
        ++i;
    }
    return 0;
}
void Scanner::get_next_char_from_file(){
    c = fgetc(fp);
    if (c == '\n') num_of_row_in_file++;
}
Lex Scanner::get_lex(){
    enum state { H, IDENT, NUMB, ALE, NEQ, COM_OR_DIV, COM, STRING};
    int digit = 0;
    unsigned long num_row_table;
    std::string buf;
    state current_state = H;
    do {
        get_next_char_from_file();
        switch (current_state){
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t'){} 
                else 
                if(isalpha(c)){
                    buf.push_back(c);
                    current_state = IDENT;
                } 
                else 
                if(isdigit(c)){
                    digit = c - '0';
                    current_state = NUMB;
                } 
                else 
                if(c == '=' || c == '<' || c == '>'){
                    buf.push_back(c);
                    current_state = ALE;
                } 
                else 
                if(c == '@') return Lex(LEX_FIN);
                else 
                if(c == '!'){
                    buf.push_back(c);
                    current_state = NEQ;
                } 
                else 
                if(c == '/'){
                    buf.push_back(c);
                    current_state = COM_OR_DIV;
                } 
                else 
                if(c == '"') current_state = STRING;
                else {
                    buf.push_back(c);
                    if((num_row_table = look(buf, TD))) return Lex((type_of_lex)(num_row_table +(int)LEX_FIN), num_row_table, num_of_row_in_file);
                    else throw c;
                }
                break;
            
            case STRING:
                if (c != '"' and c != '@' and c != '\n') buf.push_back(c);
                else{
                    if (c == '@' or c == '\n') throw c;
                    else {
                        string_data.push_back(buf);
                        return Lex(LEX_STRING_DATA, string_data.size(), num_of_row_in_file);
                    }
                }
                break;
                
            case IDENT:
                if (isalpha(c)|| isdigit(c) || c == '.') buf.push_back(c);
                else {
                    ungetc(c, fp);
                    if ((num_row_table = look(buf, TW))) return Lex((type_of_lex)num_row_table, num_row_table, num_of_row_in_file);
                    else {
                        num_row_table = put(buf);
                        return Lex(LEX_ID, num_row_table, num_of_row_in_file);
                    }
                }
                break;
                
            case NUMB:
                if (isdigit(c)) digit = digit * 10 + (c - '0');
                else{
                    ungetc(c, fp);
                    return Lex(LEX_NUM, digit, num_of_row_in_file);
                }
                break;
                
            case COM_OR_DIV:
                if(c == '*') current_state = COM;
                else{
                    ungetc(c, fp);
                    if ((num_row_table = look(buf, TD))) return Lex((type_of_lex)(num_row_table +(int)LEX_FIN), num_row_table, num_of_row_in_file);
                    else throw c;
                }
                break;
                
            case COM:
                if (c == '*'){
                    get_next_char_from_file();
                    if (c == '/'){
                        buf = "";
                        current_state = H;
                    }
                }else 
                if(c == '@' || c == '{') throw c;
                break;
             
            case ALE:
                if (c == '='){
                    buf.push_back(c);
                    num_row_table = look(buf, TD);
                    return Lex((type_of_lex)(num_row_table +(int)LEX_FIN), num_row_table, num_of_row_in_file);
                }
                else {
                    ungetc(c, fp);
                    num_row_table = look(buf, TD);
                    return Lex((type_of_lex)(num_row_table +(int)LEX_FIN), num_row_table, num_of_row_in_file);
                }
                break;
            case NEQ:
                if (c == '='){
                    buf.push_back(c);
                    num_row_table = look(buf, TD);
                    return Lex(LEX_NEQ, num_row_table, num_of_row_in_file);
                } else throw '!';
                break;
        } //end switch
    } while(true);
}

unsigned long Scanner::put(const std::string & buf){
    std::vector<Ident>::iterator k;
    if((k = find(TID.begin(), TID.end(), buf)) != TID.end()) return k - TID.begin();
    TID.push_back(Ident(buf));
    return TID.size()- 1;
}

const char* Scanner::TW[] = {
    "", "and", "bool", "do", "else",
    "if", "false", "int", "not", "or", "program",
    "read", "true", "while", "for", "write", "string", "struct", "goto", "break", NULL
};

const char* Scanner::TD[] = {
    "@", "{", "}", ";", ",", "==", "(",
    ")", "=", "<", ">", "+", "-",
    "*", "/", "<=", "!=", ">=", ":", NULL
};

std::vector<Ident> Scanner::TID = {};

std::ostream & operator<<(std::ostream &s, Lex l){
    std::string t;
    if(l.t_lex <= LEX_BREAK)
        t = Scanner::TW[l.t_lex];
    else 
    if (l.t_lex == LEX_ASSIGN) t = "=";
    else 
    if(l.t_lex >= LEX_FIN && l.t_lex <= LEX_COLON) t = Scanner::TD[ l.t_lex - LEX_FIN ];
    else 
    if(l.t_lex == LEX_NUM) t = "NUMB";
    else 
    if(l.t_lex == LEX_ID) t = "ID: " + Scanner::TID[l.v_lex].get_name();
    else 
    if(l.t_lex == LEX_STRING_DATA) t = "STRING_DATA";
    else 
    if(l.t_lex == POLIZ_GO) t = "!";
    else 
    if(l.t_lex == POLIZ_FGO) t = "!F";
    else 
    if(l.t_lex == POLIZ_ADDRESS) t = "&";
    else 
    if(l.t_lex == POLIZ_LABEL) t = "label ";
    else 
    if(l.t_lex == POLIZ_BUF) t = "BUF";
    else throw t;
    s << '(' << t << ',' << l.v_lex << ")"<<std::endl;
    return s;
}


Scanner::~Scanner(){
    TID.clear();
}

class Parser{
private:
    struct Field_of_struct{
        std::string name;
        type_of_lex type;
        Field_of_struct(std::string _name, type_of_lex _type);
    };
    struct Struct{
        std::string name;
        std::vector<Field_of_struct> fields;
        Struct(std::string name, const std::vector<Field_of_struct>& fields = {});
    };
    enum type_ID{normal, mark, undeclared, not_reference_to_ID};
    Lex curr_lex;
    type_of_lex c_type;
    long long c_val;
    type_of_lex type_def;
    long long num_struct;
    long long current_id;
    int cycle_depth;
    const char* file_name;
    std::stack<type_of_lex> lex_stack;
    std::vector<Ident> marks;
    std::vector<size_t> break_to_position;
    std::vector<size_t> marks_positions_in_poliz;
    type_of_lex get_cur_type_def() const;
    void set_type_def(const type_of_lex type);
    bool is_redefinition(Lex lex);
    bool is_redefinition(std::string name, const std::vector<Field_of_struct>& fields);
    bool is_correct_name();
    void find_marks();
    void get_next_lex();
    void program();
    void definitions();
    void definition_of_structures();
    bool is_struct_name();
    void m_alloc_for_struct();
    std::vector<Field_of_struct> definition_of_structure_fields();
    void field();
    void operators();
    void variable();
    void constant();
    void S(bool flag = true);
    type_ID E();
    type_ID A();
    type_ID B();
    type_ID C();
    type_ID D();
    type_ID F();
    bool check_types();
    void push_to_poliz(std::stack<size_t>& positions, unsigned long position);
    void pop_break_positions();
    void pop_goto_positions();
public:
    Parser(const char* file_name_);
    Scanner scan;
    std::vector<Lex> poliz;
    std::vector<Struct> structures;
    void print_poliz();
    bool analyze();
};

Parser::Parser(const char* file_name_): scan(file_name_), file_name(file_name_) {  }
Parser::Field_of_struct::Field_of_struct(std::string _name, type_of_lex _type): name(_name), type(_type) {  }
Parser::Struct::Struct(std::string _name, const std::vector<Field_of_struct>& _fields): name(_name), fields(_fields) {  }
void Parser::get_next_lex(){
    curr_lex = scan.get_lex();
    c_type = curr_lex.get_type();
    c_val = curr_lex.get_value();
}
bool Parser::analyze(){
    try{
        find_marks();
        get_next_lex();
        program();
        pop_goto_positions();
        if (c_type != LEX_FIN) throw "Expected end" + std::to_string(c_type);
        std::cout << "Let's start\n ";
        return true;
    }
    catch(std::string message) {
        std::cout << message;
        std::cout << curr_lex;
        return false;
    }
    catch(char c){
        std::cout << "Unexpected character " << c << "\n";
        return false;
    }
}

void Parser::program(){
    if (c_type != LEX_PROGRAM) throw "Expected 'program' " + std::to_string(c_type);
    else{
        get_next_lex();
        if (c_type != LEX_BEGIN) throw "Expected '{' " + std::to_string(c_type);
        get_next_lex();
        definition_of_structures();
        scan.TID;
        definitions();
        operators();
        if (c_type != LEX_END) throw "Expected '}' " + std::to_string(c_type);
        get_next_lex();
    }
}

void Parser::definition_of_structures(){
    while (c_type == LEX_STRUCT){
        get_next_lex();
        if (c_type != LEX_ID) throw "Expected name of structure " + std::to_string(c_type);
        if (is_redefinition(curr_lex)) throw "Redefinition ID with num in TID " + std::to_string(c_val);
        scan.TID[c_val].put_type(LEX_STRUCT_DEF);
        scan.TID[c_val].put_value(structures.size());
        std::string name_struct = scan.TID.back().get_name();
        Struct cur_struct(name_struct);
        get_next_lex();
        if (c_type != LEX_BEGIN) throw "Expected '{' " + std::to_string(c_type);
        get_next_lex();
        cur_struct.fields = definition_of_structure_fields();
        structures.push_back(cur_struct);
        if (c_type != LEX_END) throw "Expected '}' " + std::to_string(c_type);
        get_next_lex();
        if (c_type != LEX_SEMICOLON) throw "Expected ';' " + std::to_string(c_type);
        get_next_lex();
    }
}

std::vector<Parser::Field_of_struct> Parser::definition_of_structure_fields(){
    std::vector<Field_of_struct> fields;
    while (c_type == LEX_INT or c_type == LEX_BOOL or
           c_type == LEX_STRING) {
        set_type_def(c_type);
        do {
            get_next_lex();
            if (c_type != LEX_ID) throw "Expected ID " + std::to_string(c_type);
            auto field = scan.TID.back();
            if(is_redefinition(field.get_name(), fields)) throw "Redefinition field of struct " + std::to_string(c_val);
            fields.push_back(Field_of_struct(field.get_name(), get_cur_type_def()));
            scan.TID.pop_back();
            get_next_lex();
        } while (c_type == LEX_COMMA);
        if (c_type != LEX_SEMICOLON) throw "Expected ';'" + std::to_string(c_type);
        get_next_lex();
    }
    return fields;
}

bool Parser::is_struct_name(){
    return c_type == LEX_ID and scan.TID[c_val].get_type() == LEX_STRUCT_DEF;
}

void Parser::definitions(){
    while (c_type == LEX_INT or c_type == LEX_BOOL or
           c_type == LEX_STRING or is_struct_name()){
        if (is_struct_name()){
            c_type = LEX_STRUCT;
            num_struct = c_val;
        }
        set_type_def(c_type);
        get_next_lex();
        variable();
        while (c_type == LEX_COMMA) {
            get_next_lex();
            variable();
        }
        if (c_type != LEX_SEMICOLON) throw "Expected ';'" + std::to_string(c_type);
        get_next_lex();
    }
}

void Parser::m_alloc_for_struct(){
    for (Ident& elem : scan.TID) {
        if (elem.get_type() == LEX_STRUCT) {
            long long num_of_struct = elem.get_value();
            std::string name_struct = elem.get_name();
            for (Field_of_struct& field : structures[num_of_struct].fields) {
                std::string name = name_struct + '.' + field.name;
                Ident new_id(name);
                new_id.put_declare();
                new_id.put_type(field.type);
                new_id.put_value(0);
                scan.TID.push_back(new_id);
            }
        }
    }
}

void Parser::variable(){
    if (c_type != LEX_ID) throw "Expected ID" + std::to_string(c_type);
    if (is_redefinition(curr_lex)) throw "Redefinition ID with num in TID " + std::to_string(c_val);
    if (!is_correct_name()) throw std::string("Invalid ID name ");
    scan.TID[c_val].put_declare();
    scan.TID[c_val].put_type(get_cur_type_def()); 
    current_id = c_val;
    get_next_lex();
    if (c_type == LEX_ASSIGN) {
        if (c_type == LEX_STRUCT) throw "Unexpected assign " + std::to_string(c_val);
        scan.TID[current_id].put_assign();
        get_next_lex();
        constant();
        get_next_lex();
    }
    else {
        if (get_cur_type_def() == LEX_STRUCT) {
            scan.TID[current_id].put_value(num_struct);
            long long num_of_struct = scan.TID[current_id].get_value();
            std::string name_struct = scan.TID[current_id].get_name();
            for (Field_of_struct& field : structures[num_of_struct].fields) {
                std::string name = name_struct + '.' + field.name;
                Ident new_id(name);
                new_id.put_declare();
                new_id.put_type(field.type);
                new_id.put_value(0);
                scan.TID.push_back(new_id);
            }
        } 
        else scan.TID[current_id].put_value(-1);
    }
}

void Parser::operators(){
    while (c_type != LEX_END) S();
}

bool Parser::is_correct_name(){
    std::string name = scan.TID[c_val].get_name();
    if (name.find('.') != -1) return scan.TID[c_val].get_declare();
    return true;
}

bool Parser::is_redefinition(Lex lex){
    return scan.TID[c_val].get_declare();
}

bool Parser::is_redefinition(std::string name, const std::vector<Field_of_struct>& fields){
    for (Field_of_struct field : fields)
        if (name == field.name) return true;
    return false;
}

void Parser::constant(){
    auto type = get_cur_type_def();
    if (c_type == LEX_STRING_DATA){
        if (type != LEX_STRING) throw std::string("Unexpected string declaration ");
        scan.TID[current_id].put_value(scan.string_data.size() - 1);
    } 
    else 
    if(c_type == LEX_MINUS or c_type == LEX_PLUS){
        long long value;
        if (c_type == LEX_MINUS) value = -1;
        else value = 1;
        get_next_lex();
        if (c_type == LEX_NUM){   
            value = value * c_val;
            scan.TID[current_id].put_value(value);
            if (type != LEX_INT) throw std::string("Unexpected number declaration ");
        } 
        else throw "Expected number " + std::to_string(c_type);
    } 
    else 
    if(c_type == LEX_NUM){
        if (type != LEX_INT) throw std::string("Unexpected number declaration ");
        scan.TID[current_id].put_value(c_val);   
    } 
    else 
    if(c_type == LEX_FALSE or c_type == LEX_TRUE){
        if (type != LEX_BOOL) throw std::string("Unexpected true/false declaration ");
        if (c_type == LEX_FALSE) scan.TID[current_id].put_value(0);
        else scan.TID[current_id].put_value(1);
    } 
    else throw "Expected constant" + std::to_string(c_type);
}

void Parser::S(bool flag){
    size_t l1, l2, l3, l4, insert_l1, insert_l2;
    if (flag)
        switch (c_type) {
            case LEX_IF:
                get_next_lex();
                if (c_type != LEX_LPAREN) throw "Expected '('" + std::to_string(c_type);
                get_next_lex();
                S(false);
                l1 = poliz.size();
                poliz.push_back(Lex(POLIZ_LABEL, 0)); 
                poliz.push_back(Lex(POLIZ_FGO, 0));
                if(lex_stack.top() != LEX_BOOL) throw std::string("'if' only works with bool type ");
                lex_stack.pop();
                if (c_type != LEX_RPAREN) throw "Expected ')'" + std::to_string(c_type);
                get_next_lex(); 
                S(); 
                l2 = poliz.size();
                poliz.push_back(Lex(POLIZ_LABEL, 0)); 
                poliz.push_back(Lex(POLIZ_GO, 0));
                if (c_type != LEX_ELSE) throw "Expected else " + std::to_string(c_type);
                get_next_lex();
                poliz[l1] = Lex(POLIZ_LABEL, poliz.size());
                S(); 
                poliz[l2] = Lex(POLIZ_LABEL, poliz.size());
                break;
            case LEX_WHILE:
                cycle_depth++;
                get_next_lex();
                if (c_type != LEX_LPAREN) throw "Expected '(' " + std::to_string(c_type);
                get_next_lex();
                l1 = poliz.size();
                S(false);
                l2 = poliz.size(); //position_to_end
                poliz.push_back(Lex(POLIZ_BUF));
                poliz.push_back(Lex(POLIZ_FGO));
                
                if(lex_stack.top() != LEX_BOOL){
                    throw std::string("'while' only works with bool type ");
                }
                lex_stack.pop();
                
                if (c_type != LEX_RPAREN){
                    throw "Expected ')'" + std::to_string(c_type);
                }
                get_next_lex();
                
                S();
                poliz.push_back(Lex(POLIZ_LABEL, l1));
                poliz.push_back(Lex(POLIZ_GO, l1));
                
                poliz[l2] = Lex(POLIZ_LABEL, poliz.size());
                
                pop_break_positions();
                cycle_depth--;
                break;
            case LEX_FOR:
                cycle_depth++;
                get_next_lex();
                if (c_type != LEX_LPAREN){
                    throw "Expected '('" + std::to_string(c_type);
                }
                get_next_lex();
                
                // a
                if (c_type == LEX_SEMICOLON) {
                    get_next_lex();
                } else {
                    S(false); //E1
                    poliz.push_back(Lex(LEX_SEMICOLON));
                    lex_stack.pop();
                    if (c_type != LEX_SEMICOLON) {
                        throw "Expected ';' " + std::to_string(c_type);
                    }
                    get_next_lex();
                }
                
                l3 = poliz.size();
                
                // b
                if (c_type == LEX_SEMICOLON) {
                    get_next_lex();
                    throw std::string("'for(a;b;c)' b can't be empty ");
                } else {
                    S(false); //E2
                    insert_l1 = poliz.size();
                    poliz.push_back(Lex(POLIZ_BUF)); // mark to l1
                    poliz.push_back(Lex(POLIZ_FGO));
                    insert_l2 = poliz.size();
                    poliz.push_back(Lex(POLIZ_BUF)); // mark to l2
                    poliz.push_back(Lex(POLIZ_GO));
                    
                    l4 = poliz.size();
                    
                    if(lex_stack.top() != LEX_BOOL){
                        throw std::string("'for(a;b;c)' b must be bool type ");
                    }
                    lex_stack.pop();
                    if (c_type != LEX_SEMICOLON) {
                        throw "Expected ';' " + std::to_string(c_type);
                    }
                    get_next_lex();
                }
                
                // c
                if (c_type == LEX_SEMICOLON) {
                    get_next_lex();
                } else {
                    S(false); //E3
                    poliz.push_back(Lex(POLIZ_LABEL, l3));
                    poliz.push_back(Lex(POLIZ_GO));
                    lex_stack.pop();
                }
                
                l2 = poliz.size();
                
                if (c_type != LEX_RPAREN){
                    throw "Expected ')'" + std::to_string(c_type);
                }
                get_next_lex();
                
                S(); //S
                
                poliz.push_back(Lex(POLIZ_LABEL, l4));
                poliz.push_back(Lex(POLIZ_GO));
                l1 = poliz.size();
                
                poliz[insert_l1] = Lex(POLIZ_LABEL, l1);
                poliz[insert_l2] = Lex(POLIZ_LABEL, l2);
                pop_break_positions();

                cycle_depth--;
                break;
            case LEX_BREAK:
                
                if (cycle_depth == 0) {
                    throw "Unexpected 'break' " + std::to_string(c_type);
                }
                get_next_lex();
                
                break_to_position.push_back(poliz.size());
                poliz.push_back(Lex(POLIZ_BUF));
                poliz.push_back(Lex(POLIZ_GO));
                
                if (c_type != LEX_SEMICOLON){
                    throw "Expected ';'" + std::to_string(c_type);
                }
                get_next_lex();
                break;
            case LEX_GOTO:
                get_next_lex();
                
                if (c_type == LEX_ID){
                    auto iter = std::find(marks.begin(), marks.end(), scan.TID[c_val]);
                    
                    if (iter == marks.end()){
                        throw "Undeclarated mark " + std::to_string(c_type);
                    }
                    
                    auto num_mark = iter - marks.begin();
                    
                    poliz.push_back(Lex(POLIZ_BUF, num_mark));
                    poliz.push_back(Lex(POLIZ_GO));
                } else {
                    throw "Expected ID " + std::to_string(c_type);
                }
                get_next_lex();
                
                if (c_type != LEX_SEMICOLON){
                    throw "Expected ';' " + std::to_string(c_type);
                }
                get_next_lex();
                
                break;
            case LEX_READ:
                get_next_lex();
                if (c_type != LEX_LPAREN) {
                    throw "Expected '('  " + std::to_string(c_type);
                }
                get_next_lex();
                
                
                if (c_type != LEX_ID){
                    throw "Expected ID  " + std::to_string(c_type) + "\n";
                }
                
                poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
                poliz.push_back(Lex(LEX_READ));
                
                get_next_lex();
                
                if (c_type != LEX_RPAREN){
                    throw "Expected ')' " + std::to_string(c_type);
                }
                
                get_next_lex();
                
                if (c_type != LEX_SEMICOLON){
                    throw "Expected ';'" + std::to_string(c_type);
                }
                get_next_lex();
                break;
                
            case LEX_WRITE:
                get_next_lex();
                if (c_type != LEX_LPAREN) {
                    throw "Expected '('" + std::to_string(c_type);
                }
                get_next_lex();
                
                
                S(false);
                lex_stack.pop();
                poliz.push_back(Lex(LEX_WRITE));
                while (c_type == LEX_COMMA) {
                    get_next_lex();
                    S(false);
                    lex_stack.pop();
                    poliz.push_back(Lex(LEX_WRITE));
                }
                
                
                if (c_type != LEX_RPAREN){
                    throw "Expected ')' " + std::to_string(c_type);
                }
                
                get_next_lex();
                
                if (c_type != LEX_SEMICOLON){
                    throw "Expected ';'  " + std::to_string(c_type);
                }
                get_next_lex();
                break;
            case LEX_BEGIN:
                get_next_lex();
                operators();
                
                if (c_type != LEX_END){
                    throw "Expected '}' " + std::to_string(c_type);
                }
                
                get_next_lex();
                
                break;
                
            default:
                // {_E=_}E
                type_ID type;
                int iteration;
                
                for (iteration = 0; (type = E()) != not_reference_to_ID; iteration++) {
                    
                    if (type == undeclared) {
                        throw "Undeclared ID " + std::to_string(c_type);
                    }
                    
                    if (c_type == LEX_ASSIGN) {
                        //E=E=E...
                        
                        if (type != normal){
                            throw std::string("Unexpected assign ");
                        }
                        
                        // swap top elem of poliz (ID) to address
                        Lex top_elem = poliz.back();
                        poliz.pop_back();
                        poliz.push_back(Lex(POLIZ_ADDRESS, top_elem.get_value()));
                        
                    } else if (c_type == LEX_COLON) {
                        //ID:operator
                        
                        if (type != mark) {
                            throw "Unexpected ':' after non-mark ID " + std::to_string(c_type);
                        }
                        
                        get_next_lex();
                        S();
                        return;
                    } else if (c_type == LEX_SEMICOLON) {
                        break;
                    } else {
                        if (iteration > 0)
                            throw "Expected '='" + std::to_string(c_type);
                        else
                            throw "Expected '=' or ':'  " + std::to_string(c_type);
                    }
                    get_next_lex();
                }
                
                if (c_type != LEX_SEMICOLON){
                    throw "Expected ';' " + std::to_string(c_type);
                }
                
                // add assingn to poliz
                int cout_assingn = iteration;
                
                while (cout_assingn > 0) {
                    poliz.push_back(Lex(LEX_ASSIGN));
                    cout_assingn--;
                }
                
                poliz.push_back(Lex(LEX_SEMICOLON));
                // check that every type of element match
                type_of_lex type_stack = lex_stack.top();
                while (iteration >= 0) {
                    if (type_stack != lex_stack.top()) {
                        throw std::string("Type error");
                    }
                    lex_stack.pop();
                    
                    iteration--;
                }
                //there isn't type of operation in stack!
                
                get_next_lex();
                break;
        }
    
    else {
        int iteration = 0;
        type_ID type;
        while ((type = E()) == normal and c_type == LEX_ASSIGN) {
            get_next_lex();
            Lex top_elem = poliz.back();
            poliz.pop_back();
            poliz.push_back(Lex(POLIZ_ADDRESS, top_elem.get_value()));
            iteration++;
        }
        
        if (type == undeclared) {
            throw "Undeclared ID " + std::to_string(c_type);
        }
        
        // add assingn to poliz
        int cout_assingn = iteration;
        
        while (cout_assingn > 0) {
            poliz.push_back(Lex(LEX_ASSIGN));
            cout_assingn--;
        }
        
        type_of_lex type_stack = lex_stack.top();
        while (iteration >= 0) {
            if (type_stack != lex_stack.top()) {
                throw std::string("Type error ");
            }
            lex_stack.pop();
            
            iteration--;
        }
        
        lex_stack.push(type_stack);
        //there is type of operation in stack!
    }
}

Parser::type_ID Parser::E(){
    type_ID type = A();
    
    std::stack<size_t> positions;

    if (c_type == LEX_OR){
        // add if false goto next
        size_t position_next = poliz.size();
        poliz.push_back(Lex(POLIZ_BUF));
        poliz.push_back(Lex(POLIZ_FGO));

        // add goto end
        //poliz.push_back(Lex(LEX_TRUE));
        positions.push(poliz.size());
        poliz.push_back(Lex(POLIZ_BUF));
        poliz.push_back(Lex(POLIZ_GO));
        poliz[position_next] = Lex(POLIZ_LABEL, poliz.size());
        while (c_type == LEX_OR) {
            
            lex_stack.push(c_type);
            
            get_next_lex();
            
            A();
            
            // add if false goto next
            position_next = poliz.size();
            poliz.push_back(Lex(POLIZ_BUF));
            poliz.push_back(Lex(POLIZ_FGO));

            // add goto end, true
            //poliz.push_back(Lex(LEX_TRUE));
            positions.push(poliz.size());
            poliz.push_back(Lex(POLIZ_BUF));
            poliz.push_back(Lex(POLIZ_GO));
            poliz[position_next] = Lex(POLIZ_LABEL, poliz.size());

            check_types();
            
            type = not_reference_to_ID;
        }
        // add false
        poliz.push_back(Lex(LEX_FALSE));
        // goto end
        size_t position_insert_end = poliz.size();
        poliz.push_back(Lex(POLIZ_BUF));
        poliz.push_back(Lex(POLIZ_GO));
        
        poliz[position_insert_end] = Lex(POLIZ_LABEL, poliz.size());
        push_to_poliz(positions, poliz.size());
        
        poliz.push_back(Lex(LEX_TRUE));
        
        
    }
    return type;
}

void Parser::push_to_poliz(std::stack<size_t>& positions, unsigned long position){
    while (!positions.empty()) {
        poliz[positions.top()] = Lex(POLIZ_LABEL, position);
        positions.pop();
    }
}

Parser::type_ID Parser::A(){
    
    std::stack<size_t> positions;
    
    type_ID type = B();
    
    if (c_type == LEX_AND) {
        //lazy evaluation
        positions.push(poliz.size());
        poliz.push_back(Lex(POLIZ_BUF));
        poliz.push_back(Lex(POLIZ_FGO));
        // add goto end with false
        while (c_type == LEX_AND) {
            
            lex_stack.push(c_type);
            
            get_next_lex();
            B();
            
            //lazy evaluation
            positions.push(poliz.size());
            poliz.push_back(Lex(POLIZ_BUF));
            poliz.push_back(Lex(POLIZ_FGO));
            
            check_types();
            
            type = not_reference_to_ID;
        }
        //add true and goto next
        
        //lazy evaluation
        poliz.push_back(Lex(LEX_TRUE));
        size_t position_goto_next = poliz.size();
        poliz.push_back(Lex(POLIZ_BUF));
        poliz.push_back(Lex(POLIZ_GO));
        
        unsigned long position_false = poliz.size();
        push_to_poliz(positions, position_false);
        
        poliz.push_back(Lex(LEX_FALSE));
        poliz[position_goto_next] = Lex(POLIZ_LABEL, poliz.size());
    }
    return type;
}

Parser::type_ID Parser::B(){
    type_ID type = C();
    if (c_type == LEX_EQ or c_type == LEX_NEQ or c_type == LEX_GEQ or c_type == LEX_LEQ or c_type == LEX_LSS or c_type == LEX_GTR) {
        
        lex_stack.push(c_type);
        
        get_next_lex();
        C();
        
        check_types();
        
        type = not_reference_to_ID;
    }
    
    return type;
}
Parser::type_ID Parser::C(){
    type_ID type = D();
    while (c_type == LEX_MINUS or c_type == LEX_PLUS) {
        
        lex_stack.push(c_type);
        
        get_next_lex();
        D();
        
        check_types();
        
        type = not_reference_to_ID;
    }
    
    return type;
}
Parser::type_ID Parser::D(){
    type_ID type = F();
    while (c_type == LEX_TIMES or c_type == LEX_SLASH) {
        
        lex_stack.push(c_type);
        
        get_next_lex();
        F();
        
        check_types();
        
        type = not_reference_to_ID;
    }
    
    return type;
}

Parser::type_ID Parser::F(){
    type_ID type = not_reference_to_ID;
    if (c_type == LEX_ID) {
        if(!scan.TID[c_val].get_declare()){
            // mark?
            auto iterator = std::find(marks.begin(), marks.end(), scan.TID[c_val]);
            if (iterator != marks.end()){
                auto num_mark = iterator - marks.begin();
                marks_positions_in_poliz[num_mark] = poliz.size();
                type = mark;
            } else {
                type = undeclared;
            }
        } else {
            poliz.push_back(Lex(LEX_ID, c_val));
            type = normal;
        }
        // push type of ID
        lex_stack.push(scan.TID[c_val].get_type());
        
        get_next_lex();
    } else if (c_type == LEX_NUM) {
        //poliz
        poliz.push_back(Lex(LEX_NUM, c_val));
        //___
        lex_stack.push(LEX_INT);
        get_next_lex();
    } else if (c_type == LEX_TRUE) {
        //poliz
        poliz.push_back(Lex(LEX_TRUE));
        //___
        lex_stack.push(LEX_BOOL);
        get_next_lex();
    } else if (c_type == LEX_FALSE) {
        //poliz
        poliz.push_back(Lex(LEX_FALSE));
        //___
        
        lex_stack.push(LEX_BOOL);
        get_next_lex();
    } else if (c_type == LEX_STRING_DATA) {
        //poliz
        auto num_of_row = scan.string_data.size() - 1;
        poliz.push_back(Lex(LEX_STRING_DATA, num_of_row));
        //___
        lex_stack.push(LEX_STRING);
        get_next_lex();
    } else if (c_type == LEX_NOT) {
        get_next_lex();
        F();
        //check not
        //poliz
        poliz.push_back(Lex(LEX_NOT));
        //___
        if (lex_stack.top() != LEX_BOOL){
            throw std::string("'Not' operation is not applicable to this type ");
        }
    } else if (c_type == LEX_LPAREN) {
        get_next_lex();
        S(false);
        if (c_type == LEX_RPAREN) {
            get_next_lex();
        } else {
            throw "Expected ')'" + std::to_string(c_type);;
        }
    } else if (c_type == LEX_MINUS or c_type == LEX_PLUS) {
        get_next_lex();
        
        poliz.push_back(Lex(LEX_NUM, 0));
        F();
        if (lex_stack.top() != LEX_INT){
            throw std::string("Unary operations do not support this type ");
        }
        poliz.push_back(Lex(LEX_MINUS, 0));
        
    } else {
        throw "Unexpected lex with id " + std::to_string(c_type);
    }
    
    return type;
}

type_of_lex Parser::get_cur_type_def() const {
    return type_def;
}

void Parser::set_type_def(const type_of_lex type){
    type_def = type;
}


void Parser::find_marks(){
    Scanner scan(file_name);
    
    Lex cur_lex = scan.get_lex(), prev_lex;
    
    //empty file
    if(cur_lex.get_type() == LEX_FIN){
        return;
    }
    
    prev_lex = cur_lex;
    
    while((cur_lex = scan.get_lex()).get_type() != LEX_FIN){
        if(cur_lex.get_type() == LEX_COLON and prev_lex.get_type() == LEX_ID){
            
            auto iter = std::find(marks.begin(), marks.end(), scan.TID[prev_lex.get_value()]);
            
            if (iter != marks.end()){
                throw std::string("Identical marks found ");
            }
            
            marks.push_back(scan.TID[prev_lex.get_value()]);
        }
        prev_lex = cur_lex;
    }
    marks_positions_in_poliz = std::vector<size_t>(marks.size());
    
    for (Ident mark : marks) {
        std::string name = mark.get_name();
        if (name.find('.') != -1) {
            throw std::string("Invalid mark name ");
        }
    }
}

bool Parser::check_types(){
    type_of_lex sec_op = lex_stack.top();
    lex_stack.pop();
    type_of_lex operation = lex_stack.top();
    lex_stack.pop();
    type_of_lex first_op = lex_stack.top();
    lex_stack.pop();
    
    type_of_lex result;
    if (operation != LEX_OR and operation != LEX_AND) {
        poliz.push_back(Lex(operation));
    }
    type_of_lex type_of_op = first_op;
    if(first_op != sec_op){
        throw std::string("Type error ");
    }
    
    if (type_of_op == LEX_STRING){
        if(operation == LEX_PLUS){
            result = LEX_STRING;
        } else if (operation == LEX_LSS or operation == LEX_GTR or
                   operation == LEX_GEQ or operation == LEX_LEQ or
                   operation == LEX_EQ or operation == LEX_NEQ){
            result = LEX_BOOL;
        } else {
            throw std::string("Invalid operation with string type ");
        }
    } else if (type_of_op == LEX_INT){
        if(operation == LEX_PLUS or operation == LEX_MINUS or
           operation == LEX_TIMES or operation == LEX_SLASH){
            result = LEX_INT;
        } else if (operation == LEX_LSS or operation == LEX_GTR or
                   operation == LEX_GEQ or operation == LEX_LEQ or
                   operation == LEX_EQ or operation == LEX_NEQ){
            result = LEX_BOOL;
        } else {
            throw std::string("Invalid operation with int type ");
        }
    } else if (type_of_op == LEX_BOOL){
        if(operation == LEX_OR or operation == LEX_AND){
            result = LEX_BOOL;
        } else {
            throw std::string("Invalid operation with bool type ");
        }
    } else {
        throw std::string("Invalid type ");
    }
    
    lex_stack.push(result);
    
    return false;
}

void Parser::print_poliz(){
    for (Lex curr_elem : poliz) {
        std::cout << curr_elem << " ";
    }
}

void Parser::pop_break_positions(){
    while (!break_to_position.empty()) {
        size_t position = break_to_position.back();
        break_to_position.pop_back();
        poliz.size();
        poliz[position] = Lex(POLIZ_LABEL, poliz.size());
    }
}


void Parser::pop_goto_positions(){
    for (Lex& elem_in_poliz : poliz) {
        if (elem_in_poliz.get_type() == POLIZ_BUF) {
            elem_in_poliz = Lex(POLIZ_LABEL, marks_positions_in_poliz[elem_in_poliz.get_value()]);
        }
    }
}

class Interpreter{
private:
    Parser parser;
    long long get_value_op(Lex op);
    type_of_lex get_type_exeption(Lex op);
    type_of_lex get_type_address(Lex op);
    long long get_int(Lex op);
    std::string get_string(Lex op);
    bool get_bool(Lex op);
    template <class T>
    bool cmp(type_of_lex type_cmp, T op1, T op2);
    void assign_struct(long long dst, long long src);
public:
    explicit Interpreter(const char* file_name);
    void run();
};

Interpreter::Interpreter(const char* file_name): parser(file_name){}
void Interpreter::run(){
    if (!parser.analyze()) throw "Parser throw\n";
    parser.print_poliz();
    std::stack<Lex> stack;
    auto poliz = parser.poliz;
    unsigned long position = 0;
    while (position != poliz.size()) {
        type_of_lex type_lex = poliz[position].get_type();
        if (type_lex == LEX_ID or type_lex == LEX_NUM or type_lex == LEX_STRING_DATA or type_lex == LEX_TRUE or type_lex == LEX_FALSE or type_lex == POLIZ_LABEL or type_lex == POLIZ_ADDRESS) {
            stack.push(poliz[position]);
        }
        else 
        if(type_lex == LEX_PLUS or type_lex == LEX_MINUS or type_lex == LEX_SLASH or type_lex == LEX_TIMES){
            auto op2 = stack.top();
            stack.pop();
            auto op1 = stack.top();
            stack.pop();
            type_of_lex type_ex = get_type_exeption(op1);
            if (type_ex == LEX_STRING) {
                std::string s1 = get_string(op1);
                std::string s2 = get_string(op2);
                stack.push(Lex(LEX_STRING_DATA, parser.scan.string_data.size()));
                parser.scan.string_data.push_back(s1 + s2);
            }
            else 
            if(type_ex == LEX_INT){
                long long result, val1 = get_int(op1), val2 = get_int(op2);
                switch (type_lex){
                    case LEX_PLUS:
                        result = val1 + val2;
                        break;
                    case LEX_MINUS:
                        result = val1 - val2;
                        break;
                    case LEX_SLASH:
                        result = val1 / val2;
                        break;
                    case LEX_TIMES:
                        result = val1 * val2;
                        break;
                    default:
                        throw "Bug";
                        break;
                }
                stack.push(Lex(LEX_NUM, result));
            }
        }
        else 
        if(type_lex == LEX_BOOL) {} 
        else 
        if(type_lex == POLIZ_GO){
            auto op = stack.top();
            stack.pop();
            position = op.get_value();
            continue;
        } 
        else 
        if(type_lex == POLIZ_FGO){
            auto op2 = stack.top();
            stack.pop();
            auto op1 = stack.top();
            stack.pop();
            if (!get_bool(op1)){
                position = op2.get_value();
                continue;
            } 
        }
        else 
        if(type_lex == LEX_WRITE){
            auto op = stack.top();
            stack.pop();
            type_of_lex type_ex = get_type_exeption(op);
            if (type_ex == LEX_STRING) std::cout << "Output: " << get_string(op) << std::endl;
            else 
            if(type_ex == LEX_INT) std::cout << "Output: " << get_int(op) << std::endl;
            else 
            if(type_ex == LEX_BOOL){
                if (get_bool(op)) std::cout << "Output: " << "TRUE" << std::endl;
                else std::cout << "Output: " << "FALSE" << std::endl;
            } 
            else throw "Bug";
        }
        else 
        if(type_lex == LEX_ASSIGN){
            auto src = stack.top();
            stack.pop();
            auto dst = stack.top();
            stack.pop();
            type_of_lex type_ex = get_type_address(dst);
            if (type_ex == LEX_STRING) {
                auto position_string_data = parser.scan.string_data.size();
                parser.scan.string_data.push_back(get_string(src));
                parser.scan.TID[dst.get_value()].put_value(position_string_data);
                stack.push(Lex(LEX_STRING_DATA, position_string_data));
            } 
            else 
            if(type_ex == LEX_INT){
                long long value = get_int(src);
                parser.scan.TID[dst.get_value()].put_value(value);
                stack.push(Lex(LEX_NUM, value));
            } 
            else 
            if(type_ex == LEX_BOOL){
                bool value = get_bool(src);
                type_of_lex value_to_stack;
                value ? value_to_stack = LEX_TRUE : value_to_stack = LEX_FALSE;
                parser.scan.TID[dst.get_value()].put_value(value);
                stack.push(Lex(value_to_stack));
            } 
            else 
            if(type_ex == LEX_STRUCT){
                size_t type_dst = parser.scan.TID[dst.get_value()].get_value();
                size_t type_src = parser.scan.TID[src.get_value()].get_value();
                if (type_dst != type_src) throw "Type error ";
                assign_struct(dst.get_value(), src.get_value());
                stack.push(dst);
            }
        } 
        else 
        if(type_lex == LEX_SEMICOLON) stack.pop();
        else 
        if(type_lex == LEX_LSS or type_lex == LEX_LEQ or
                   type_lex == LEX_GTR or type_lex == LEX_GEQ or
                   type_lex == LEX_EQ or type_lex == LEX_NEQ){
            auto op2 = stack.top();
            stack.pop();
            auto op1 = stack.top();
            stack.pop();
            type_of_lex type_ex = get_type_exeption(op1);
            bool result;
            if(type_ex == LEX_STRING){
                std::string s1 = get_string(op1);
                std::string s2 = get_string(op2);
                result = cmp(type_lex, s1, s2);
            } 
            else{
                long long val1 = get_int(op1), val2 = get_int(op2);
                result = cmp(type_lex, val1, val2);
            }
            if (result) stack.push(Lex(LEX_TRUE));
            else stack.push(Lex(LEX_FALSE));
        } 
        else 
        if(type_lex == LEX_READ){
            auto dst = stack.top();
            stack.pop();
            type_of_lex type_ex = get_type_address(dst);
            if(type_ex == LEX_INT){
                long long input;
                std::cout<<"Input: ";
                std::cin >> input;
                parser.scan.TID[dst.get_value()].put_value(input);
            } 
            else 
            if(type_ex == LEX_STRING){
            	std::cout<<"Input: ";
                std::string input;
                std::cin >> input;
                parser.scan.TID[dst.get_value()].put_value(parser.scan.string_data.size());
                parser.scan.string_data.push_back(input);
            }
            else 
            if(type_ex == LEX_BOOL){
                bool input;
                std::cout<<"Input: ";
                std::cin >> input;
                parser.scan.TID[dst.get_value()].put_value(input);
            }
        }
        position++;
    }
}

long long Interpreter::get_value_op(Lex op){
    long long value;
    if(op.get_type() == LEX_ID) value = parser.scan.TID[op.get_value()].get_value();
    else 
    if(op.get_type() == LEX_NUM or op.get_type() == LEX_STRING_DATA) value = op.get_value();
    else throw "Bug";
    return value;
}

type_of_lex Interpreter::get_type_exeption(Lex op){
    if(op.get_type() == LEX_STRING_DATA) return LEX_STRING;
    else 
    if(op.get_type() == LEX_NUM) return LEX_INT;
    else
    if(op.get_type() == LEX_FALSE or op.get_type() == LEX_TRUE) return LEX_BOOL;
    else return parser.scan.TID[op.get_value()].get_type();
}

type_of_lex Interpreter::get_type_address(Lex op){
    if (op.get_type() != POLIZ_ADDRESS) throw "Bug";
    return parser.scan.TID[op.get_value()].get_type();
}

long long Interpreter::get_int(Lex op){
    long long value;
    if(op.get_type() == LEX_ID) value = parser.scan.TID[op.get_value()].get_value();
    else 
    if(op.get_type() == LEX_NUM) value = op.get_value();
    else throw "Bug";
    return value;
}

std::string Interpreter::get_string(Lex op){
    std::string result;
    if(op.get_type() == LEX_ID){
        size_t num_of_row_string_data = parser.scan.TID[op.get_value()].get_value();
        result = parser.scan.string_data[num_of_row_string_data];
    } 
    else 
    if(op.get_type() == LEX_STRING_DATA){
        size_t num_of_row_string_data = op.get_value();
        result = parser.scan.string_data[num_of_row_string_data];
    }
    else throw "Bug";
    return result;
}

bool Interpreter::get_bool(Lex op){
    if(op.get_type() == LEX_ID) return parser.scan.TID[op.get_value()].get_value();
    else 
    if(op.get_type() == LEX_FALSE or op.get_type() == LEX_TRUE) return op.get_type() == LEX_TRUE;
    else throw "Bug";
}

template <class T>
bool Interpreter::cmp(type_of_lex type_cmp, T val1, T val2){
    bool result;
    switch (type_cmp) {
        case LEX_LEQ:
            result = val1 <= val2;
            break;
        case LEX_LSS:
            result = val1 < val2;
            break;
        case LEX_GTR:
            result = val1 > val2;
            break;
        case LEX_GEQ:
            result = val1 >= val2;
            break;
        case LEX_EQ:
            result = val1 == val2;
            break;
        case LEX_NEQ:
            result = val1 != val2;
            break;
        default:
            throw "Bug";
            break;
    }
    return result;
}

void Interpreter::assign_struct(long long dst, long long src){
    if (dst == src) return;
    auto num_declare = parser.scan.TID[dst].get_value();
    auto count_fields = parser.structures[num_declare].fields.size();
    for (int i = 1; i <= count_fields; i++) parser.scan.TID[dst + i] = parser.scan.TID[src + i];
}

int main(int argc, char** argv){  
    auto file_name = argv[1];
    std::cout<<file_name<<'\n';
    Interpreter intetpreter(file_name);
    try {
        intetpreter.run();
    } catch (const char* message) {
        std::cout << message;
    }
    std::cout<<"The End\n";
    return 0;
}