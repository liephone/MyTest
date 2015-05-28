
template <int N> struct Kitten {
    static_assert(N < 2, "Kitten<N> requires N < 2.");
};

 
int main() {
    Kitten<1> peppermint;
 
    Kitten<3> jazz;
}

