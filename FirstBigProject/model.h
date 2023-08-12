#pragma once


#include <vector>
#include <string>
#include <limits>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "mesh.h"
#include "shader2.h"
#include "boundary.h"
#include "CollisionRecorder.h"

class Model {
public:
    enum MODELSTATUS { LOADING, LOADED, ERR };

private:
    std::vector<Mesh> _meshes;
    std::vector<Texture2D> _texturesLoaded;
    std::string _directory;
    MODELSTATUS _status = LOADING;

    Boundary _boundBox; // the model's boundary box, won't change after it's initialized

public:
    Model();
    Model(std::string path);
    ~Model();

private:
    Model(std::vector<Mesh>&& meshes, std::vector<Texture2D>&& textures, std::string directory, Boundary boundBox);

public:
    inline MODELSTATUS status() const { return _status; }
    inline int meshCount() const { return _meshes.size(); }
    inline int textureCount() const { return _texturesLoaded.size(); }
    inline std::string directory() const { return _directory; }
    inline Boundary boundBox() const { return _boundBox; }
    inline const std::vector<Mesh>& meshes() const { return _meshes; }

    Model* copyToCurrentContext() const;

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);

public:
    void render(Shader& shader) const;
    CollisionRecorder hit(const Ray& ray, const QMatrix4x4& modelMatrix) const;    // test the hit record of an input array (transformed into local space)
};
