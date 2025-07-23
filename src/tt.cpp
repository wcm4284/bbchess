#include "tt.h"

namespace Engine {

void TranspositionTable::resize(size_t MB) {

	if (table) 
		delete[] table;

	
	table = new Cluster[(MB << 20) / sizeof(Cluster)];


}

} // namespace Engine
