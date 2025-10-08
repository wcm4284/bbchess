/**
 * @file tt.h
 * @brief Contains Transposition Table logic and helpers
 *
 */

#ifndef TT_H_INCLUDED
#define TT_H_INCLUDED

#include "position.h"

#include <tuple> 
#include <variant>

namespace Engine {

union Cluster;

/**
 * @enum ProbeType
 * @brief Type of TT Entry to retrieve in a query
*/
enum ProbeType {
    PERFT, ///< Perft entry
    SEARCH ///< Regular search entry
};

/**
 * @struct TTData
 * @brief Unpacked TT data, delivered in a query
*/
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
    
    Move move; ///< Best move in the position
    Depth depth; ///< Depth this search was performed at
    Value value; ///< Value returned by search
    Value eval; ///< Stand-pat eval of position
    Bound bound; ///< Cutoff during search
    bool is_pv; ///< Is this the best line?
};

/**
 * @struct PerftEntry
 * @brief Packed data stored in the TT for perft search
*/
struct PerftEntry {
    
    /**
     * @brief Unpacks the data stored in this object
     * @return TTData instance of the data stored here
    */
    TTData read() {
        return TTData(
                Move(0),
                Depth(depth16),
                Value(nodes >> 32), // stored in v
                Value(nodes & 0xFFFFFFFF), // stored in ev
                Bound(0),
                false);
    }
    
    /**
     * @brief Saves the data passed into the table
     * @param key Zobrist key of position
     * @param d Depth search was performed at
     * @param n Number of nodes found
    */
    void save(Key key, Depth d, uint64_t n);
    bool occupied() const { return bool(depth16); }

    uint64_t key48 : 48; ///< Zobrist key
    uint16_t depth16; ///< Depth search was performed at
    uint64_t nodes; ///< Number of nodes found
};

/**
 * @struct SearchEntry
 * @brief Packed data stored in the TT for bestmove search
*/
struct SearchEntry {
    
    /**
     * @brief Unpacks the data stored in this object
     * @return TTData instance of the stored data
    */
    TTData read() {
        return TTData(
                     Move(raw16), 
                     Depth(depth8), 
                     Value(value16), 
                     Value(eval16), 
                    Bound(gen8 & 0b11),
                     bool(gen8 & 0b100) );
    }
    
    /**
     * @brief Saves the data passed into the table
     * @param key Zobrist key of the position
     * @param depth Depth the search was performed at
     * @param raw Raw data of best move
     * @param v Value returned from search
     * @param ev Static evaluation of the position
     * @param b Cutoff perfomred in search
     * @param gen Table generation
    */
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

/**
 * @struct PerftWriter
 * @brief Wrapper around PerftEntry for writing information
*/
struct PerftWriter {
    PerftWriter() = delete;
    PerftWriter(PerftEntry* e) : entry(e) {}
    
    /**
     * @brief Write to the TT Entry
     * @note View PerftEntry for param descriptions
    */
    void write(Key key, Depth depth, uint64_t cnt);

    PerftEntry* entry; ///< Pointer to entry
};

/**
 * @struct SearchWriter
 * @brief Wrapper around SearchEntry for writing information
*/
struct SearchWriter {
    SearchWriter() = delete;
    SearchWriter(SearchEntry* e) : entry(e) {}
    
    /**
     * @brief Write to the TT Entry
     * @note View SearchEntry for param descriptions
    */
    void write(Key key, Depth depth, Move m, Value v, Value ev, Bound b, uint8_t gen);  
    
    SearchEntry* entry; ///< Pointer to entry
};

/**
 * @class TranspositionTable
 * @brief Used to store information about positions to reduce search time
*/
class TranspositionTable {
   public:

    TranspositionTable() = default;
    ~TranspositionTable();
    
    /// @brief Set the size of the table
    /// @param MB size, in megabytes, to allocate for the table
    void resize(size_t MB);
    /// @brief Zero out all entries in the table
    void clear();
    /// @brief Set probe type that will be used
    void set_probe(ProbeType);
    /// @brief Return the generation
    uint8_t generation() const;
    /// @brief Query for a bestmove search
    SearchEntry* find_s_entry(const Key) const; 
    /// @brief Query for a perft search
    PerftEntry* find_p_entry(const Key) const;
    
    /// @brief Probe function to be used by the search
    /// @return A tuple containing a boolean for if the entry was hit, the data, and a writer to the entry
    std::tuple<bool, TTData, std::variant<SearchWriter, PerftWriter>> probe(const Key key) const;

   private:
    
    /// @brief Probe for a perft entry
    std::tuple<bool, TTData, PerftWriter> probe_perft(const Key) const;
    /// @brief Probe for a bestmove entry
    std::tuple<bool, TTData, SearchWriter> probe_search(const Key) const;

    size_t numClusters; ///< Number of clusters in the table
    Cluster *table = nullptr; ///< Pointer for the table
    uint8_t generation8; ///< Current generation
    ProbeType probeMode;

};

inline uint8_t TranspositionTable::generation() const { return generation8; }

} // namespace Engine

#endif
