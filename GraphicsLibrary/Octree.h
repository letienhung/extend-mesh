// Based on Radakan engine (ported from Java) (GNU General Public License v3)
// With major modifications by Ibraheem
// http://radakan.googlecode.com/svn/trunk/simplephysics/src/com/jme/bounding/Octree.java

#pragma once

#include <cmath>

#include "BoundingBox.h"
#include "SimpleDraw.h"

typedef std::set<int> IndexSet;
typedef IndexSet::iterator IndexSetIter;

#include <stack>
using namespace std;

template <typename FaceType = Face>
class OctreeBase
{
private:
	Vector<OctreeBase> children;
	Vector<FaceType> triangleData;

public:
	BoundingBox<FaceType> boundingBox;
	int trianglePerNode;

	OctreeBase(){trianglePerNode = -1;}

	OctreeBase(const Vector<int> & trisIndex, Mesh * mesh, int triPerNode) 
	{
		// just add triangles invovled to "triangleData"
		for(Vector<int>::const_iterator index = trisIndex.begin(); index != trisIndex.end(); index++)
		{
			this->triangleData.push_back(*mesh->f(*index));
		}

		init(triPerNode);
	}

	OctreeBase(StdList<FaceType>& tris, int triPerNode) 
	{
		// add triangles involved to "triangleData"
                for(typename StdList<FaceType>::iterator f = tris.begin(); f != tris.end(); f++)
		{
			this->triangleData.push_back(*f);
		}

		init(triPerNode);
	}

	void init(int triPerNode)
	{
		this->trianglePerNode = triPerNode;

		BoundingBox<FaceType> bb;
		bb.computeFromTris(triangleData);

		//Transform and scale to node's coordinates
		double largeSize = Max(bb.xExtent, Max(bb.yExtent, bb.zExtent));

		this->boundingBox = BoundingBox<FaceType>(bb.center, largeSize, largeSize, largeSize);
	}

	void initBuild( StdList<FaceType>& tris, int triPerNode )
	{
		// add triangles involved to "triangleData"
                for(typename StdList<FaceType>::iterator f = tris.begin(); f != tris.end(); f++)
			this->triangleData.push_back(*f);

		this->trianglePerNode = triPerNode;

		BoundingBox<FaceType> bb;
		bb.computeFromTris(triangleData);

		//Transform and scale to node's coordinates
		double largeSize = Max(bb.xExtent, Max(bb.yExtent, bb.zExtent));
		this->boundingBox = BoundingBox<FaceType>(bb.center, largeSize, largeSize, largeSize);

		// Build the tree
		this->build();
	}

	void build(int depth = 0) 
	{
                if ((int)triangleData.size() > this->trianglePerNode)
		{
			if(depth < trianglePerNode * 0.25f)
			{
				subdivide();

                                for (typename Vector<OctreeBase>::iterator child = children.begin();  child != children.end(); child++)
					child->build(depth + 1);
			}
		} 
	}

	void newNode( double x, double y, double z )
	{
		double extent = boundingBox.xExtent / 2.0;

		Vec center;

		center.x = boundingBox.center.x + (extent * x);
		center.y = boundingBox.center.y + (extent * y);
		center.z = boundingBox.center.z + (extent * z);

		BoundingBox<FaceType> bb(center, extent, extent, extent);

		Vector<FaceType> tris = collectTriangles(bb);

		children.push_back( OctreeBase<FaceType>(*this, bb, tris) );
	}

	void subdivide()
	{
		// 8 nodes
		newNode(-1, -1, -1);
		newNode(1, -1, -1);
		newNode(-1, 1, -1);
		newNode(1, 1, -1);
		newNode(-1, -1, 1);
		newNode(1, -1, 1);
		newNode(-1, 1, 1);
		newNode(1, 1, 1);
	}

	Vector<FaceType> getTriangleData()
	{
		return triangleData;
	}

	OctreeBase( OctreeBase& parent, const BoundingBox<FaceType>& bb, const Vector<FaceType>& tris )
	{
		this->boundingBox = bb;
		this->trianglePerNode = parent.trianglePerNode;
		this->triangleData = tris;
	}

	void draw(double r, double g, double b)
	{
		SimpleDraw::DrawBox(boundingBox.center, boundingBox.xExtent, boundingBox.yExtent, boundingBox.zExtent,r,g,b);

                for (typename Vector<OctreeBase>::iterator child = children.begin();  child != children.end(); child++)
			child->draw(r,g,b);
	}

	Vector<FaceType> collectTriangles(const BoundingBox<FaceType>& bb) 
	{
		Vector<FaceType> triList;

                for(typename Vector<FaceType>::iterator face = triangleData.begin(); face != triangleData.end(); face++)
		{
			if( bb.containsTriangle(face->vec(0), face->vec(1), face->vec(2)) )
				triList.push_back(*face);
		}

		return triList;
	}

	bool intersectHit(IndexSet& tris) 
	{
		if(this->children.size() > 0)
			return false;

                for(typename Vector<FaceType>::iterator face = triangleData.begin(); face != triangleData.end(); face++)
			tris.insert( face->index );

		return true;
	}

	IndexSet intersectPoint(const Vec& point) 
	{
		IndexSet tris;

		if (boundingBox.contains(point)) 
			intersectRecursivePoint(point, tris);

		return tris;
	}

	void intersectRecursivePoint(const Vec& point, IndexSet& tris) 
	{
		if (intersectHit(tris))
			return;

                for (typename Vector<OctreeBase>::iterator child = children.begin();  child != children.end(); child++)
		{
			if (child->boundingBox.contains(point))
				child->intersectRecursivePoint(point, tris);
		}
	}

	IndexSet intersectRay(const Ray& ray) 
	{
		IndexSet tris;

		if (boundingBox.intersects(ray)) 
			intersectRecursiveRay(ray, tris);

		return tris;
	}

	void intersectRecursiveRay(const Ray& ray, IndexSet& tris) 
	{
		if (intersectHit(tris))
			return;

                for (typename Vector<OctreeBase>::iterator child = children.begin();  child != children.end(); child++)
		{
			if (child->boundingBox.intersects(ray))
				child->intersectRecursiveRay(ray, tris);
		}
	}

	IndexSet intersectSphere(const Vec& sphere_center, double radius)
	{
		IndexSet tris;

		if (boundingBox.intersectsSphere(sphere_center, radius)) 
			intersectRecursiveSphere(sphere_center, radius, tris);

		return tris;
	}

	void intersectRecursiveSphere(const Vec& sphere_center, double radius, IndexSet& tris)
	{
		if (intersectHit(tris))
			return;

                for (typename Vector<OctreeBase>::iterator child = children.begin();  child != children.end(); child++)
		{
			if (child->boundingBox.intersectsSphere(sphere_center, radius))
				child->intersectRecursiveSphere(sphere_center, radius, tris);
		}
	}

	IndexSet intersectRaySphere(const Ray& ray, const Vec& sphere_center, double radius)
	{
		IndexSet tris;

		intersectRayBoth(ray, tris);

		// Grow sphere until we intersect ?
		//while(!boundingBox.intersectsSphere(sphere_center, radius))
		//	radius *= 1.25f;

		if(boundingBox.intersectsSphere(sphere_center, radius))
			intersectRecursiveSphere(sphere_center, radius, tris);

		return tris;
	}

	void intersectRayBoth(const Ray& ray, IndexSet & tris) 
	{
		stack<OctreeBase*> trees;

		Ray inverseRay = ray.inverse();

		if (boundingBox.intersects(ray) || boundingBox.intersects(inverseRay))
			trees.push(this);

		while(!trees.empty())
		{
			OctreeBase * t = trees.top(); trees.pop();

			if (!t->intersectHit(tris))
			{
                                for (typename Vector<OctreeBase<FaceType> >::iterator child = t->children.begin(); child != t->children.end(); child++)
				{
					if (child->boundingBox.intersects(ray) || child->boundingBox.intersects(inverseRay))
						trees.push(&(*child));
				}
			}
		}
	}

	int intersectRayBoth(const Ray& ray) 
	{
		stack<OctreeBase*> trees;

                IndexSet tris;

		Ray inverseRay = ray.inverse();

		if (boundingBox.intersects(ray) || boundingBox.intersects(inverseRay))
			trees.push(this);

		while(!trees.empty())
		{
			OctreeBase * t = trees.top(); trees.pop();

			if (!t->intersectHit(tris))
			{
                                for (typename Vector<OctreeBase<FaceType> >::iterator child = t->children.begin(); child != t->children.end(); child++)
				{
					if (child->boundingBox.intersects(ray) || child->boundingBox.intersects(inverseRay))
						trees.push(&(*child));
				}
			}
		}

		return -1;
	}

	FaceType * findClosestTri(const Ray & ray, IndexSet & tris, Mesh * mesh, HitResult & hitRes)
	{
		double minDist = FLT_MAX;
		FaceType *closestFace = NULL, *curr_FaceType = NULL;

		double u = 0.0, v = 0.0;
		double actualMinDist = 0;

		// Find the triangles in this tree
		intersectRayBoth(ray, tris);

		for(IndexSetIter it = tris.begin(); it != tris.end(); it++)
		{
			curr_FaceType = mesh->f(*it);

			curr_FaceType->intersectionTest(ray, hitRes, true);

                        if(hitRes.hit && (abs(hitRes.distance) < minDist))
			{
				closestFace = curr_FaceType;

				minDist = abs(hitRes.distance);
				actualMinDist = hitRes.distance;

				u = hitRes.u; 
				v = hitRes.v;
			}
		}

		if(closestFace)
		{
			// set 'hitRes' to that closest hit result
			hitRes.distance = actualMinDist;	
			hitRes.index = closestFace->index;
			hitRes.u = u;	hitRes.v = v;
			hitRes.hit = true;
		}
		else
		{
			hitRes.hit = false;
			hitRes.distance = FLT_MAX;
			hitRes.index = -1;
		}

		return closestFace;
	}

	/* Perform intersection tests  */
	bool testIntersectHit(const Ray& ray, HitResult & hitRes) 
	{
		if(this->children.size() > 0)
			return false;

		// Do actual intersection test
                for(typename Vector<FaceType>::iterator face = triangleData.begin(); face != triangleData.end(); face++)
		{
			face->intersectionTest(ray, hitRes, true);

			if(hitRes.hit)
				return true;
		}

		return false;
	}

	void testIntersectRayBoth(const Ray& ray, HitResult & hitRes) 
	{
		stack<OctreeBase*> trees;

		Ray inverseRay = ray.inverse();

		if (boundingBox.intersects(ray) || boundingBox.intersects(inverseRay))
			trees.push(this);

		while(!trees.empty())
		{
			OctreeBase * t = trees.top(); trees.pop();

			if (t->testIntersectHit(ray, hitRes))
			{
				return;
			}
			else
			{
                                for (typename Vector<OctreeBase<FaceType> >::iterator child = t->children.begin(); child != t->children.end(); child++)
				{
					if (child->boundingBox.intersects(ray) || child->boundingBox.intersects(inverseRay))
						trees.push(&(*child));
				}
			}
		}
	}
};

typedef OctreeBase<Face> Octree;
typedef OctreeBase<Triangle> OctreeTriangles;