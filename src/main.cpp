#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cstdlib>
#include <ctime>

#define GRN "\x1B[42m"
#define YEL "\x1B[43m"
#define RESET "\x1B[0m"

#define GUESSES 6
#define LETTERS 5

using std::string;
using std::vector;

int random(int max) {
    return rand() % max;
}

bool put_words_in(vector<string> &list) {
    std::ifstream words;
    words.open("/usr/share/dict/words.sorted");
    string buf;

    if (!words.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        return false;
    }
    while (!words.eof()) {
        buf = "";
        words >> buf;
        if (words.eof()) break;
        if (buf.length() == LETTERS) {
            list.push_back(buf);
        }
    }
    return true;
}

bool has_word (vector<string> &words, string &word) {
    int beg = 0;
    int end = words.size() - 1;
    int mid, diff;

    if (word.length() != LETTERS) {
        std::cout << "Incorrect word length:" << std::endl;
        return false;
    }

    while (beg <= end) {
        mid = (beg + end) / 2;
        diff = words[mid].compare(word);
        if (diff == 0)
            return true;
        else if (diff > 0)
            end = mid - 1;
        else
            beg = mid + 1;
    }

    return false;
}

void put_word(string &word, string &secret) {
    string copy { secret };
    string space;
    char c;
    size_t pos;

    std::cout << '\t';
    for (int i = 0; i < word.length(); i++) {
        space = " ";
        c = word[i];
        pos = copy.find(c);
        if (pos != string::npos) {
            space += (pos == i)
                ? GRN
                : YEL
                ;
            copy[static_cast<int>(pos)] = '.';
        }
        std::cout << space << c << RESET;
    }

    std::cout << std::endl;
    return;
}

void display(vector<string> &guesses, string &secret) {
    std::cout << "\x1B[2J\x1B[1;1H\twordle$ (lowercase)\n"
        << YEL << " " << RESET << " - right letter, wrong place\n"
        << GRN << " " << RESET << " - right letter, right place\n"
        << std::endl
        ;
    for (int i = 0; i < guesses.size(); i++)
        put_word(guesses[i], secret);
    if (guesses.size() < GUESSES) {
        std::cout << "\t";
        for (int i = 0; i < LETTERS; i++)
            std::cout << " -";
    }
    for (int i = 0; i < GUESSES-guesses.size(); i++)
        std::cout << std::endl;

    std::cout << "\t___________" << std::endl;
    return;
}

void parse_guess(string &guess, vector<string> &guesses,
        vector<string> &words, string &secret,
        bool &rotate, bool &won) {
    if (!has_word(words, guess)) {
        std::cout << "Not a valid word!" << std::endl;
        rotate = false;
        return;
    }
    guesses.push_back(guess);
    if (guess.compare(secret) == 0)
        won = true;
    return;
}

int main(void) {
    vector<string> words;
    if (!put_words_in(words))
        return 1;
    srand((unsigned) time(0));
    string secret = words[random(words.size())];

    vector<string> guesses;

    bool won = false;
    bool rotate = true;
    string guess;

    while (!won && guesses.size() < GUESSES) {
        if (rotate)
            display(guesses, secret);
        else
            rotate = true;
        std::cout << "\t";
        std::cin >> guess;
        parse_guess(guess, guesses,
                words, secret,
                rotate, won);
    }

    display(guesses, secret);

    std::cout << std::endl;
    if (won)
        std::cout << "You won!" << std::endl;
    else
        std::cout << "You lost! The word was: "
            << secret << std::endl
            ;

    return 0;
}
