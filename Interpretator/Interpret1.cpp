#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
using namespace std;



enum type_of_lex {
	LEX_NULL, // 0
	LEX_AND,
	LEX_ELSE,
	LEX_IF,
	LEX_FALSE,
	LEX_INT, 
	LEX_STRING, 
	LEX_BOOL,
	LEX_NOT,
	LEX_OR,
	LEX_PROGRAM,
	LEX_READ,
	LEX_TRUE,
	LEX_WHILE,
	LEX_FOR,
	LEX_GOTO,
	LEX_WRITE,
	LEX_STRUCT,
	LEX_BREAK,
	LEX_FIN,
	LEX_SEMICOLON,
	LEX_COLON,
	LEX_COMMA,
	LEX_DOT,
	LEX_ASSIGN,
	LEX_LPAREN,
	LEX_RPAREN,
	LEX_LBRACE,
	LEX_RBRACE,
	LEX_EQ,
	LEX_LSS,
	LEX_GTR,
	LEX_PLUS,
	LEX_MINUS,
	LEX_TIMES,
	LEX_SLASH,
	LEX_LEQ,
	LEX_NEQ,
	LEX_GEQ,
	LEX_NUM,
	LEX_CONSTSTR,
	LEX_ID,
	LEX_UNARY_MINUS,
	POLIZ_LABEL,
	POLIZ_ADDRESS,
	POLIZ_GO,
	POLIZ_FGO
};

class Lex {
 private:
	type_of_lex t_lex;
	long long v_lex;
 public:
	Lex(type_of_lex t = LEX_NULL, long long v = 0) {
		t_lex = t;
		v_lex = v;
	}
	type_of_lex get_type() const {
		return t_lex;
	}
	long long get_value() const {
		return v_lex;
	}
	friend int LexInd(const type_of_lex* arr, type_of_lex x);
	friend ostream& operator<<(std::ostream& s, Lex l);
};
class Ident {
 private:
	char* name;
	bool declare;
	type_of_lex type;
	bool assign;
	long long value;

	bool expected_in_goto;
	int point_of_goto;

 public:
	Ident() {
		declare = false;
		assign = false;
		expected_in_goto = false;}
	char* get_name() {
		return name;
	}
	void put_name(const char* n) {
		name = new char[strlen(n) + 1];
		strcpy(name, n);
	}
	bool get_expected_in_goto() const{
		return expected_in_goto;
	}
	void put_expected_in_goto() {
		expected_in_goto = true;
	}
	int get_point_of_goto() const{
		return point_of_goto;
	}
	void put_point_of_goto(int p) {
		point_of_goto = p;
	}
	bool get_declare() const {
		return declare;
	}
	void put_declare() {
		declare = true;
	}
	type_of_lex get_type() {
		return type;
	}
	void put_type(type_of_lex t) {
		type = t;
	}
	bool get_assign() const {
		return assign;
	}
	void put_assign() {
		assign = true;
	}
	long long get_value() const {
		return value;
	}
	void put_value(long long v) {
		value = v;
	}
};

class Tabl_ident {
 private:
	Ident* p;
	long long size;
	int top;

 public:
	Tabl_ident(long long max_size) {
		p = new Ident[size = max_size];
		top = 1;
	}
	~Tabl_ident() {
		delete[] p;
	}
	void pop_back(){
		top--;
		if (top == 1){
			throw "end_of_tabl";
		}
	}
	long long get_size() const{
		return size;
	}
	Ident& operator[](long long k) {
		return p[k];
	}
	int put(const char* buf);
};
int Tabl_ident::put(const char* buf) {
	for (int j = 1; j < top; ++j)
		if (!strcmp(buf, p[j].get_name()))
			return j;
	p[top].put_name(buf);
	++top;
	return top - 1;
}
class Scanner {
 private:
	enum state {
		H, IDENT, NUMB, COM, ALE, DELIM, NEQ, CONSTSTR, LINE_COM, MULTLINE_COM
	};

	state CS;
	FILE* fp;
	int c;
	char buf[80];
	int buf_top;

	void clear() {
		buf_top = 0;
		for (int j = 0; j < 80; ++j)
			buf[j] = '\0';
	}
	void add() {
		buf[buf_top++] = c;
	}
	int look(const char* buf, const char** list) {
		int i = 0;
		while (list[i]) {
			if (!strcmp(buf, list[i]))
				return i;
			++i;
		}
		return 0;
	}
	void gc() {
		c = fgetc(fp);
	}

 public:
	const static char* TW[];
	const static type_of_lex words[];
	const static char* TD[];
	const static type_of_lex dlms[];
	static Tabl_ident TID;
	static Tabl_ident TCONSTSTR;



	Lex get_lex();
	Scanner(const char* program) {
		fp = fopen(program, "r");
		CS = H;
		clear();
		gc();
	}
};
const char* Scanner::TW[] = // таблица служебных слов
	{
		"" // 0
		"and", // 1
		"else", // 2
		"if", // 3
		"false", // 4
		"int", // 5
		"string", // 6
		"bool", // 7
		"not", // 8
		"or", // 9
		"program", // 10
		"read", // 11
		"true", // 12
		"while", // 13
		"break", // 14
		"for", // 15
		"goto", // 16
		"write", // 17
		"struct", // 18
	};
const char* Scanner::TD[] = // таблица ограничителей
	{
		"", // 0
		"@", // 1
		";", // 2
		":", // 3
		",", // 4
		".", // 5
		"=", // 6
		"(", // 7
		")", // 8
		"{", // 9
		"}", // 10
		"==", // 11
		"<", // 12
		">", // 13
		"+", // 14
		"-", // 15
		"*", // 16
		"/", // 17
		"<=", // 18
		"!=", // 19
		">=", // 20
	};
Tabl_ident Scanner::TID(100); // таблица идентификаторов
Tabl_ident Scanner::TCONSTSTR(100);

const type_of_lex Scanner::words[] =
	{
		LEX_NULL,
		LEX_AND,
		LEX_ELSE,
		LEX_IF,
		LEX_FALSE,
		LEX_INT,
		LEX_STRING,
		LEX_BOOL,
		LEX_NOT,
		LEX_OR,
		LEX_PROGRAM,
		LEX_READ,
		LEX_TRUE,
		LEX_WHILE,
		LEX_FOR,
		LEX_GOTO,
		LEX_WRITE,
		LEX_STRUCT,
		LEX_BREAK,
	};
const type_of_lex Scanner::dlms[] =
	{
		LEX_NULL,
		LEX_FIN,
		LEX_SEMICOLON,
		LEX_COLON,
		LEX_COMMA,
		LEX_DOT,
		LEX_ASSIGN,
		LEX_LPAREN,
		LEX_RPAREN,
		LEX_LBRACE,
		LEX_RBRACE,
		LEX_EQ,
		LEX_LSS,
		LEX_GTR,
		LEX_PLUS,
		LEX_MINUS,
		LEX_TIMES,
		LEX_SLASH,
		LEX_LEQ,
		LEX_NEQ,
		LEX_GEQ,
	};
Lex Scanner::get_lex() {
	int d, j;
	CS = H;
	do {
		switch (CS) {
		case H:
			if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
				gc();
			else if (isalpha(c)) {
				clear();
				add();
				gc();
				CS = IDENT;
			}
			else if (isdigit(c)) {
				d = c - '0';
				gc();
				CS = NUMB;
			}
			else if (c == '/') {
				gc();
				CS = COM;
			}
			else if (c == '=' || c == '<' || c == '>') {
				clear();
				add();
				gc();
				CS = ALE;
			}
			else if (c == EOF)
				return Lex(LEX_FIN, 1);
			else if (c == '!') {
				clear();
				add();
				gc();
				CS = NEQ;
			}
			else if (c == '"') {
				clear();
				gc();
				CS = CONSTSTR;
			}
			else
				CS = DELIM;
			break;
		case CONSTSTR:
			if (c == '"'){
				gc();
				return Lex(LEX_CONSTSTR, (long long)strdup(buf));
			} else {
				add();
				gc();
			}
			break;
		case IDENT:
			if (isalpha(c) || isdigit(c) || c == '.') {
				add();
				gc();
			}
			else if (j = look(buf, TW))
				return Lex(words[j], j);
			else {
				j = TID.put(buf);
				return Lex(LEX_ID, j);
			}
			break;
		case NUMB:
			if (isdigit(c)) {
				d = d * 10 + (c - '0');
				gc();
			}
			else if(isalpha(c))
			{
				throw(c);
			}
			else
				return Lex(LEX_NUM, d);
			break;
		case COM:
			if (c == '/'){
				gc();
				CS = LINE_COM;
			} else if (c == '*'){
				gc();
				CS = MULTLINE_COM;
			} else {
				j = look("/", TD);
				return Lex(dlms[j], j);
			}
			break;
		case MULTLINE_COM:
			if (c == '*'){	//TODO -- case /* dfv/* fv*/
				gc();
				if (c == '/'){
					gc();
					CS = H;
				}
			} else if (c == EOF){
				throw "wrong EOF";
			} else {
				gc();
			}
			break;
		case LINE_COM:
			if (c == '\n'){
				gc();
				CS = H;
			} else if (c == EOF) {
				CS = H;
			} else {
				gc();
			}
			break;
		case ALE:
			if (c == '=') {
				add();
				gc();
				j = look(buf, TD);
				return Lex(dlms[j], j);
			}
			else {
				j = look(buf, TD);
				return Lex(dlms[j], j);
			}
			break;
		case NEQ:
			if (c == '=') {
				add();
				gc();
				j = look(buf, TD);
				return Lex(LEX_NEQ, j);
			}
			else
				throw '!';
			break;
		case DELIM:
			clear();
			add();
			if (j = look(buf, TD)) {
				gc();
				return Lex(dlms[j], j);
			}
			else
				throw c;
			break;
		}
	} while (true);
}

int LexInd(const type_of_lex* arr, type_of_lex x){
	int i = 0;
	while (arr[i] != x){
		i++;
	}
	return i;
}

std::ostream& operator<<(std::ostream& s, Lex l) {
	std::string t;
	if (l.t_lex <= LEX_BREAK)
	t = Scanner::TW[LexInd(Scanner::words,l.t_lex)];
	else if (l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ)
	t = Scanner::TD[LexInd(Scanner::dlms,l.t_lex)];
	else if (l.t_lex == LEX_ID)
	t ="ID_" + std::string(Scanner::TID[l.v_lex].get_name());
	else if (l.t_lex == LEX_NUM) 
	t = "NUM_" + std::to_string(l.v_lex);
	else if (l.t_lex == LEX_CONSTSTR)
	t = "CONST_STR";
	else if (l.t_lex == LEX_UNARY_MINUS)
	t = "-u";
	else if (l.t_lex == POLIZ_GO)
	t = "!";
	else if (l.t_lex == POLIZ_FGO)
	t = "!F";
	else if (l.t_lex == POLIZ_ADDRESS)
	t = "&_" + std::string(Scanner::TID[l.v_lex].get_name());
	else if (l.t_lex == POLIZ_LABEL)
	t = "mark_" + std::to_string(l.v_lex);
	else
	throw l;

s << t <<" " << l.get_value() << endl;

return s;
}
int main(int argc, char** argv) {
	Scanner myscanner(argv[1]);
	Lex current_lex;
	int current_value;
	type_of_lex current_type;
	while(current_type != LEX_FIN){
		current_lex = myscanner.get_lex();
		current_type = current_lex.get_type();
		current_value = current_lex.get_value();
		cout << current_lex<< endl;
	}
	return 0;
}
