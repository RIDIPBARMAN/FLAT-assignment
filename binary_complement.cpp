#include <fst/fstlib.h>
#include <iostream>
#include <string>

using namespace fst;

int main() {
    std::string input;
    std::cout << "Enter a binary string: ";
    std::getline(std::cin, input);

    // Build the FST
    StdVectorFst fst;
    int start = fst.AddState();
    fst.SetStart(start);
    int current = start;

    for (char c : input) {
        int next = fst.AddState();
        if (c == '0') {
            fst.AddArc(current, StdArc('0', '1', TropicalWeight::One(), next));
        } else if (c == '1') {
            fst.AddArc(current, StdArc('1', '0', TropicalWeight::One(), next));
        } else {
            std::cerr << "Invalid character: " << c << std::endl;
            return 1;
        }
        current = next;
    }
    fst.SetFinal(current, TropicalWeight::One());

    // Compose input with FST to collect the output
    StdVectorFst input_fst;
    int s = input_fst.AddState();
    input_fst.SetStart(s);
    for (char c : input) {
        int t = input_fst.AddState();
        input_fst.AddArc(s, StdArc(c, c, TropicalWeight::One(), t));
        s = t;
    }
    input_fst.SetFinal(s, TropicalWeight::One());

    // Apply composition
    StdVectorFst result;
    Compose(input_fst, fst, &result);

    // Extract output labels
    std::string output;
    for (StateIterator<StdVectorFst> siter(result); !siter.Done(); siter.Next()) {
        for (ArcIterator<StdVectorFst> aiter(result, siter.Value()); !aiter.Done(); aiter.Next()) {
            const auto &arc = aiter.Value();
            if (arc.olabel != 0) output.push_back(static_cast<char>(arc.olabel));
        }
    }

    std::cout << "Binary complement: " << output << std::endl;
    return 0;
}
