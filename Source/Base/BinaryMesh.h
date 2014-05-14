#ifndef BINARYMESH_H
#define BINARYMESH_H
#include <Base/SharedTypes.h>

struct BinaryMesh
{
    // Header (64 bytes)
    struct header_t {
    uint8_t magic[3];
    uint8_t version;
    uint32_t nIndices;
    uint32_t nVerts;
    char name[52];
    } header;
    // Data (variable size)
    Vec3* verts;
    Vec2* texCoords;
    Vec3* normals;
    uint32_t* indices;
    // Verification checksum (4 bytes)
    uint32_t checksum;

    /**
     * @brief Sets all fields of a mesh to their initial values
     */
    static void initialize(BinaryMesh* mesh);
    /**
     * @brief Allocates space for vertex data according to the values set in the header
     */
    static void allocData(BinaryMesh* mesh);
    /**
     * @brief Clears the data previously allocated by allocData()
     */
    static void clearData(BinaryMesh* mesh);
    /**
     * @brief Updates the checksum of the mesh for writing
     */
    static void updateChecksum(BinaryMesh* mesh);
    /**
     * @brief Checks if the mesh has a valid header and checksum
     */
    static bool isValid(const BinaryMesh* mesh);
    /**
     * @brief Reads a mesh from memory into a mesh struct and validates it
     * @param src Buffer to read from
     * @param size Size of buffer
     * @throws InitializationException if the header or checksum read from src is invalid
     */
    static void read(BinaryMesh* mesh, const Byte* src, size_t size);
};

#endif // BINARYMESH_H
