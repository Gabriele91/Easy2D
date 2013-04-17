#ifndef MESH_H
#define MESH_H

#include <Config.h>
#include <Math2D.h>
#include <EString.h>
#include <Resource.h>

namespace Easy2D {


	class Mesh : public Resource<Mesh> {
		
	public:
		// graphics vertex
		struct gVertex{ 
			Vec2 vt,uv;
			//cast op
			gVertex(const Vec4& vtuv){
				vt=vtuv.xy();
				uv=Vec2(vtuv.z,vtuv.w);
			}
			gVertex(const Vec2& v){
				vt=v;
			}
			gVertex(float x,float y,float u,float v):vt(x,y),uv(u,v){}
		};
		//draw mode
		enum DrawMode{
				TRIANGLE,
				TRIANGLE_STRIP,
				LINE_STRIP,
				LINES
		};

	private:
		//buffer GPU
		uint vertexBuffer;
		uint indexBuffer;
		//vba GPU
		uint vbaDraw;
		//Valori CPU
		std::vector<gVertex> mVertexs;
		std::vector<ushort> mIndexs;
		//tipo di disegno
		DrawMode dmode;
		//draw bind mesh
		void __bind();

	protected:

			//AABB
			Vector2D center;
			Vector2D extends;
			Vector2D min,max;

	public:
		//Surface
		Mesh(ResourcesGroup *rsmr=NULL,
			 const String& pathfile="");
		//clear cpu data
		void cpuClear(){ mVertexs.clear();  mIndexs.clear(); }
		//distruttore
		virtual ~Mesh();
		//aabb
		DFORCEINLINE const Vector2D& getCenter() const { return center; }
		DFORCEINLINE const Vector2D& getExtends() const { return extends; }
		//metodo che aggiunge i vertici
        void addVertex(const gVertex& gv);
		void addVertex(float x,float y, float u,float v){ 
			addVertex(gVertex(x,y,u,v));
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