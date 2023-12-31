//
// Created by 丁麓然 on 9/22/23.
//

#include "../../inc/frontend/frontend.h"
#include "../../inc/backend/backend.h"
#include "../../inc/basic.h"
#include "../../inc/datastructure/customlist.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

extern MODE mode;
extern Page page;

namespace interface {

void Welcome() {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  int terminalWidth = size.ws_col;
  int terminalHeight = size.ws_row;

  std::cout << "\033[2J\033[H"; // Clear terminal content

  for (int i = 0; i < (terminalHeight - 2) / 2; i++) {
    std::cout << std::endl;
  }

  std::cout << std::setw((terminalWidth - 44) / 2) << ""
            << "Welcome to use Florentino's word processor!" << std::endl;
  std::cout << std::setw((terminalWidth - 32) / 2) << ""
            << "Press \"h\" to get help." << std::endl;
  std::cout << std::setw((terminalWidth - 32) / 2) << ""
            << "Press \"c\" to create a new file." << std::endl;
  std::cout << std::setw((terminalWidth - 32) / 2) << ""
            << "Press \"o\" to open a exist file." << std::endl;
  std::cout << std::setw((terminalWidth - 32) / 2) << ""
            << "Press \"q\" to quit." << std::endl;

  for (int i = 0; i < (terminalHeight - 2) / 2 - 2; i++) {
    std::cout << std::endl;
  }
  SwitchInterface(GetUserInput());
}

void Help() {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  int terminalWidth = size.ws_col;
  int terminalHeight = size.ws_row;

  system("clear");
  for (int i = 0; i < (terminalHeight - 5) / 2; i++) {
    std::cout << std::endl;
  }

  std::cout << std::setw((terminalWidth - 4) / 2) << ""
            << "help: get help" << std::endl;
  std::cout << std::setw((terminalWidth - 10) / 2) << ""
            << "open: open a file" << std::endl;
  std::cout << std::setw((terminalWidth - 12) / 2) << ""
            << "create: create a file" << std::endl;
  std::cout << std::setw((terminalWidth - 10) / 2) << ""
            << "save: save the file" << std::endl;
  std::cout << std::setw((terminalWidth - 12) / 2) << ""
            << "exit: exit the program" << std::endl;

  for (int i = 0; i < (terminalHeight - 5) / 2; i++) {
    std::cout << std::endl;
  }
}

void ShowPage(const custom::list<int> &to_highlight) {
  using std::wcout;

  system("clear");
  if (page.empty()) {
    wcout << L"Page is empty." << std::endl;
    return;
  }
  if (not to_highlight.empty()) {
    page.highlight_show(to_highlight, mode);
  } else {
    page.show(mode);
  }
}

void SearchWord(const wchar_t *const input) {
  custom::list<int> to_highlight;
  if (mode == PLAIN_MODE) {
    to_highlight = page.find(input);
    if (to_highlight.empty()) {
      std::wcout << L"Can't find \"" << input << L"\"." << std::endl;
      return;
    } else {
      std::wcout << "The word \"" << input << "\" appears "
                 << to_highlight.size() << " times." << std::endl;
      ShowPage(to_highlight);
    }
  } else {
    custom::string word;
    if (std::all_of(input, input + wcslen(input),
                    [](wchar_t c) { return std::iswdigit(c); })) {
      int idx = std::stoi(input);
      word = page[idx];
    } else {
      word = input;
    }
    to_highlight = page.find(word);
    std::wcout << "The word \"" << word << "\" appears " << to_highlight.size()
               << " times." << std::endl;
    ShowPage(to_highlight);
  }
}

char GetUserInput() {
  char userInput;

  while (true) {
    std::cin >> userInput;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      break;
    }
  }
  return userInput;
}

void SwitchInterface(const char userInput) {
  using custom::OpenFile, custom::SaveFile;
  using std::wcout, std::endl, std::wcin;

  wchar_t temp[100];
  wchar_t temp2[100];
  Page rpage = page;
  switch (userInput) {
  case 'h':
  case 'H':
  case '?':
    Help();
    break;
  case 'o':
  case 'O':
    wcin >> temp;
    OpenFile(temp);
    ShowPage();
    break;
  case 'c':
  case 'C':
    wcin >> temp;
    wcout << L"The word \"" << temp << L"\" appears " << page.count(temp)
          << L" times." << endl;
    wcin.get();
    wcin.get();
    ShowPage();
    break;
  case 'w':
  case 'W':
    wcin >> temp;
    SaveFile(temp);
    ShowPage();
    break;
  case 's':
  case 'S':
    wcin >> temp;
    SearchWord(temp);
    break;
  case 'i':
  case 'I':
    wcin >> temp;
    wcin >> temp2;
    page.insert(std::stoi(temp), temp2);
    ShowPage({std::stoi(temp)});
    break;
  case 'r':
  case 'R':
    wcin >> temp;
    wcin >> temp2;
    page.replace(std::stoi(temp), temp2);
    ShowPage({std::stoi(temp)});
    break;
  case 'd':
  case 'D':
    if (mode == PLAIN_MODE) {
      wcin >> temp;
      page.remove(temp);
    } else {
      wcin >> temp;
      page.remove(std::stoi(temp));
    }
    ShowPage();
    break;
  case 'm':
  case 'M':
    if (mode == PLAIN_MODE) {
      mode = NUMBER_MODE;
    } else {
      mode = PLAIN_MODE;
    }
    ShowPage();
    break;
  case 'j':
  case 'J':
      rpage.reverse();
    if (rpage == page)
      wcout << L"Page is a palindrome." << endl;
    else
      wcout << L"Page is not a palindrome." << endl;
    wcin.get();
    ShowPage();
    break;
  case 'p':
  case 'P':
    wcin >> temp;
    if (temp[1] != L'\0') {
      throw std::invalid_argument("Invalid argument");
    }
    page = custom::string(page).split(temp[0]);
    page.set_segment(temp);
    ShowPage();
    break;
  case 't':
  case 'T':
    page.reverse();
    ShowPage();
    break;
  case 'e':
  case 'E':
    if (mode == PLAIN_MODE) {
      wcin >> temp;
      wcin >> temp2;
      custom::list<int> idx = page.find(temp);
      if (idx.empty()) {
        std::wcout << L"Can't find \"" << temp << L"\"." << std::endl;
        wcin.get();
        ShowPage();
        break;
      }
      int t = std::stoi(temp2);
      page.remove(idx.front(), t);
    } else {
      wcin >> temp;
      wcin >> temp2;
      int idx = std::stoi(temp);
      int t = std::stoi(temp2);
      page.remove(idx, t);
    }
    ShowPage();
    break;
  case 'q':
  case 'Q':
    exit(0);
  default:
    std::wcout << L"Unknown command: " << userInput << std::endl;
    break;
  }
  SwitchInterface(GetUserInput());
}
} // namespace interface
