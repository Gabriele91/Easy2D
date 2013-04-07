#include <stdafx.h>
#include "Mesh.h"
using namespace Easy2D;
//
Mesh::Mesh(ResourcesManager<Mesh> *rsmr,
		   const String& pathfile)
		  :Resource(rsmr,pathfile)

{}
//distruttore
Mesh::~Mesh(){}
//metodo che aggiunge i vertici
void Mesh::addVertex(const Vector4D& v4d){}
void Mesh::addIndex(ushort mIndex){}
void Mesh::addTriangleIndexs(ushort v1,ushort v2,ushort v3){}
void Mesh::addQuadIndexs(ushort v1,ushort v2,ushort v3,ushort v4){}
void Mesh::setDrawMode(DrawMode dmode){}
void Mesh::build(){}
//resource
bool Mesh::load(){return false;}
bool Mesh::unload(){return false;}
//draw
void Mesh::draw(){}