#include "AssimpLoader.h"
#include "Enums.h"

namespace AIGlib {

	AssimpLoader::AssimpLoader(const unsigned int& width, const unsigned int& height) :
		_width(width),
		_height(height)
	{

	}

	AssimpLoader::~AssimpLoader()
	{
		freeResources();
	}

	bool AssimpLoader::loadFile(std::string filePath)
	{
    cleanResources();

		std::cout << "loading '" << filePath << "' file as Assimp Model...\n" << std::endl;

		std::string::size_type slashIndex = filePath.find_last_of("/");
		if (slashIndex == std::string::npos)
		{
			_basePath = ".";
		}
		else if (slashIndex == 0) 
		{
			_basePath = "/";
		}
		else
		{
			_basePath = filePath.substr(0, slashIndex);
		}

		int flags = aiProcess_Triangulate;
		flags |= aiProcess_JoinIdenticalVertices;
		flags |= aiProcess_GenSmoothNormals;
		flags |= aiProcess_GenUVCoords;
		flags |= aiProcess_TransformUVCoords;
		flags |= aiProcess_RemoveComponent;
		flags |= aiProcessPreset_TargetRealtime_Quality;

    _importers.push_back(new Assimp::Importer());
		_scene = _importers.at(_importers.size()-1)->ReadFile(filePath, flags);	
		if (!_scene) 
		{
      std::cerr << _importers.at(_importers.size() - 1)->GetErrorString() << std::endl;
      exit(-1);
		}

		std::cout << "*** Scene properties ***\n" << std::endl;
		std::cout << "HasAnimations: " << _scene->HasAnimations() << std::endl;
		std::cout << "HasCameras: " << _scene->HasCameras() << std::endl;
		std::cout << "HasLights: " << _scene->HasLights() << std::endl;
		std::cout << "HasMaterials: " << _scene->HasMaterials() << std::endl;
		std::cout << "HasMeshes: " << _scene->HasMeshes() << std::endl;
		std::cout << "HasTextures: " << _scene->HasTextures() << std::endl;

		std::cout << "Assimp Model successfully readed.\n" << std::endl;

		return true;
	}

	void AssimpLoader::getMaterials()
	{
		//Reseteamos la lista de materiales por si se hubiese cargado otro objeto antes
		_materialPropertiesIndices.clear();
		std::vector<unsigned int>().swap(_materialPropertiesIndices);

		for (unsigned int i = 0; i < _scene->mNumMaterials; ++i) 
		{
			const aiMaterial* material = _scene->mMaterials[i];
			MaterialProperties* materialProperty = new MaterialProperties();

			//Nombre
			unsigned int max;
			aiString name;
			material->Get(AI_MATKEY_NAME, name);
			materialProperty->setName(name.C_Str());

			materialProperty->setIlluminationModel(IlluminationModelType::PHONG);
			materialProperty->setShadingModel(ShadingModelType::PHONG);
			/*
			//Shading model (en AIGlib IlluminationModel y ShadingModel)
			int shadingModel;
			material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);

			switch (shadingModel)
			{
			case aiShadingMode_Flat:
				materialProperty->setIlluminationModel(IlluminationModelType::PHONG);
				materialProperty->setShadingModel(ShadingModelType::FLAT);
				break;
			case aiShadingMode_Gouraud:
				materialProperty->setIlluminationModel(IlluminationModelType::PHONG);
				materialProperty->setShadingModel(ShadingModelType::GOURAUD);
				break;
			case aiShadingMode_Phong:
				materialProperty->setIlluminationModel(IlluminationModelType::PHONG);
				materialProperty->setShadingModel(ShadingModelType::PHONG);
				break;
			case aiShadingMode_Blinn:
				materialProperty->setIlluminationModel(IlluminationModelType::BLINN_PHONG);
				materialProperty->setShadingModel(ShadingModelType::PHONG);
				break;
			default:
				materialProperty->setIlluminationModel(IlluminationModelType::PHONG);
				materialProperty->setShadingModel(ShadingModelType::PHONG);
				break;
			}
			*/
			//two-sided
			max = 1;
			int twoSided = 0;
			//if ((AI_SUCCESS == aiGetMaterialIntegerArray(material, AI_MATKEY_TWOSIDED, &twoSided, &max)) && twoSided) {
			if ((AI_SUCCESS == material->Get(AI_MATKEY_TWOSIDED, &twoSided, &max)) && twoSided)
			{
				materialProperty->setTwoSided(true);
			}
			else
			{
				materialProperty->setTwoSided(false);
			}

			//wireframe
			max = 1;
			int wireframe = 0;
			//if (AI_SUCCESS == aiGetMaterialIntegerArray(material, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
			if (AI_SUCCESS == material->Get(AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
			{
				materialProperty->setWireframe(wireframe ? WireframeType::LINE : WireframeType::FILL);
			}
			else
			{
				materialProperty->setWireframe(WireframeType::FILL);
			}

			//Colores del material
			aiColor3D ambientK(0.0f, 0.0f, 0.0f);
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, ambientK)) 
			{
				materialProperty->setAmbientK(glm::vec3(ambientK.r, ambientK.g, ambientK.b));
			}

			aiColor3D diffuseK(0.0f, 0.0f, 0.0f);
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK)) 
			{
				materialProperty->setDiffuseK(glm::vec3(diffuseK.r, diffuseK.g, diffuseK.b));
				if (materialProperty->getAmbientSource() == 0) //AmbientK not setted yet
				{ 
					materialProperty->setAmbientK(glm::vec3(diffuseK.r, diffuseK.g, diffuseK.b));
				}
			}

			aiColor3D specularK(0.0f, 0.0f, 0.0f);
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, specularK)) 
			{
				materialProperty->setSpecularK(glm::vec3(specularK.r, specularK.g, specularK.b));
			}

			aiColor3D emissiveK(0.0f, 0.0f, 0.0f);
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveK)) 
			{
				materialProperty->setEmissiveK(glm::vec3(emissiveK.r, emissiveK.g, emissiveK.b));
			}

			aiColor3D opacityK(0.0f, 0.0f, 0.0f);
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_TRANSPARENT, opacityK)) 
			{
				materialProperty->setOpacityK(glm::vec3(opacityK.r, opacityK.g, opacityK.b));
			}

			// Propiedades escalares
			max = 1;
			float shininess = 0.0f;
			if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, &shininess, &max))
			{
				materialProperty->setShininess(shininess);
			}

			max = 1;
			float shininessStrength = 1.0f;
			if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, &shininessStrength, &max)) 
			{
				materialProperty->setShininessStrength(shininessStrength);
			}

			max = 1;
			float reflectivity = 0.0f;
			if (AI_SUCCESS == material->Get(AI_MATKEY_REFLECTIVITY, reflectivity)) 
			{
				materialProperty->setReflectivity(reflectivity);
			}

			max = 1;
			float refractIndex = 1.0f;
			if (AI_SUCCESS == material->Get(AI_MATKEY_REFRACTI, refractIndex))
			{
				materialProperty->setRefractIndex(refractIndex);
			}

			max = 1;
			float opacity = 1.0f;
			if (AI_SUCCESS == material->Get(AI_MATKEY_OPACITY, opacity)) 
			{
				materialProperty->setOpacity(opacity);
			}

			//Texturas del material
			aiString path;
			if (material->GetTextureCount(aiTextureType_AMBIENT) > 0) 
			{
				if (material->GetTexture(aiTextureType_AMBIENT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int ambientTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setAmbientTexId(ambientTexId);
				}
			}

			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int diffuseTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setDiffuseTexId(diffuseTexId);
					if (materialProperty->getAmbientSource() == 0) //AmbientTex not setted yet
					{
						materialProperty->setAmbientTexId(diffuseTexId);
					}
				}
			}

			if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) 
			{
				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int specularTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setSpecularTexId(specularTexId);
				}
			}

			if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0) 
			{
				if (material->GetTexture(aiTextureType_EMISSIVE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int emissiveTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setEmissiveTexId(emissiveTexId);
				}
			}

			if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
			{
				if (material->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int normalTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setNormalTexId(normalTexId);
				}
			}

			if (material->GetTextureCount(aiTextureType_LIGHTMAP) > 0)
			{
				if (material->GetTexture(aiTextureType_LIGHTMAP, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int lightMapTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setLightmapTexId(lightMapTexId);
				}
			}

			if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
			{
				if (material->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int heightTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setHeightTexId(heightTexId);
				}
			}

			if (material->GetTextureCount(aiTextureType_SHININESS) > 0) 
			{
				if (material->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int shininessTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setShininessTexId(shininessTexId);
				}
			}

			if (material->GetTextureCount(aiTextureType_OPACITY) > 0)
			{
				if (material->GetTexture(aiTextureType_OPACITY, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int opacityTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setOpacityTexId(opacityTexId);				
				}
			}

			if (material->GetTextureCount(aiTextureType_DISPLACEMENT) > 0) 
			{
				if (material->GetTexture(aiTextureType_DISPLACEMENT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int displacementTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setDisplacementTexId(displacementTexId);
				}
			}

			if (material->GetTextureCount(aiTextureType_UNKNOWN) > 0) 
			{
				if (material->GetTexture(aiTextureType_UNKNOWN, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
					std::string fullPath = _basePath + "/" + path.data;
					unsigned int unknownTexId = RenderEngine::get().registerTexture(fullPath);
					materialProperty->setAuxiliarTexId(unknownTexId);
				}
			}

			//materialProperties.push_back(materialProperty);
			unsigned int materialPropertiesIndex = RenderEngine::get().addMaterialProperties(materialProperty);
			_materialPropertiesIndices.push_back(materialPropertiesIndex);
		}
	}

	std::vector<GroupNode*> AssimpLoader::getLights()
	{
		std::vector<GroupNode*> lights;

		for (unsigned int i = 0; i < _scene->mNumLights; ++i)
		{
			const aiLight* light = _scene->mLights[i];
			glm::vec3 lightPosition = glm::vec3(light->mPosition.x, light->mPosition.y, light->mPosition.z);
			glm::vec3 lightDirection = glm::vec3(light->mDirection.x, light->mDirection.y, light->mDirection.z);
			glm::vec3 ambientIntensity = glm::vec3(light->mColorAmbient.r, light->mColorAmbient.g, light->mColorAmbient.b);
			glm::vec3 diffuseIntensity = glm::vec3(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b);
			glm::vec3 specularIntensity = glm::vec3(light->mColorSpecular.r, light->mColorSpecular.g, light->mColorSpecular.b);
			float constantAttenuation = light->mAttenuationConstant;
			float linearAttenuation = light->mAttenuationLinear;
			float quadraticAttenuation = light->mAttenuationQuadratic;
			glm::vec3 coneDirection = glm::vec3(light->mDirection.x, light->mDirection.y, light->mDirection.z);
			float coneAngle = light->mAngleInnerCone;
			switch (light->mType)
			{
			case aiLightSourceType::aiLightSource_AMBIENT:
          RenderEngine::get().getScene().setAmbientIntensity(new AmbientIntensity(ambientIntensity));
				break;
			case aiLightSourceType::aiLightSource_POINT:
				if (light->mColorAmbient.IsBlack())
				{
					lights.push_back(new PointLight(lightPosition, constantAttenuation, linearAttenuation, quadraticAttenuation,
						diffuseIntensity, specularIntensity));
				}
				else
				{
					lights.push_back(new PointLight(lightPosition, constantAttenuation, linearAttenuation, quadraticAttenuation,
						ambientIntensity, diffuseIntensity, specularIntensity));
				}

				break;
			case aiLightSourceType::aiLightSource_DIRECTIONAL:
				if (light->mColorAmbient.IsBlack())
				{
					lights.push_back(new DirectionalLight(lightDirection, diffuseIntensity, specularIntensity));
				}
				else
				{
					lights.push_back(new DirectionalLight(lightDirection, ambientIntensity, diffuseIntensity, specularIntensity));
				}
				break;
			case aiLightSourceType::aiLightSource_SPOT:
				if (light->mColorAmbient.IsBlack())
				{
					lights.push_back(new SpotLight(lightPosition, coneDirection, coneAngle, constantAttenuation, linearAttenuation,
						quadraticAttenuation, diffuseIntensity, specularIntensity));
				}
				else
				{
					lights.push_back(new SpotLight(lightPosition, coneDirection, coneAngle, constantAttenuation, linearAttenuation,
						quadraticAttenuation, ambientIntensity, diffuseIntensity, specularIntensity));
				}
				break;
			}
		}

		return lights;
	}
	std::vector<Camera*> AssimpLoader::getCameras()
	{
		std::vector<Camera*> cameras;
		for (unsigned int i = 0; i < _scene->mNumCameras; ++i)
		{
			const aiCamera* camera = _scene->mCameras[i];

			glm::vec3 position = glm::vec3(camera->mPosition.x, camera->mPosition.y, camera->mPosition.z);
			float fov = glm::degrees(camera->mHorizontalFOV);
			float aspectRatio = camera->mAspect;
			float near = camera->mClipPlaneNear;
			float far = camera->mClipPlaneFar;
			std::string name = camera->mName.C_Str();

			cameras.push_back(new Camera(_width, _height, near, far, fov, position, name));
		}
		return cameras;
	}


	std::vector<Mesh*> AssimpLoader::createAllMeshes(glm::vec3 scale, glm::vec3 translation, glm::vec3 rotation, int customShaderId)
	{

		if (RenderEngine::get().getIfCustomShaders()) 
		{
			if (customShaderId == -1)
			{
				std::cout << "Error - You're using custom shaders but custom shader id is -1." << std::endl;
				exit(-1);
			}
			_customShaderId = customShaderId;			
		}
		else
		{
			if (customShaderId != -1) 
			{
				std::cout << "Error - You're trying to use a custom shader id without setCustomShaders enabled." << std::endl;
				exit(-1);
			}
		}

		std::vector<Mesh*> meshes;

		_translation = translation;
		_rotation = rotation;
		_scale = scale;

		if (_scene->HasMaterials())
		{
			getMaterials();
		}

		if (_scene->HasMeshes()) 
		{
			for (unsigned int i = 0; i < _scene->mNumMeshes; ++i) 
			{
				const aiMesh* aiMesh = _scene->mMeshes[i];
				Mesh* mesh = createMesh(aiMesh);
        mesh->translate(_translation);
        mesh->rotate(_rotation);


				meshes.push_back(mesh);

			}
		}

		return meshes;
	}

	Mesh* AssimpLoader::createMesh(const aiMesh* mesh)
	{		
		const std::string meshName = mesh->mName.C_Str();

		std::vector<glm::vec3> vertexPos;
		std::vector<glm::vec4> vertexColor;
		std::vector<glm::vec3> vertexNormal;
		std::vector<glm::vec2> vertexTexCoord;
		std::vector<glm::vec3> vertexTangent;

		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) 
		{
			//vertexPos
			vertexPos.push_back(glm::vec3(mesh->mVertices[i].x * _scale.x, mesh->mVertices[i].y * _scale.y, mesh->mVertices[i].z * _scale.z));
			//vertexColor
			if (mesh->mColors[0]) 
			{
				vertexColor.push_back(glm::vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a));
			}

			//vertexNormal
			if (mesh->mNormals)
			{
				vertexNormal.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
			}

			//vertexTexCoord
			if (mesh->mTextureCoords[0]) 
			{
				vertexTexCoord.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
			}

			//vertexTangent
			if (mesh->mTangents) 
			{
				vertexTangent.push_back(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			/*struct aiFace *face = mesh->mFaces + i;
			indices.push_back(face->mIndices[0]);
			indices.push_back(face->mIndices[1]);
			indices.push_back(face->mIndices[2]);*/
			const aiFace* face = &mesh->mFaces[i];
			for (unsigned int j = 0; j < face->mNumIndices; ++j) 
			{
				indices.push_back(face->mIndices[j]);
			}
		}

		GeometryProperties* modelGeometry = new GeometryProperties(mesh->mNumVertices);
		modelGeometry->addVertexPosAttribute(vertexPos);
		modelGeometry->addVertexColorAttribute(vertexColor);
		modelGeometry->addVertexNormalAttribute(vertexNormal);
		modelGeometry->addVertexTexCoordAttribute(vertexTexCoord);
		modelGeometry->addVertexTangentAttribute(vertexTangent);
		modelGeometry->addIndicesList(mesh->mNumFaces, indices);
    unsigned int modelGeometryIndex = RenderEngine::get().addGeometry(modelGeometry);

		Mesh* objectMesh = new Mesh(mesh->mName.C_Str()); 
		
		objectMesh->setGeometryProperties(modelGeometryIndex);
		if (_materialPropertiesIndices.size() > 0)
		{
			unsigned int materialIndex = _materialPropertiesIndices[mesh->mMaterialIndex];
			objectMesh->setMaterialProperties(materialIndex);
		}
		if (RenderEngine::get().getIfCustomShaders())
		{
			objectMesh->setCustomShaderId(_customShaderId);
		}

		return objectMesh;
	}

	GroupNode* AssimpLoader::createNode(const aiNode* node, GroupNode* root)
	{		
		GroupNode* groupNode = new GroupNode(node->mName.C_Str());
		for (unsigned int i = 0; i < node->mNumMeshes; ++i) 
		{
			unsigned int meshId = node->mMeshes[i];
			const aiMesh* mesh = _scene->mMeshes[meshId];
			Mesh* node = createMesh(mesh);
			delete mesh;
			node->translate(_translation);
			node->rotate(_rotation);
			groupNode->addChildNode(node);
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			groupNode->addChildNode(createNode(node->mChildren[i], groupNode));
		}

		return groupNode;
	}

	GroupNode* AssimpLoader::createSceneGraph(glm::vec3 scale, glm::vec3 translation, glm::vec3 rotation, int customShaderId)
	{
		if (RenderEngine::get().getIfCustomShaders()) 
		{
			if (customShaderId == -1) 
			{
				std::cerr << "Error - You're using custom shaders but custom shader id is -1." << std::endl;
				exit(-1);
			}
			_customShaderId = customShaderId;
		}
		else 
		{
			if (customShaderId != -1) 
			{
				std::cerr << "Error - You're trying to use a custom shader id without setCustomShaders enabled." << std::endl;
				exit(-1);
			}
		}

		_translation = translation;
		_rotation = rotation;
		_scale = scale;

		GroupNode* root = new GroupNode("worldNode");

		//Se registran los materiales, texturas... para usar después con las mallas
		if (_scene->HasMaterials())
		{
			getMaterials(); //Usados con las mallas
		}

		//Cameras
		std::vector<Camera*> cameras;
		if (_scene->HasCameras())
		{
			cameras = getCameras();
			for (const auto& camera : cameras)
			{
				root->addChildNode(camera);
			}
		}

		//Lights
		std::vector<GroupNode*> lights;
		if (_scene->HasLights())
		{
			lights = getLights();
			for (const auto& light : lights)
			{
				root->addChildNode(light);
			}
		}

		//Meshes
		GroupNode* meshes = createNode(_scene->mRootNode, root);
		root->addChildNode(meshes);
		return root;
	}

  void AssimpLoader::cleanResources()
  {
    _basePath = "";
    _scale = glm::vec3(1.0f);
    _translation = glm::vec3(0.0f);
    _rotation = glm::vec3(0.0f);
    _materialPropertiesIndices.clear();
    std::vector<unsigned int>().swap(_materialPropertiesIndices);
    _customShaderId = invalidValue;
  }

	void AssimpLoader::freeResources()
	{
    delete _scene;
	}
}