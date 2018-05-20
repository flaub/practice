#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct AST {
  AST(string op_, int n_) : op(op_), n(n_) {}
  AST(string op_, AST* a_, AST* b_) : op(op_), a(a_), b(b_) {}

  string op;
  AST* a = nullptr;
  AST* b = nullptr;
  int n = -1;

  void Print(int depth) {
    for (int i = 0; i < depth; i++) {
      cout << "  ";
    }
    cout << op;
    if (op == "imm" || op == "arg") {
      cout << "(" << n << ")";
    }
    cout << endl;
    if (a) {
      a->Print(depth + 1);
    }
    if (b) {
      b->Print(depth + 1);
    }
  }

  void ReduceBinExpr(function<int(int a, int b)> compute) {
    a->Reduce();
    b->Reduce();
    if (a->op == "imm" && b->op == "imm") {
      op = "imm";
      n = compute(a->n, b->n);
      delete a;
      a = nullptr;
      delete b;
      b = nullptr;
    }
  }

  void Reduce() {
    if (op == "+") {
      ReduceBinExpr([](int a, int b) { return a + b; });
    } else if (op == "-") {
      ReduceBinExpr([](int a, int b) { return a - b; });
    } else if (op == "*") {
      ReduceBinExpr([](int a, int b) { return a * b; });
    } else if (op == "/") {
      ReduceBinExpr([](int a, int b) { return a / b; });
    }
  }

  void GenBinExpr(vector<string>& code, const char* instr) {
    a->Gen(code);
    b->Gen(code);
    code.push_back("PO");
    code.push_back("SW");
    code.push_back("PO");
    code.push_back(instr);
    code.push_back("PU");
  }

  void Gen(vector<string>& code) {
    if (op == "imm") {
      code.push_back("IM " + to_string(n));
      code.push_back("PU");
    } else if (op == "arg") {
      code.push_back("AR " + to_string(n));
      code.push_back("PU");
    } else if (op == "+") {
      GenBinExpr(code, "AD");
    } else if (op == "-") {
      GenBinExpr(code, "SU");
    } else if (op == "*") {
      GenBinExpr(code, "MU");
    } else if (op == "/") {
      GenBinExpr(code, "DI");
    }
  }
};

// function   ::= '[' arg-list ']' expression
// arg-list   ::= /* nothing */
//              | variable arg-list
// expression ::= term
//              | expression '+' term
//              | expression '-' term
// term       ::= factor
//              | term '*' factor
//              | term '/' factor
// factor     ::= number
//              | variable
//              | '(' expression ')'

// function   ::= '[' arg-list ']' expr
// arg-list   ::= /* nothing */
//              | variable arg-list
// expr       ::= term expr_tail
// expr_tail  ::= '+' term expr_tail
//              | '-' term expr_tail
//              | E
// term       ::= factor term_tail
// term_tail  ::= '*' factor term_tail
//              | '/' factor term_tail
//              | E
// factor     ::= '(' expr ')'
//              | number
//              | variable

class Parser {
public:
  Parser(vector<string> tokens) : tokens_(tokens) {}

  AST* Parse() {
    // cout << "tokens:" << endl;
    // for (string token : tokens_) {
    //   cout << "  " << token << endl;
    // }
    return ParseFunction();
  }

  AST* ParseFunction() {
    print("function", 0);
    consume("[");
    ParseArgList(0);
    consume("]");
    return ParseExpr(0);
  }

  void ParseArgList(size_t depth) {
    print("arg_list", depth);
    if (peek() == "]") {
      return;
    }
    args_.push_back(ParseVariable(depth + 1));
    ParseArgList(depth + 1);
  }

  AST* ParseExpr(size_t depth) { //
    print("expr", depth);
    return ParseExprTail(ParseTerm(depth + 1), depth + 1);
  }

  AST* ParseExprTail(AST* lval, size_t depth) {
    print("expr_tail", depth);
    if (check("+")) {
      consume("+");
      print("+", depth + 1);
      return ParseExprTail(new AST("+", lval, ParseTerm(depth + 2)), depth + 1);
    } else if (check("-")) {
      consume("-");
      print("-", depth + 1);
      return ParseExprTail(new AST("-", lval, ParseTerm(depth + 2)), depth + 1);
    }
    return lval;
  }

  AST* ParseTerm(size_t depth) { //
    print("term", depth);
    return ParseTermTail(ParseFactor(depth + 1), depth + 1);
  }

  AST* ParseTermTail(AST* lval, size_t depth) {
    print("term_tail", depth);
    if (check("*")) {
      consume("*");
      print("*", depth + 1);
      return ParseTermTail(new AST("*", lval, ParseFactor(depth + 2)),
                           depth + 1);
    }
    if (check("/")) {
      consume("/");
      print("/", depth + 1);
      return ParseTermTail(new AST("/", lval, ParseFactor(depth + 2)),
                           depth + 1);
    }
    return lval;
  }

  AST* ParseFactor(size_t depth) {
    print("factor", depth);
    if (isEOF()) {
      return nullptr;
    }
    if (check("(")) {
      consume("(");
      AST* value = ParseExpr(depth + 1);
      consume(")");
      return value;
    }
    try {
      int value = stoi(peek());
      consume();
      return new AST("imm", value);
    } catch (...) {
      string var = ParseVariable(depth + 1);
      for (size_t i = 0; i < args_.size(); i++) {
        if (args_[i] == var) {
          return new AST("arg", i);
        }
      }
      throw runtime_error("Invalid arg");
    }
  }

  string ParseVariable(size_t depth) {
    print("var: " + peek(), depth);
    return consume();
  }

private:
  string consume(string match = "") {
    string next = peek();
    idx_++;
    if (!match.empty() && next != match) {
      cerr << "Mismatch! next: " << next << ", match: " << match << endl;
    }
    return next;
  }

  bool check(string token) {
    if (isEOF()) {
      return false;
    }
    return peek() == token;
  }

  string peek() { //
    return tokens_[idx_];
  }

  bool isEOF() { //
    return idx_ == tokens_.size();
  }

  void print(std::string node, size_t depth) {
    // for (size_t i = 0; i < depth; i++) {
    //   cout << "  ";
    // }
    // cout << node << endl;
  }

private:
  vector<string> tokens_;
  vector<string> args_;
  size_t idx_ = 0;
};

struct Compiler {

  vector<string> compile(string program) {
    return pass3(pass2(pass1(program)));
  }

  // Turn a program string into a vector of tokens.  Each token
  // is either '[', ']', '(', ')', '+', '-', '*', '/', a variable
  // name or a number (as a string)
  vector<string> tokenize(string program) {
    static regex re("[-+*/()[\\]]|[A-Za-z]+|\\d+");
    sregex_token_iterator it(program.begin(), program.end(), re);
    return vector<string>(it, sregex_token_iterator());
  }

  // Returns an un-optimized AST
  AST* pass1(string program) {
    auto tokens = tokenize(program);
    Parser parser(tokens);
    return parser.Parse();
  }

  // Returns an AST with constant expressions reduced
  AST* pass2(AST* ast) { //
    ast->Print(0);
    ast->Reduce();
    ast->Print(0);
    return ast;
  }

  // Returns assembly instructions
  vector<string> pass3(AST* ast) {
    vector<string> assembly;
    ast->Gen(assembly);
    // assembly.push_back("PO");
    return assembly;
  }
};

int simulate(const vector<string>& assembly, const vector<int>& argv) {
  int r0 = 0, r1 = 0;
  stack<int> istack;
  for (auto& ins : assembly) {
    string code = ins.substr(0, 2);
    if (code == "IM") {
      r0 = stoi(ins.substr(3));
    } else if (code == "AR") {
      r0 = argv.at(stoi(ins.substr(3)));
    } else if (code == "SW") {
      swap(r0, r1);
    } else if (code == "PU") {
      istack.push(r0);
    } else if (code == "PO") {
      r0 = istack.top();
      istack.pop();
    } else if (code == "AD") {
      r0 += r1;
    } else if (code == "SU") {
      r0 -= r1;
    } else if (code == "MU") {
      r0 *= r1;
    } else if (code == "DI") {
      r0 /= r1;
    }
  }
  return r0;
}

void Test(const char* src, vector<int> args, int expected) {
  cout << "src: " << src << endl;
  cout << "args: ";
  for (int arg : args) {
    cout << arg << ", ";
  }
  cout << endl;
  Compiler compiler;
  vector<string> assembly = compiler.compile(src);
  for (string inst : assembly) {
    cout << inst << endl;
  }
  cout << endl;
  int result = simulate(assembly, args);
  if (result == expected) {
    cout << "PASS" << endl;
  } else {
    cout << "FAIL" << endl;
    cout << "result: " << result << endl;
    cout << "expected: " << expected << endl;
  }
  cout << endl;
}

int main() {
  Test("[ a b ] a*a + b*b", {3, 4}, 3 * 3 + 4 * 4);
  Test("[ first second ] (first + second) / 2", {3, 4}, (3 + 4) / 2);
  Test("[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)", {1, 2, 3},
       (2 * 3 * (1) + 5 * (2) - 3 * (3)) / (1 + 3 + 2 * 2));
  Test("[x] x+2*5", {10}, (10) + 2 * 5);
  return 0;
}
