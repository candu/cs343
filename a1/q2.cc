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
    suspend();
  }
  void expectDigit() {
    status = isdigit(ch) ? KEEP_SENDING : INVALID;
    suspend();
  }
  void expectDigitAndEnd() {
    status = isdigit(ch) ? VALID : INVALID;
    suspend();
  }
  void skip() {
    suspend();
  }
  void main() {
    // TODO: implement this
    expectAlpha();
    expectDigit();
    expectAlpha();
    while (ch == ' ') {
      skip();
    }
    expectDigit();
    expectAlpha();
    expectDigitAndEnd();
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
    string::iterator it;
    int status;
    for (it = line.begin(); it != line.end(); ) {
      status = g.next(*it++);
      if (status != KEEP_SENDING) {
        break;
      }
    }
    string match(line.begin(), it);
    cout << " : \"" << match << "\"";
    switch (status) {
      case VALID:
        cout << " yes";
        break;
      case KEEP_SENDING:
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
