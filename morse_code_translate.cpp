#include <fst/fstlib.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>

using namespace fst;

// Function to create Morse dictionary
std::unordered_map<char, std::string> buildMorseMap() {
    return {
        {'A',".-"},   {'B',"-..."}, {'C',"-.-."}, {'D',"-.."},  {'E',"."},   {'F',"..-."},
        {'G',"--."},  {'H',"...."}, {'I',".."},   {'J',".---"}, {'K',"-.-"}, {'L',".-.."},
        {'M',"--"},   {'N',"-."},   {'O',"---"},  {'P',".--."}, {'Q',"--.-"},{'R',".-."},
        {'S',"..."} , {'T',"-"},    {'U',"..-"},  {'V',"...-"}, {'W',".--"}, {'X',"-..-"},
        {'Y',"-.--"}, {'Z',"--.."},
        {'0',"-----"},{'1',".----"},{'2',"..---"},{'3',"...--"},{'4',"....-"},
        {'5',"....."},{'6',"-...."},{'7',"--..."},{'8',"---.."},{'9',"----."},
        {' ', "/"}
    };
}

// Function to encode a single character into its FST path
void encodeCharToFst(StdVectorFst &fst, int &current, const std::string &code) {
    int prev = current;
    for (char mc : code) {
        int next = fst.AddState();
        fst.AddArc(prev, StdArc(mc, mc, TropicalWeight::One(), next));
        prev = next;
    }
    current = prev;

    // Add separator (space between letters)
    int sep = fst.AddState();
    fst.AddArc(current, StdArc(' ', ' ', TropicalWeight::One(), sep));
    current = sep;
}

// Extract final Morse code string from FST
std::string extractMorseOutput(const StdVectorFst &fst) {
    std::string result;
    for (StateIterator<StdVectorFst> siter(fst); !siter.Done(); siter.Next()) {
        int state = siter.Value();
        for (ArcIterator<StdVectorFst> aiter(fst, state); !aiter.Done(); aiter.Next()) {
            const StdArc &arc = aiter.Value();
            result.push_back(static_cast<char>(arc.olabel));
        }
    }
    return result;
}

int main() {
    auto morse = buildMorseMap();

    std::string text;
    std::cout << "Enter text: ";
    std::getline(std::cin, text);
    std::transform(text.begin(), text.end(), text.begin(), ::toupper);

    StdVectorFst fst;
    int start = fst.AddState();
    fst.SetStart(start);

    int current = start;
    for (char c : text) {
        if (morse.find(c) != morse.end()) {
            encodeCharToFst(fst, current, morse[c]);
        }
    }
    fst.SetFinal(current, TropicalWeight::One());

    std::string morse_output = extractMorseOutput(fst);
    std::cout << "Morse code: " << morse_output << std::endl;

    return 0;
}
