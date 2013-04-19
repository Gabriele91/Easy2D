#include <stdafx.h>
#include <ResourcesGroup.h>
//////////////////////////////
using namespace Easy2D;
//////////////////////////////
// GCC SUCK //
/*
* Texture manager:
*/
template <>
/** load texture */
DFORCEINLINE Texture::ptr ResourcesGroup::load<Texture>(const String& path){
    return textures.load(path);
}
template <>
/** get texture (but not load in memory) */
DFORCEINLINE Texture::ptr ResourcesGroup::get<Texture>(const String& path){
    return textures.get(path);
}
template<>
/** find a texture already returned */
DFORCEINLINE Texture::ptr ResourcesGroup::find<Texture>(const String& path){
    return textures.find(path);
}
/** directory texture resources */
template<>
DFORCEINLINE String ResourcesGroup::getResourceDirectory<Texture>(){
    return textures.mapResources.getPath().getDirectory();
}
/*
* Table manager:
*/
template<>
/** load table */
DFORCEINLINE Table::ptr ResourcesGroup::load<Table>(const String& path){
    return tables.load(path);
}
template<>
/** get table  (but not load in memory) */
DFORCEINLINE Table::ptr ResourcesGroup::get<Table>(const String& path){
    return tables.get(path);
}
template<>
/** find a table already returned */
DFORCEINLINE Table::ptr ResourcesGroup::find<Table>(const String& path){
    return tables.find(path);
}
/** directory table resources */
template<>
DFORCEINLINE String ResourcesGroup::getResourceDirectory<Table>(){
    return tables.mapResources.getPath().getDirectory();
}

/*
* Mesh manager:
*/
template<>
/** load mesh */
DFORCEINLINE Mesh::ptr ResourcesGroup::load<Mesh>(const String& path){
    return meshes.load(path);
}
template<>
/** get mesh  (but not load in memory) */
DFORCEINLINE Mesh::ptr ResourcesGroup::get<Mesh>(const String& path){
    return meshes.get(path);
}
template<>
/** find a mesh already returned */
DFORCEINLINE Mesh::ptr ResourcesGroup::find<Mesh>(const String& path){
    return meshes.find(path);
}
/** directory mesh resources */
template<>
DFORCEINLINE String ResourcesGroup::getResourceDirectory<Mesh>(){
    return meshes.mapResources.getPath().getDirectory();
}
