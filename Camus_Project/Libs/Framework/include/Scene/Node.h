#ifndef CA_NODE_H
#define CA_NODE_H

#include <Utils/Maths.h>

#include <vector>
#include <string>

namespace hyperspace {
	namespace scene {
		enum NodeType_{
			// MESHES
			STATIC_MESH_NODE=0,
			FLOOR_MESH_NODE,
			SKYBOX_MESH_NODE,
			DYNAMIC_MESH_NODE,
			SKINNING_MESH_NODE,
			MORPH_MESH_NODE,
			ALPHA_TEST_MESH_NODE,
			ALPHA_BLENDING_MESH_NODE,
			// Utils
			BONE_NODE = 50,	
			CAMERA,

			// PHYSICS
			STATIC_PHYSICS_NODE = 100,
			CONVEX_PHYSICS_NODE,
			CAPSULE_PHYSICS_NODE,
			BOUNDING_BOX_PHYSICS_NODE,
			CHARACTER_CONTROLLER_PHYSICS_NODE,
			JOINT_PHYSICS_NODE,

			DEFAULT_NODE	=	512	
		};

		class CNode_ {
		public:
			CNode_(NodeType_);
			~CNode_();
			std::uint32_t				uniqueID;
			std::uint32_t				parentID;
			std::vector<std::uint32_t>	siblingsID;
			std::vector<std::uint32_t>	sons;

			NodeType_		type;
			XMATRIX44		transform;
			std::string		name;

			static std::vector<std::uint32_t>	allnodes;
		};
	}
}

#endif