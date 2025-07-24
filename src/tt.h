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
		
		TTData read() { // TODO: fix bound and pv
			return TTData(move, Depth(depth8), Value(value16), Value(eval16), Bound(0), true);
		}

		void save(Key key, 

	uint16_t key16;
	
	private:

	uint8_t  depth8;
	uint8_t  gen8;
	int16_t  eval16;
	Move	 move;
	int16_t  value16;


};

// exists as a wrapper around a pointer to a TTEntry
struct TTWriter {

	TTWriter(TTEntry* e) : entry(e) {}
	
	// i'm unsure about gen TODO
	void write(Key key, Depth depth, uint8_t gen, Value ev, Move m, Value v);  

	
	TTEntry* entry;
};

class TranspositionTable {

	
	public:

	TranspositionTable() = default;
	~TranspositionTable();

	
	void resize(size_t MB);
	void clear();
	std::tuple<bool, TTData, TTWriter> probe(const Key key) const;

	private:

	TTEntry* find_entry(const Key) const; 

	size_t numClusters;
		
	Cluster *table = nullptr;

};

} // namespace Engine

#endif
