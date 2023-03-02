#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cstdlib>
#include <ctime>

#define GRN "\x1B[42m"
#define YEL "\x1B[43m"
#define RES "\x1B[0m"

#define GUESSES 6
#define LETTERS 5


template<class T>
using vec = std::vector<T>;
using std::string;

int random(int max) {
    srand((unsigned) time(0));
    return rand() % max;
}

bool put_words_in(vec<string> &list, int &counter) {
    std::ifstream words;
    words.open("/usr/share/dict/words.sorted");
    string buf;
    counter = 0;

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
            counter++;
        }
    }
    return true;
}

bool has_word (vec<string> &words, string &word) {
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
    std::ostringstream buf;
    char c;
    size_t pos;

    buf << "\t";
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
        buf << space << word[i] << RES;
    }

    std::cout << buf.str() << std::endl;
    return;
}

void display(vec<string> &guesses, string &secret) {
    std::cout << "\x1B[2J\x1B[1;1H\twordle$ (lowercase)\n"
        << YEL << " " << RES << " - right letter, wrong place\n"
        << GRN << " " << RES << " - right letter, right place\n"
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

void parse_guess(string &guess, vec<string> &guesses,
        vec<string> &words, string &secret,
        bool &rotate, bool &won) {
    if (!has_word(words, guess)) {
        std::cout << "Not a valid word!" << std::endl;
        if (system("sleep 1"))
            ;
        rotate = false;
        return;
    }
    guesses.push_back(guess);
    if (guess.compare(secret) == 0)
        won = true;
    return;
}

int main(void) {
    vec<string> words;
    int ct;
    if (!put_words_in(words, ct))
        return 1;
    string secret = words[random(ct)];

    vec<string> guesses;

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
        std::cout << "You won!";
    else
        std::cout << "You lost! The word was: "
            << secret << std::endl
            ;

    return 0;
}
