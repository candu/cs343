#include <uC++.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

enum GrammarState {
  KEEP_SENDING = 0,
  VALID = 1,
  INVALID = 2
};

_Coroutine grammar {
  char ch;
  int status;
  void expectAlpha() {
    status = isalpha(ch) ? KEEP_SENDING : INVALID;
  }
  void expectDigit() {
    status = isdigit(ch) ? KEEP_SENDING : INVALID;
  }
  void main() {
    expectAlpha(); suspend();
    expectDigit(); suspend();
    expectAlpha(); suspend();
    while (ch == ' ') {
      suspend();
    }
    expectDigit(); suspend();
    expectAlpha(); suspend();
    expectDigit(); status = VALID;
  }
 public:
  int next(char c) {
    ch = c;
    resume();
    return status;
  }
};

void uMain::main() {
  ifstream inFile(argv[1]);
  string line;
  while (getline(inFile, line)) {
    cout << "\"" << line << "\"";
    if (line.empty()) {
      cout << " Warning! Blank line." << endl;
      continue;
    }
    grammar g;
    int status;
    string::iterator it = line.begin();
    do {
      status = g.next(*it++);
      if (it == line.end() && status == KEEP_SENDING) {
        status = INVALID;
      }
    } while (status == KEEP_SENDING);
    string match(line.begin(), it);
    cout << " : \"" << match << "\"";
    switch (status) {
      case VALID:
        cout << " yes";
        break;
      case INVALID:
        cout << " no";
        break;
      default:
        cout << "unexpected status: " << status << endl;
        return;
    }
    if (it != line.end()) {
      string extraneous(it, line.end());
      cout << " -- extraneous characters \"" << extraneous << "\"";
    }
    cout << endl;
  }
}
