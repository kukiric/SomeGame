#ifndef BASEMESHIMPORTER_H
#define BASEMESHIMPORTER_H
#include <Base/Filesystem/File.h>
#include <Base/BinaryMesh.h>

namespace Importers
{
    class BaseMeshImporter
    {
        protected:
            BinaryMesh meshData;
        public:
            BaseMeshImporter();
            virtual void read(const char* filename) = 0;
            virtual void write(const char* destination);
            const BinaryMesh& getMesh() const;
    };
}

#endif // BASEMESHIMPORTER_H