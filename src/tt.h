#ifndef TT_H_INCLUDED
#define TT_H_INCLUDED

#include "types.h"
#include "position.h"

#include <tuple> 

namespace Engine {

struct Cluster;

struct TTData {
	TTData() = delete;

	TTData(Move m, Depth d, Value v, Value ev, Bound b, bool pv) :
		move(m),
		depth(d),
		value(v),
		eval(ev),
		bound(b),
		is_pv(pv) {}

	Move move;
	Depth depth;
	Value value, eval;
	Bound bound;
	bool is_pv;
};

class TTEntry {

	public:
		TTData read() {
			return TTData(
                         move, 
                         Depth(depth8), 
                         Value(value16), 
                         Value(eval16), 
                         Bound(gen8 & 0b11),
                         bool(gen8 & 0b100) );
		}
        void save(Key key, Depth depth, Value ev, Move m, Value v, Bound b, uint8_t gen); 
        bool occupied() const { return bool(depth8); }

	uint16_t key16;
	
	private:

	uint8_t  depth8;
	// gen8 is unique in that it holds 3 pieces of info:
	// 1. generation (5 bits)
	// 2. pv: (1 bit)
	// 3. bound: EXACT, LOWER, UPPER (2 bits)
	uint8_t  gen8; 
	int16_t  eval16;
	Move	 move;
	int16_t  value16;
};

// exists as a wrapper around a pointer to a TTEntry
struct TTWriter {
	TTWriter(TTEntry* e) : entry(e) {}
	
	// i'm unsure about gen TODO
	void write(Key key, Depth depth, Value ev, Move m, Value v, Bound b);  
	
	TTEntry* entry;
};

class TranspositionTable {
   public:

	TranspositionTable() = default;
	~TranspositionTable();

	void resize(size_t MB);
	void clear();
    uint8_t generation() const;
	TTEntry* find_entry(const Key) const; 
	std::tuple<bool, TTData, TTWriter> probe(const Key key) const;

   private:

    friend struct TTEntry;

	size_t numClusters;
	Cluster *table = nullptr;
    uint8_t generation8; 

};

inline uint8_t TranspositionTable::generation() const { return generation8; }

} // namespace Engine

#endif
