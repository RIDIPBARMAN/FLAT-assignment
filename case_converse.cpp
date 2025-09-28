#include <fst/fstlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace fst;

// Function to add arcs for lowercase → uppercase conversion
void addLowercaseArcs(StdVectorFst &fst, int start, int accept) {
    for (char c = 'a'; c <= 'z'; ++c) {
        char upper = static_cast<char>(std::toupper(c));
        fst.AddArc(start, StdArc(c, upper, TropicalWeight::One(), accept));
    }
}

// Function to add arcs that preserve character (identity mapping)
void addIdentityArcs(StdVectorFst &fst, int start, int accept, const std::string &chars) {
    for (char c : chars) {
        fst.AddArc(start, StdArc(c, c, TropicalWeight::One(), accept));
    }
}

// Function to perform conversion using the FST
std::string convertString(const StdVectorFst &fst, int start, const std::string &input) {
    std::string output;

    for (char c : input) {
        bool matched = false;

        // Iterate over all arcs to find match
        for (ArcIterator<StdVectorFst> aiter(fst, start); !aiter.Done(); aiter.Next()) {
            const StdArc &arc = aiter.Value();
            if (arc.ilabel == c) {
                output.push_back(static_cast<char>(arc.olabel));
                matched = true;
                break;
            }
        }

        if (!matched) {
            // Fallback: keep character unchanged
            output.push_back(c);
        }
    }
    return output;
}

int main() {
    StdVectorFst fst;

    int start = fst.AddState();
    fst.SetStart(start);

    int accept = fst.AddState();
    fst.SetFinal(accept, TropicalWeight::One());

    // Add mappings
    addLowercaseArcs(fst, start, accept);
    addIdentityArcs(fst, start, accept, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");

    // Input
    std::string input;
    std::cout << "Enter a string: ";
    std::getline(std::cin, input);

    // Conversion
    std::string output = convertString(fst, start, input);

    // Result
    std::cout << "Converted string: " << output << std::endl;
    return 0;
}
