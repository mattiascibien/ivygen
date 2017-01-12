#include "objexporter.h"

#include "obj_writer.h"

ObjExporter::ObjExporter(QObject *parent) :
    ExporterInterface()
{
}

bool ObjExporter::save(const std::string &path, const std::string &file, BasicMesh &model)
{
    return OBJWriter::writeOBJ(path, file, model);
}
