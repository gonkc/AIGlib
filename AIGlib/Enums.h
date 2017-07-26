#ifndef _ENUMS_H__
#define _ENUMS_H__

namespace AIGlib 
{
	enum class NodeType { GROUP, MESH, CAMERA, POINTLIGHT, DIRECTIONALLIGHT, SPOTLIGHT };
	enum class AttributeType { POS, COLOR, NORMAL, TEXCOORD, TANGENT, BINORMAL };
  enum class AttributeDataType { FLOAT, INT, UNSIGNED_INT, BYTE, UNSIGNED_BYTE, SHORT, UNSIGNED_SHORT, HALF_FLOAT, DOUBLE, FIXED, INT_2_10_10_10_REV, UNSIGNED_INT_2_10_10_10_REV, UNSIGNED_INT_10F_11F_11F_REV };
	enum class MaterialSourceType { TEXTURE, UNIFORM, ATTRIBUTE };	
	enum class TextureMaterialType { AMBIENT, DIFFUSE, SPECULAR, EMISSIVE, NORMAL, LIGHTMAP, HEIGHT, SHININESS, OPACITY, DISPLACEMENT, AUXILIAR };
	enum class UniformMaterialType { AMBIENT, DIFFUSE, SPECULAR, EMISSIVE, OPACITY };
	enum class CameraType { BASE, ARCBALL, TRACKBALL, FLIGHT, DRIVE, TERRAIN, ORBITAL, FIRSTPERSON, SPHERICAL };
	enum class ProjMatrixType { PERSPECTIVE, ORTHOGRAPHIC };
	enum class IlluminationModelType { PHONG, BLINN_PHONG};
	enum class ShadingModelType { FLAT, GOURAUD, PHONG };
	enum class LightType { AMBIENT, POINT, DIRECTIONAL, SPOT };
	enum class MixType { MUL, SUM};
	enum class WireframeType { POINT, LINE, FILL };
}

#endif