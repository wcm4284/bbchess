#include "tt.h"

#include <bit> 
#include <cstring>

namespace Engine {

static constexpr size_t searchClusterSize = 3;
static constexpr size_t perftClusterSize = 2;

union Cluster {
    
    PerftEntry prft[perftClusterSize];
    SearchEntry srch[searchClusterSize];
};

static_assert(sizeof(Cluster) == 32, "Cluster should be aligned to 64 bytes");

void PerftEntry::save(Key key, Depth depth, uint64_t cnt) {
    key48 = key >> 16;
    depth16 = uint16_t(depth);
    nodes = cnt;
}

void PerftWriter::write(Key key, Depth depth, uint64_t cnt) {
    entry->save(key, depth, cnt);
}

void SearchEntry::save(Key key, Depth depth, uint16_t raw, Value v, Value ev, Bound b, uint8_t generation8) {
    key16 = uint16_t(key >> 48);
    depth8 = uint8_t(depth);
    gen8 = generation8 | b;
    eval16 = int16_t(ev);
    value16 = int16_t(v);
    raw16 = raw; 
}

void SearchWriter::write(Key key, Depth depth, Move m, Value v, Value ev, Bound b, uint8_t gen) {
    entry->save(key, depth, m.raw(), v, ev, b, gen); 
}

TTData TTData::null() {
    return TTData(Move(0), Depth(0), VALUE_ZERO, VALUE_ZERO, BOUND_NONE, false);
}

TranspositionTable::~TranspositionTable() {
    delete[] table;
}

// Sets the size of the transposition table to the requested size (MBs)
// Makes sure that the number of clusters is a power of 2 so that we can
// index into the table efficiently with key & (numClusers - 1) instead of %
void TranspositionTable::resize(size_t MB) {

    delete[] table;

    numClusters = (MB << 20) / sizeof(Cluster); 
    numClusters = 1ULL << (std::bit_width(numClusters) - 1);

    std::cout << numClusters << std::endl;
    
    table = new Cluster[numClusters];
    clear(); 
}

// @prereq - table & numClusters need to be set, otherwise we're doing nothing 
// This could be multi-threaded, but it doesn't take long enough to clear for me
// to care
void TranspositionTable::clear() {
    assert(table);

    for (size_t i = 0; i < numClusters; ++i) {
        // because a TTEntry object isn't trivially constructable, and by extension
        // a Cluster object, the compiler throws as a -Wclass-memaccess warning
        // to make this go away, we tell it to not worry about us for a second
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
        memset(&table[i], 0, sizeof(Cluster)); 
#pragma GCC diagnostic pop
    }
}

void TranspositionTable::set_probe(ProbeType pb) {
    
    if (pb != probeMode) {
        clear();
    }

    probeMode = pb;
}

SearchEntry* TranspositionTable::find_s_entry(const Key key) const {
    // Ideally this is the same as key % numClusters.
    assert(popcnt(numClusters) == 1);
    size_t clusterIdx = key & (numClusters - 1);
    return &table[clusterIdx].srch[0];
}

PerftEntry* TranspositionTable::find_p_entry(const Key key) const {
    size_t clusterIdx = key & (numClusters - 1);
    return &table[clusterIdx].prft[0];
}

std::tuple<bool, TTData, PerftWriter> 
TranspositionTable::probe_perft(const Key key) const {

    PerftEntry *pe = find_p_entry(key);
    for (size_t i = 0; i < perftClusterSize; ++i) {
        if (pe[i].key48 == (key >> 16)) {
            return {pe[i].occupied(), pe[i].read(), PerftWriter(&pe[i])};
        }
    }

    PerftEntry *replace = pe;
    for (size_t i = 1; i < perftClusterSize; ++i) {
        if (!pe[i].occupied()) {
            replace = &pe[i];
            break;
        }

        if (replace->depth16 < pe[i].depth16) {
            replace = &pe[i];
        }
    }

    return {false, TTData::null(), PerftWriter(replace)};
}

std::tuple<bool, TTData, SearchWriter> 
TranspositionTable::probe_search(const Key key) const {

    SearchEntry *se = find_s_entry(key);
    for (size_t i = 0; i < searchClusterSize; ++i) {
        // we use the most significant bits because the table index uses
        // the least significant bits, so any entries in the same cluster
        // likely have the same least significant bits, which would lead to
        // a lot of false positives
        if (se[i].key16 == (key >> 48)) {
            return {se[i].occupied(), se[i].read(), SearchWriter(&se[i])};
        }
    }

    SearchEntry *replace = se;
    for (size_t i = 1; i < searchClusterSize; ++i) {
        if (!se[i].occupied()) {
            replace = &se[i];
            break;
        }
        // TODO regular replacement policy
        
    }

    return {false, TTData::null(), SearchWriter(replace)};
}

std::tuple<bool, TTData, std::variant<SearchWriter, PerftWriter>> 
TranspositionTable::probe(const Key key) const {
    if (probeMode == SEARCH)
        return probe_search(key);

    assert(probeMode == PERFT);
    return probe_perft(key);
}

} // namespace Engine
