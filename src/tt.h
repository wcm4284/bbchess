#ifndef TT_H_INCLUDED
#define TT_H_INCLUDED

#include "types.h"
#include "position.h"

namespace Engine {

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
		
		TTData read();
	
	private:

	uint16_t key16;
	uint8_t  depth8;
	uint8_t  gen8;
	int16_t  eval16;
	Move	 move;
	int16_t  value16;


};

struct Cluster {

	TTEntry entries[4];

};

class TranspositionTable {

	
	public:

	TranspositionTable() = default;
	~TranspositionTable() { if (table) { delete[] table; } }

	void resize(size_t MB);
	void clear();


	private:
		
	Cluster *table = nullptr;

};

} // namespace Engine

#endif
