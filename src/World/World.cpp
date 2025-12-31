#include <glm/ext/matrix_transform.hpp>
#include <world.h>




World::~World()
{
    
    
}

void World::Generate()
{
      // Generate Center Chunk
       int center = 0;
       ChunkData templateChunk = GenerateChunk();
       templateChunk.chunkPos =  glm::vec2(center * CHUNK_X, center * CHUNK_Z);  
       chunkList.emplace(center,templateChunk);
        


      for(int layer = 0; layer < worldSize; layer++)
      {
        for(int x = -layer; x <= layer; x++) 
        {
          for(int z = -layer; z <= layer; z++) 
            {
              if (layer == 0 && x == 0 && z == 0) continue;
              if(chunkExists(x, z)) continue;
              ChunkData chunk = GenerateChunkAt(x, z);
              chunkList.emplace(chunkKey(x,z),chunk);
            }
        }
      }


      
  
  
      //Positive Chunks 
      for (int x = -worldSize; x <= worldSize; ++x){
          for (int z = -worldSize; z <= worldSize; ++z)
           {
              if (x == 0 && z == 0) continue; // skip center
              ChunkData chunk = GenerateChunkAt(x, z);
               chunkList.emplace(chunkKey(x,z),chunk);
            }
      }


}
bool World::chunkExists(int x, int z)
{
  if(chunkList.find(chunkKey(x,  z)) != chunkList.end()) {return true;}
  else return false;
}


ChunkData World::GenerateChunkAt(int x, int z)
{
  ChunkData templateChunk = GenerateChunk();
  templateChunk.chunkPos =  glm::vec2(x * CHUNK_X, z* CHUNK_Z) ;
  return templateChunk;
}


ChunkData World::GenerateChunk()
{
  ChunkData chunk;
for (int z = 0; z < CHUNK_Z; ++z){
    for (int y = 0; y < CHUNK_Y; ++y)
        for (int x = 0; x < CHUNK_X; ++x)
        {
            if (y <= CHUNK_Y) // floor layer
               chunk.blocks[x + CHUNK_X * (y + CHUNK_Y * z)] = 1; // solid block
            else
                chunk.blocks[x + CHUNK_X * (y + CHUNK_Y * z)] = 0; // air
        }
    
    }
    buildNaiveMesh(chunk, chunk.chunkVertices, chunk.chunkIndices);



    glGenVertexArrays(1, &chunk.chunkVAO);
    glGenBuffers(1, &chunk.chunkVBO);
    glGenBuffers(1, &chunk.chunkEBO);

    glBindVertexArray(chunk.chunkVAO);

    glBindBuffer(GL_ARRAY_BUFFER, chunk.chunkVBO);
    glBufferData(GL_ARRAY_BUFFER, chunk.chunkVertices.size() * sizeof(Vertex), chunk.chunkVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.chunkEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk.chunkIndices.size() * sizeof(uint32_t), chunk.chunkIndices.data(), GL_STATIC_DRAW);
   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
  return chunk;
}

void World::Render(Shader* shader, glm::vec3 playerPos)
{
glm::ivec2 playerChunk = glm::ivec2(floor(playerPos.x / CHUNK_X), floor(playerPos.z / CHUNK_Z));
int startX = playerChunk.x - renderDistance;
int endX   = playerChunk.x + renderDistance;
int startZ = playerChunk.y - renderDistance;
int endZ   = playerChunk.y + renderDistance;

for (int x = startX; x <= endX; ++x)
for (int z = startZ; z <= endZ; ++z)
{
    if (!chunkExists(x, z)) continue;

    ChunkData& chunkData = chunkList[chunkKey(x, z)];
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(chunkData.chunkPos.x, 0, chunkData.chunkPos.y));
    shader->setMat4("model", model);

    glBindVertexArray(chunkData.chunkVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(chunkData.chunkIndices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
    }


