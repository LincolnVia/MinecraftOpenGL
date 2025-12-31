#pragma once

#include <stdio.h>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

// Assume you have these constants somewhere
constexpr int CHUNK_X = 16;
constexpr int CHUNK_Y = 16;
constexpr int CHUNK_Z = 16;
constexpr int CHUNK_VOLUME = CHUNK_X * CHUNK_Y * CHUNK_Z;

// Placeholder for chunk data
struct ChunkData {
    std::array<uint16_t, CHUNK_VOLUME> blocks;

    uint16_t get(int x, int y, int z) const {
        return blocks[x + CHUNK_X * (y + CHUNK_Y * z)];
    }

    glm::ivec2 chunkPos;
    
    std::vector<Vertex> chunkVertices;
    std::vector<uint32_t> chunkIndices;
    GLuint chunkVAO = 0;
    GLuint chunkVBO = 0;
    GLuint chunkEBO = 0;

    
};
inline uint16_t getSafe(const ChunkData& chunk, int x, int y, int z) {
    if (x < 0 || x >= CHUNK_X ||
        y < 0 || y >= CHUNK_Y ||
        z < 0 || z >= CHUNK_Z)
        return 0; // treat out-of-bounds as air
    return chunk.get(x, y, z);
}



// UVs are the same for all faces
static const glm::vec2 uvs[4] = {
    {0, 0}, {0, 1}, {1, 1}, {1, 0}
};

// +X face
static const glm::vec3 posXVerts[4] = {
    {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1}
};

// -X face
static const glm::vec3 negXVerts[4] = {
    {0,0,1}, {0,1,1}, {0,1,0}, {0,0,0}
};

// +Y face
static const glm::vec3 posYVerts[4] = {
    {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0}
};

// -Y face
static const glm::vec3 negYVerts[4] = {
    {0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}
};

// +Z face
static const glm::vec3 posZVerts[4] = {
    {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}
};

// -Z face
static const glm::vec3 negZVerts[4] = {
    {1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}
};


inline int64_t chunkKey(int x, int z)
{
    // Use upper 32 bits for x, lower 32 bits for z
    return (static_cast<int64_t>(x) << 32) | (static_cast<int64_t>(z) & 0xFFFFFFFFLL);
}

inline void emitFace(const glm::vec3* faceVerts,
              const glm::vec3& normal,
              const glm::ivec3& voxelPos,
              std::vector<Vertex>& vertices,
              std::vector<uint32_t>& indices)
{
    uint32_t startIndex = static_cast<uint32_t>(vertices.size());

    for (int i = 0; i < 4; ++i) {
        vertices.push_back({
            faceVerts[i] + glm::vec3(voxelPos),
            normal,
            uvs[i]
        });
    }

    indices.insert(indices.end(), {
        startIndex, startIndex + 1, startIndex + 2,
        startIndex, startIndex + 2, startIndex + 3
    });
}
inline void emitCube(const ChunkData& chunk, int x, int y, int z,
                     std::vector<Vertex>& vertices,
                     std::vector<uint32_t>& indices)
{
    glm::ivec3 p(x, y, z);

    if (getSafe(chunk, x+1, y, z) == 0) emitFace(posXVerts, {1,0,0}, p, vertices, indices);
    if (getSafe(chunk, x-1, y, z) == 0) emitFace(negXVerts, {-1,0,0}, p, vertices, indices);
    if (getSafe(chunk, x, y+1, z) == 0) emitFace(posYVerts, {0,1,0}, p, vertices, indices);
    if (getSafe(chunk, x, y-1, z) == 0) emitFace(negYVerts, {0,-1,0}, p, vertices, indices);
    if (getSafe(chunk, x, y, z+1) == 0) emitFace(posZVerts, {0,0,1}, p, vertices, indices);
    if (getSafe(chunk, x, y, z-1) == 0) emitFace(negZVerts, {0,0,-1}, p, vertices, indices);
}


inline void buildNaiveMesh(const ChunkData& chunk,
                    std::vector<Vertex>& vertices,
                    std::vector<uint32_t>& indices)
{
    vertices.clear();
    indices.clear();

    for (int z = 0; z < CHUNK_Z; ++z)
    for (int y = 0; y < CHUNK_Y; ++y)
    for (int x = 0; x < CHUNK_X; ++x)
    {
        if(chunk.get(x,y,z) == 0) continue;
      emitCube(chunk, x,y, z, vertices, indices);
                      
    }
}
