#ifndef MESH_H
#define MESH_H

#include <Config.h>
#include <Math2D.h>
#include <EString.h>
#include <Resource.h>
#include <iostream>

namespace Easy2D {


	class Mesh : public Resource<Mesh> {

		enum DrawMode{
				TRIANGLE,
				TRIANGLE_STRIP,
				LINE,
				LINES
		};
		//buffer GPU
		uint vertexBuffer;
		uint indexBuffer;
		//Valori CPU
		std::vector<Vector4D> mVertexs;
		std::vector<unsigned short> mIndexs;
		//tipo di disegno
		DrawMode dmode;

	protected:

			//AABB
			Vector2D center;
			Vector2D extends;
			Vector2D min,max;

	public:
		//Surface
		Mesh(ResourcesManager<Mesh> *rsmr=NULL,
			 const String& pathfile="");
		//distruttore
		virtual ~Mesh();
		//aabb
		DFORCEINLINE const Vector2D& getCenter() const { return center; }
		DFORCEINLINE const Vector2D& getExtends() const { return extends; }
		//metodo che aggiunge i vertici
        void addVertex(const Vector4D& v4d);
		void addVertex(float x,float y, float u,float v){ 
			addVertex(Vector4D(x,y,u,v));
		}
		void addIndex(ushort mIndex);
		void addTriangleIndexs(ushort v1,ushort v2,ushort v3);
		void addQuadIndexs(ushort v1,ushort v2,ushort v3,ushort v4);
		void setDrawMode(DrawMode dmode);
		void build();
		//resource
		virtual bool load();
		virtual bool unload();
		//draw
		void draw();

	};

};

#endif