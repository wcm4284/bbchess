#include "tt.h"

#include <cstring>

namespace Engine {

static constexpr size_t clusterSize = 4;
struct Cluster {

	TTEntry entries[clusterSize];

};

TranspositionTable::~TranspositionTable() {
	delete[] table;
}

void TranspositionTable::resize(size_t MB) {

	delete[] table;

	numClusters = (MB << 20) / sizeof(Cluster); 
	std::cout << "allocating table with " << numClusters << " clusters" << std::endl;
	
	table = new Cluster[numClusters];
	clear(); 
}

// @prereq - table & numClusters need to be set, otherwise we're doing nothing 
// This could be multi-threaded, but it doesn't take long enough to clear for me
// to care
void TranspositionTable::clear() {

	assert(table);

	if (!table)
		return;

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

TTEntry* TranspositionTable::find_entry(const Key key) const {
	
	// Ideally this is the same as key % numClusters.
	size_t clusterIdx = key & (numClusters - 1);
	return &table[clusterIdx].entries[0];

}

std::tuple<bool, TTData, TTWriter> 
TranspositionTable::probe(const Key key) const {

	TTEntry* tte = find_entry(key);

	for (size_t i = 0; i < clusterSize; ++i) {

		// we use the most significant bits because the table index uses
		// the least significant bits, so any entries in the same cluster
		// likely have the same least significant bits, which would lead to
		// a lot of collisions
		if (tte[i].key16 == (key >> 48)) {
			
			return {true, tte[i].read(), TTWriter(&tte[i])};

		}

	}

	TTEntry* replace = tte;
	for (size_t i = 1; i < clusterSize; ++i) {



	}


	return {false, TTEntry().read(), TTWriter(replace)};
}

} // namespace Engine
