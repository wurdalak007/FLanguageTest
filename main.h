class RegularAutomat {
public:
    struct Vertex {
        std::map<char, int> next;
        Vertex() = default;
    };

    RegularAutomat (char _ch, const std::string& _pat);
    void Closer();
    void findPrefix(int verNum, int startPos);
    uint32_t getMax();

    RegularAutomat operator+(const RegularAutomat& other) {
        for (auto el : other.bohr) {
            for (auto p : el.next)
                el.next[p.first] += size;
            this->bohr.push_back(el);
        }
        bohr[0].next['1'] = size;

        for (auto el : other.termVertex)
            this->termVertex.insert(el+size);

        size += other.size;
        return *this;
    }

    RegularAutomat operator*(const RegularAutomat& other) {
        for (auto el : other.bohr) {
            for (auto p : el.next)
                el.next[p.first] += size;
            this->bohr.push_back(el);
        }

        for (int el : this->termVertex) {
            bohr[el].next['1'] = size;
        }

        this->termVertex.clear();
        for (auto el : other.termVertex)
            this->termVertex.insert(el + this->size);

        this->size += other.size;

        return *this;
    }

private:
    uint16_t size;
    std::set<int> termVertex;
    std::vector<Vertex> bohr;
    std::string pat;
    uint32_t maxP;

};

void add(std::stack<RegularAutomat>& st);

void mul(std::stack<RegularAutomat>& st);

bool testing ();