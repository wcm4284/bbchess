#ifndef TT_H_INCLUDED
#define TT_H_INCLUDED

#include "types.h"
#include "position.h"

#include <tuple> 
#include <variant>

namespace Engine {

union Cluster;

enum ProbeType {
    PERFT,
    SEARCH
};

struct TTData {
    TTData() = delete;

    TTData(Move m, Depth d, Value v, Value ev, Bound b, bool pv) :
        move(m),
        depth(d),
        value(v),
        eval(ev),
        bound(b),
        is_pv(pv) {}

    static TTData null();

    Move move;
    Depth depth;
    Value value, eval;
    Bound bound;
    bool is_pv;
};

struct PerftEntry {
    
    TTData read() {
        return TTData(
                Move(0),
                Depth(depth16),
                Value(nodes >> 32), // stored in v
                Value(nodes & 0xFFFFFFFF), // stored in ev
                Bound(0),
                false);
    }
    
    void save(Key key, Depth d, uint64_t);
    bool occupied() const { return bool(depth16); }

    uint64_t key48 : 48;
    uint16_t depth16;
    uint64_t nodes;
};

struct SearchEntry {

    TTData read() {
        return TTData(
                     Move(raw16), 
                     Depth(depth8), 
                     Value(value16), 
                     Value(eval16), 
                    Bound(gen8 & 0b11),
                     bool(gen8 & 0b100) );
    }
    void save(Key key, Depth depth, uint16_t raw, Value v, Value ev, Bound b, uint8_t gen); 
    bool occupied() const { return bool(depth8); }

    uint16_t key16;
    uint8_t  depth8;
    // gen8 is unique in that it holds 3 pieces of info:
    // 1. generation (5 bits)
    // 2. pv: (1 bit)
    // 3. bound: EXACT, LOWER, UPPER (2 bits)
    uint8_t  gen8; 
    int16_t  eval16;
    uint16_t raw16; // store raw move data bc Move isn't trivially constructible
    int16_t  value16;
};

// wrapper around PerftEntry
struct PerftWriter {
    PerftWriter() = delete;
    PerftWriter(PerftEntry* e) : entry(e) {}

    void write(Key key, Depth depth, uint64_t cnt);

    PerftEntry* entry;
};

// exists as a wrapper around a pointer to a SearchEntry
struct SearchWriter {
    SearchWriter() = delete;
    SearchWriter(SearchEntry* e) : entry(e) {}
    
    // i'm unsure about gen TODO
    void write(Key key, Depth depth, Move m, Value v, Value ev, Bound b, uint8_t gen);  
    
    SearchEntry* entry;
};

class TranspositionTable {
   public:

    TranspositionTable() = default;
    ~TranspositionTable();

    void resize(size_t MB);
    void clear();
    void set_probe(ProbeType);
    uint8_t generation() const;
    SearchEntry* find_s_entry(const Key) const; 
    PerftEntry* find_p_entry(const Key) const;

    std::tuple<bool, TTData, std::variant<SearchWriter, PerftWriter>> probe(const Key key) const;

   private:

    std::tuple<bool, TTData, PerftWriter> probe_perft(const Key) const;
    std::tuple<bool, TTData, SearchWriter> probe_search(const Key) const;

    size_t numClusters;
    Cluster *table = nullptr;
    uint8_t generation8; 
    ProbeType probeMode;

};

inline uint8_t TranspositionTable::generation() const { return generation8; }

} // namespace Engine

#endif
