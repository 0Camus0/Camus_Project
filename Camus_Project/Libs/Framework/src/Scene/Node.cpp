#include <Scene/Node.h>

namespace hyperspace {
	namespace scene {

		static std::uint32_t g_Id = 0;

		std::vector<std::uint32_t> CNode_::allnodes;

		CNode_::CNode_(NodeType_ t) {
			type = t;
			uniqueID = g_Id;
			g_Id++;
		
		}

		CNode_::~CNode_(){
		
		}
	}
}