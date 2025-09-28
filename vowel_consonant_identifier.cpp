#include <fst/fstlib.h>
#include <iostream>
#include <string>
#include <cctype>

using namespace fst;

int main() {
    // Create an FST with start and accept states
    StdVectorFst fst;
    int start = fst.AddState();
    int accept = fst.AddState();
    fst.SetStart(start);
    fst.SetFinal(accept, TropicalWeight::One());

    // Define vowels and consonants
    std::string vowels = "aeiou";
    std::string consonants = "bcdfghjklmnpqrstvwxyz";

    // Helper lambda to add arcs for letters (both lower & upper case)
    auto addMappings = [&](const std::string &letters, char out) {
        for (char c : letters) {
            fst.AddArc(start, StdArc(static_cast<int>(c), static_cast<int>(out), TropicalWeight::One(), accept));
            fst.AddArc(start, StdArc(static_cast<int>(std::toupper(c)), static_cast<int>(out), TropicalWeight::One(), accept));
        }
    };

    // Map vowels -> 'V', consonants -> 'C'
    addMappings(vowels, 'V');
    addMappings(consonants, 'C');

    // Map all other printable ASCII characters to themselves
    for (char c = 32; c < 127; ++c) {
        char lower = std::tolower(c);
        if (vowels.find(lower) == std::string::npos &&
            consonants.find(lower) == std::string::npos) {
            fst.AddArc(start, StdArc(static_cast<int>(c), static_cast<int>(c), TropicalWeight::One(), accept));
        }
    }

    // Take input string
    std::string input;
    std::cout << "Enter a string: ";
    std::getline(std::cin, input);

    // Process input using arcs
    std::string output;
    for (char c : input) {
        bool matched = false;
        for (ArcIterator<StdVectorFst> it(fst, start); !it.Done(); it.Next()) {
            const StdArc &arc = it.Value();
            if (arc.ilabel == static_cast<int>(c)) {
                output.push_back(static_cast<char>(arc.olabel));
                matched = true;
                break;
            }
        }
        if (!matched) {
            output.push_back('?'); // fallback if no arc found
        }
    }

    std::cout << "Output: " << output << std::endl;
    return 0;
}
