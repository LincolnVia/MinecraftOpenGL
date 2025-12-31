#ifndef WORLD_H
#define WORLD_H



#include "chunkData.h"
#include "shader.h"
#include <map>


struct PairHash {
          std::size_t operator()(const std::pair<int, int>& p) const {
              return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
          }
      };

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
      void PrintChunks()
      {
          std::cout << "Total chunks: " << chunkList.size() << std::endl;
          std::cout << "Chunk positions:" << std::endl;
    
          // Create a sorted map for organized output
          std::map<int, std::map<int, bool>> grid;
    
          for(const auto& [key, chunk] : chunkList)
          {
              // Calculate x and z from the chunk position
              int x = static_cast<int>(chunk.chunkPos.x / CHUNK_X);
              int z = static_cast<int>(chunk.chunkPos.y / CHUNK_Z);
        
              grid[z][x] = true;
        
              std::cout << "  Chunk at (" << x << ", " << z << ") - "
                        << "World pos: (" << chunk.chunkPos.x << ", " << chunk.chunkPos.y << ")" 
                        << std::endl;
          }
    
          // Print a visual grid
          std::cout << "\nVisual grid:" << std::endl;
    
          int minZ = grid.begin()->first;
          int maxZ = grid.rbegin()->first;
    
          for(int z = minZ; z <= maxZ; z++)
          {
              for(int x = -worldSize; x <= worldSize; x++)
              {
                  if(grid[z].count(x))
                      std::cout << "■ ";
                  else
                      std::cout << "□ ";
              }
              std::cout << " (z=" << z << ")" << std::endl;
          }
      }      
    
            int worldSize = 6;
    
      
      std::unordered_map<std::pair<int, int>, ChunkData, PairHash>& getChunkList() {return chunkList;};
      
  private:
                const std::vector<unsigned int>& textures; 
      std::unordered_map<std::pair<int, int>, ChunkData, PairHash> chunkList;
       int renderDistance = 16;

  
};






#endif




