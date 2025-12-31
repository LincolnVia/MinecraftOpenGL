#ifndef WORLD_H
#define WORLD_H



#include "chunkData.h"
#include "shader.h"
class World
{
  public:

      World(const std::vector<unsigned int>& _textures)
        : textures(_textures) {}

      ~World();
       void Generate();
       ChunkData GenerateChunk();
       ChunkData GenerateChunkAt(int x, int z);
       bool chunkExists(int x, int z);
       void Render(Shader* shader, glm::vec3 playerPos);
      void PrintChunks() const
      {
          std::cout << "Generated chunks:\n";
          for (const auto& [key, chunk] : chunkList) {
              std::cout << "Chunk at (" << chunk.chunkPos.x << ", " << chunk.chunkPos.y << ")\n";
          }
      }
      
    
            int worldSize = 6;
    
      
      std::unordered_map<int, ChunkData>& getChunkList() {return chunkList;};
      
  private:
      const std::vector<unsigned int>& textures; 
      std::unordered_map<int, ChunkData> chunkList;
      int renderDistance = 6;

  
};






#endif




