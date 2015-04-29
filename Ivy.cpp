/**************************************************************************************
**
**  Copyright (C) 2006 Thomas Luft, University of Konstanz. All rights reserved.
**
**  This file is part of the Ivy Generator Tool.
**
**  This program is free software; you can redistribute it and/or modify it
**  under the terms of the GNU General Public License as published by the
**  Free Software Foundation; either version 2 of the License, or (at your
**  option) any later version.
**  This program is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
**  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
**  for more details.
**  You should have received a copy of the GNU General Public License along
**  with this program; if not, write to the Free Software Foundation,
**  Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA 
**
***************************************************************************************/


#include "Ivy.h"
#include "Common.h"


Ivy::Ivy()
{
	resetSettings();
}


void Ivy::resetSettings()
{
	primaryWeight = 0.5f;

	randomWeight = 0.2f;

	gravityWeight = 1.0f;

	adhesionWeight = 0.1f;


	branchingProbability = 0.95f;

	leafProbability = 0.7;


	ivySize = 0.005f;

	ivyLeafSize = 1.5f;

	ivyBranchSize = 0.15;


	maxFloatLength = 0.1f;

	maxAdhesionDistance = 0.1f;
}


void Ivy::seed(const Vector3d &seedPos)
{	
	BasicMesh::reset();

	roots.clear();


	IvyNode tmpNode;

	tmpNode.pos = seedPos;

	tmpNode.primaryDir = Vector3d(0.0f, 1.0f, 0.0f);

	tmpNode.adhesionVector = Vector3d(0.0f, 0.0f, 0.0f);

	tmpNode.length = 0.0f;

	tmpNode.floatingLength = 0.0f;

	tmpNode.climb = true;


	IvyRoot tmpRoot;

	tmpRoot.nodes.push_back( tmpNode );

	tmpRoot.alive = true;

	tmpRoot.parents = 0;


	roots.push_back( tmpRoot );
}


void Ivy::grow()
{
	//parameters that depend on the scene object bounding sphere
	float local_ivySize = Common::mesh.boundingSphereRadius * ivySize;

	float local_maxFloatLength = Common::mesh.boundingSphereRadius * maxFloatLength;

	
	//normalize weights of influence
	float sum = primaryWeight + randomWeight + adhesionWeight;

	primaryWeight /= sum;

	randomWeight /= sum;

	adhesionWeight /= sum;


	//lets grow
	for (std::vector<IvyRoot>::iterator root = roots.begin(); root != roots.end(); ++root)
	{
		//process only roots that are alive
		if (!root->alive) continue;

		//let the ivy die, if the maximum float length is reached
		if (root->nodes.back().floatingLength > local_maxFloatLength) root->alive = false;


		//grow vectors: primary direction, random influence, and adhesion of scene objectss

			//primary vector = weighted sum of previous grow vectors
			Vector3d primaryVector = root->nodes.back().primaryDir;
			
			//random influence plus a little upright vector
			Vector3d randomVector = Vector3d::getNormalized( Vector3d::getRandomized() + Vector3d(0.0f, 0.2f, 0.0f) );

			//adhesion influence to the nearest triangle = weighted sum of previous adhesion vectors
			Vector3d adhesionVector = computeAdhesion(root->nodes.back().pos);	

			//compute grow vector
			Vector3d growVector = local_ivySize * (primaryVector * primaryWeight + randomVector * randomWeight + adhesionVector * adhesionWeight);


		//gravity influence

			//compute gravity vector
			Vector3d gravityVector = local_ivySize * Vector3d(0.0f, -1.0f, 0.0f) * gravityWeight; 

			//gravity depends on the floating length
			gravityVector *= pow(root->nodes.back().floatingLength / local_maxFloatLength, 0.7f);


		//next possible ivy node
	
			//climbing state of that ivy node, will be set during collision detection
			bool climbing;

			//compute position of next ivy node
			Vector3d newPos = root->nodes.back().pos + growVector + gravityVector;

			//combine alive state with result of the collision detection, e.g. let the ivy die in case of a collision detection problem
			root->alive = root->alive & computeCollision(root->nodes.back().pos, newPos, climbing);

			//update grow vector due to a changed newPos
			growVector = newPos - root->nodes.back().pos - gravityVector;
	

		//create next ivy node
			IvyNode tmpNode;

			tmpNode.pos = newPos;

			tmpNode.primaryDir = Vector3d::getNormalized( 0.5f * root->nodes.back().primaryDir + 0.5f * Vector3d::getNormalized( growVector ) );

			tmpNode.adhesionVector = adhesionVector;

			tmpNode.length = root->nodes.back().length + Vector3d::getLength(newPos - root->nodes.back().pos);

			tmpNode.floatingLength = climbing ? 0.0f : root->nodes.back().floatingLength + Vector3d::getLength(newPos - root->nodes.back().pos);

			tmpNode.climb = climbing;

		root->nodes.push_back( tmpNode );
	}



	//lets produce child ivys
	for (std::vector<IvyRoot>::iterator root = roots.begin(); root != roots.end(); ++root)
	{
		//process only roots that are alive
		if (!root->alive) continue;

		//process only roots up to hierarchy level 3, results in a maximum hierarchy level of 4
		if (root->parents > 3) continue;


		//add child ivys on existing ivy nodes
		for (std::vector<IvyNode>::iterator node = root->nodes.begin(); node != root->nodes.end(); ++node)
		{
			//weight depending on ratio of node length to total length
			float weight = 1.0f - ( cos( node->length / root->nodes.back().length * 2.0f * PI) * 0.5f + 0.5f );

			//random influence
			float probability = rand()/(float)RAND_MAX;

			if (probability * weight > branchingProbability)
			{
				//new ivy node
				IvyNode tmpNode;

				tmpNode.pos = node->pos;

				tmpNode.primaryDir = Vector3d(0.0f, 1.0f, 0.0f);

				tmpNode.adhesionVector = Vector3d(0.0f, 0.0f, 0.0f);

				tmpNode.length = 0.0f;

				tmpNode.floatingLength = node->floatingLength;

				tmpNode.climb = true;


				//new ivy root
				IvyRoot tmpRoot;

				tmpRoot.nodes.push_back( tmpNode );

				tmpRoot.alive = true;

				tmpRoot.parents = root->parents + 1;


				roots.push_back( tmpRoot );


				//limit the branching to only one new root per iteration, so return
				return;
			}
		}
	}
}


Vector3d Ivy::computeAdhesion(const Vector3d &pos)
{
	//the resulting adhesion vector
	Vector3d adhesionVector;


	//define a maximum distance
	float local_maxAdhesionDistance = Common::mesh.boundingSphereRadius * maxAdhesionDistance;

	float minDistance = local_maxAdhesionDistance;


	//find nearest triangle
	for (std::vector<BasicTriangle>::iterator t = Common::mesh.triangles.begin(); t != Common::mesh.triangles.end(); ++t)
	{
		//scalar product projection
		float nq = Vector3d::dotProduct(t->norm, pos - t->v0->pos);

		//continue if backside of triangle
		if ( nq < 0.0f ) continue;

		//project last node onto triangle plane, e.g. scalar product projection
		Vector3d p0 = pos - t->norm * nq;

		//compute barycentric coordinates of p0
		float alpha, beta, gamma;
			
		if (Vector3d::getBarycentricCoordinates(t->v0->pos, t->v1->pos, t->v2->pos, p0, alpha, beta, gamma))
		{
			//compute distance
			float distance = Vector3d::getLength(p0 - pos);

			//find shortest distance
			if (distance < minDistance)
			{
				minDistance = distance;

				adhesionVector = Vector3d::getNormalized(p0 - pos);
					
				//distance dependent adhesion vector
				adhesionVector *= 1.0f - distance / local_maxAdhesionDistance;
			}
		}
	}

	return adhesionVector;
}



bool Ivy::computeCollision(const Vector3d &oldPos, Vector3d &newPos, bool &climbing)
{
	//reset climbing state
	climbing = false;

	bool intersection;
	
	int deadlockCounter = 0;

	do
	{
		intersection = false;

		for (std::vector<BasicTriangle>::iterator t = Common::mesh.triangles.begin(); t != Common::mesh.triangles.end(); ++t)
		{
			//compute intersection with triangle plane parametrically: intersectionPoint = oldPos + (newPos - oldPos) * t0;
			float t0 = -Vector3d::dotProduct( t->norm, oldPos - t->v0->pos ) / Vector3d::dotProduct( t->norm, newPos - oldPos );

			//plane intersection
			if ((t0 >= 0.0f) && ( t0 <= 1.0f))
			{
				//intersection point
				Vector3d intersectionPoint = oldPos + (newPos - oldPos) * t0;

				float alpha, beta, gamma;

				//triangle intersection
				if (Vector3d::getBarycentricCoordinates(t->v0->pos, t->v1->pos, t->v2->pos, intersectionPoint, alpha, beta, gamma))
				{                    
					//test on entry or exit of the triangle mesh
					bool entry = Vector3d::dotProduct( t->norm, newPos - oldPos) < 0.0f ? true : false;

					if (entry)
					{
						//project newPos to triangle plane
						Vector3d p0 = newPos - t->norm * Vector3d::dotProduct(t->norm, newPos - t->v0->pos);

						//mirror newPos at triangle plane
						newPos += 2.0f * (p0 - newPos);

						intersection = true;

						climbing = true;
					}
				}
			}
		}

		//abort climbing and growing if there was a collistion detection problem
		if (deadlockCounter++ > 5)
		{
			return false;
		}
  	}
	while (intersection);

	return true;
}


void Ivy::birth()
{
	//evolve a gaussian filter over the adhesian vectors

	float gaussian[11] = {1.0f, 2.0f, 4.0f, 7.0f, 9.0f, 10.0f, 9.0f, 7.0f, 4.0f, 2.0f, 1.0f }; 
	
	for (std::vector<IvyRoot>::iterator root = roots.begin(); root != roots.end(); ++root)
	{
		for (int g = 0; g < 5; ++g)
		{
			for (std::vector<IvyNode>::iterator node = root->nodes.begin(); node != root->nodes.end(); ++node)
			{
				Vector3d e;

				for (int i = -5; i <= 5; ++i)
				{
					Vector3d tmpAdhesion;

					if ((node + i) < root->nodes.begin()) tmpAdhesion = root->nodes.front().adhesionVector;
					if ((node + i) >= root->nodes.end()) tmpAdhesion = root->nodes.back().adhesionVector;
					if (((node + i) >= root->nodes.begin()) && ((node + i) < root->nodes.end())) tmpAdhesion = (node + i)->adhesionVector;

					e += tmpAdhesion * gaussian[i+5];
				}

				node->smoothAdhesionVector = e / 56.0f;
			}

			for (std::vector<IvyNode>::iterator node = root->nodes.begin(); node != root->nodes.end(); ++node)
			{
				node->adhesionVector = node->smoothAdhesionVector;
			}
		}
	}


	//parameters that depend on the scene object bounding sphere
	float local_ivyLeafSize = Common::mesh.boundingSphereRadius * ivySize * ivyLeafSize;

	float local_ivyBranchSize = Common::mesh.boundingSphereRadius * ivySize * ivyBranchSize;


	//reset existing geometry
	BasicMesh::reset();


	//set data path
	path = "../textures/";


	//create material for leafs
	BasicMaterial tmpMaterial;

	tmpMaterial.id = 1;
	tmpMaterial.name = "leaf_adult";
	tmpMaterial.texFile = "efeu1.png";
	
	materials.push_back( tmpMaterial );


	//create second material for leafs
	tmpMaterial.id = 2;
	tmpMaterial.name = "leaf_young";
	tmpMaterial.texFile = "efeu0.png";
	
	materials.push_back( tmpMaterial );


	//create material for branches
	tmpMaterial.id = 3;
	tmpMaterial.name = "branch";
	tmpMaterial.texFile = "efeu_branch.png";
	
	materials.push_back( tmpMaterial );


	//create leafs
	for (std::vector<IvyRoot>::iterator root = roots.begin(); root != roots.end(); ++root)
	{
		//simple multiplier, just to make it a more dense
		for (int i = 0; i < 10; ++i)
		{
			//srand(i + (root - roots.begin()) * 10);

			for (std::vector<IvyNode>::iterator node = root->nodes.begin(); node != root->nodes.end(); ++node)
			{
				//weight depending on ratio of node length to total length
				float weight = pow(node->length / root->nodes.back().length, 0.7f);

				//test: the probability of leaves on the ground is increased
				float groundIvy = std::max<float>(0.0f, -Vector3d::dotProduct( Vector3d(0.0f, 1.0f, 0.0f), Vector3d::getNormalized(node->adhesionVector) ));
				weight += groundIvy * pow(1.0f - node->length / root->nodes.back().length, 2.0f);
				
				//random influence
				float probability = rand()/(float)RAND_MAX;

				if (probability * weight > leafProbability)
				{
					//alignment weight depends on the adhesion "strength"
					float alignmentWeight = node->adhesionVector.length();

					//horizontal angle (+ an epsilon vector, otherwise there's a problem at 0° and 90°... mmmh)
					float phi = Vector2d::vectorToPolar( Vector2d::getNormalized( Vector2d(node->adhesionVector.z, node->adhesionVector.x) ) + Vector2d::getEpsilon() ) - PI * 0.5f;

					//vertical angle, trimmed by 0.5
					float theta = Vector3d::getAngle( node->adhesionVector, Vector3d(0.0f, -1.0f, 0.0f) ) * 0.5f;

					//center of leaf quad
					Vector3d center = node->pos + Vector3d::getRandomized() * local_ivyLeafSize;

					//size of leaf
					float sizeWeight = 1.5f - (cos(weight * 2.0f * PI) * 0.5f + 0.5f);


					//random influence
					phi += (rand()/(float)RAND_MAX - 0.5f) * (1.3f - alignmentWeight);

					theta += (rand()/(float)RAND_MAX - 0.5f) * (1.1f - alignmentWeight);


                    
					//create vertices
					BasicVertex tmpVertex;

					tmpVertex.pos = center + Vector3d(-local_ivyLeafSize * sizeWeight, 0.0f, local_ivyLeafSize * sizeWeight);
					tmpVertex.pos = Vector3d::rotateAroundAxis(tmpVertex.pos, center, Vector3d(0.0f, 0.0f, 1.0f), theta);					
					tmpVertex.pos = Vector3d::rotateAroundAxis(tmpVertex.pos, center, Vector3d(0.0f, 1.0f, 0.0f), phi);
					tmpVertex.pos += Vector3d::getRandomized() * local_ivyLeafSize * sizeWeight * 0.5f;
					vertices.push_back( tmpVertex );

					tmpVertex.pos = center + Vector3d( local_ivyLeafSize * sizeWeight, 0.0f, local_ivyLeafSize * sizeWeight);
					tmpVertex.pos = Vector3d::rotateAroundAxis(tmpVertex.pos, center, Vector3d(0.0f, 0.0f, 1.0f), theta);					
					tmpVertex.pos = Vector3d::rotateAroundAxis(tmpVertex.pos, center, Vector3d(0.0f, 1.0f, 0.0f), phi);					
					tmpVertex.pos += Vector3d::getRandomized() * local_ivyLeafSize * sizeWeight * 0.5f;
					vertices.push_back( tmpVertex );

					tmpVertex.pos = center + Vector3d(-local_ivyLeafSize * sizeWeight, 0.0f, -local_ivyLeafSize * sizeWeight);
					tmpVertex.pos = Vector3d::rotateAroundAxis(tmpVertex.pos, center, Vector3d(0.0f, 0.0f, 1.0f), theta);					
					tmpVertex.pos = Vector3d::rotateAroundAxis(tmpVertex.pos, center, Vector3d(0.0f, 1.0f, 0.0f), phi);
					tmpVertex.pos += Vector3d::getRandomized() * local_ivyLeafSize * sizeWeight * 0.5f;
					vertices.push_back( tmpVertex );

					tmpVertex.pos = center + Vector3d( local_ivyLeafSize * sizeWeight, 0.0f, -local_ivyLeafSize * sizeWeight);
					tmpVertex.pos = Vector3d::rotateAroundAxis(tmpVertex.pos, center, Vector3d(0.0f, 0.0f, 1.0f), theta);					
					tmpVertex.pos = Vector3d::rotateAroundAxis(tmpVertex.pos, center, Vector3d(0.0f, 1.0f, 0.0f), phi);
					tmpVertex.pos += Vector3d::getRandomized() * local_ivyLeafSize * sizeWeight * 0.5f;
					vertices.push_back( tmpVertex );


					//create texCoords
					BasicTexCoord tmpTexCoord;

					tmpTexCoord.pos = Vector2d( 0.0f, 1.0f);
					texCoords.push_back( tmpTexCoord );

					tmpTexCoord.pos = Vector2d( 1.0f, 1.0f);
					texCoords.push_back( tmpTexCoord );

					tmpTexCoord.pos = Vector2d( 0.0f, 0.0f);
					texCoords.push_back( tmpTexCoord );

					tmpTexCoord.pos = Vector2d( 1.0f, 0.0f);
					texCoords.push_back( tmpTexCoord );


					//create triangle
					BasicTriangle tmpTriangle;

					tmpTriangle.matid = 1;

					float probability = rand()/(float)RAND_MAX;
					if (probability * weight > leafProbability) tmpTriangle.matid = 2;

					tmpTriangle.v0id = vertices.size()-1;
					tmpTriangle.v1id = vertices.size()-3;
					tmpTriangle.v2id = vertices.size()-2;

					tmpTriangle.t0id = vertices.size()-1;
					tmpTriangle.t1id = vertices.size()-3;
					tmpTriangle.t2id = vertices.size()-2;

					triangles.push_back( tmpTriangle );				

					tmpTriangle.v0id = vertices.size()-2;
					tmpTriangle.v1id = vertices.size()-0;
					tmpTriangle.v2id = vertices.size()-1;

					tmpTriangle.t0id = vertices.size()-2;
					tmpTriangle.t1id = vertices.size()-0;
					tmpTriangle.t2id = vertices.size()-1;

					triangles.push_back( tmpTriangle );	
				}
			}
		}
	}



	//branches
	for (std::vector<IvyRoot>::iterator root = roots.begin(); root != roots.end(); ++root)
	{
		//process only roots with more than one node
		if (root->nodes.size() == 1) continue;


		//branch diameter depends on number of parents
		float local_ivyBranchDiameter = 1.0f / (float)(root->parents + 1) + 1.0f;


        for (std::vector<IvyNode>::iterator node = root->nodes.begin(); node != root->nodes.end()-1; ++node)
		{
			//weight depending on ratio of node length to total length
			float weight = node->length / root->nodes.back().length;


			//create trihedral vertices
			Vector3d up = Vector3d(0.0f, -1.0f, 0.0f);

			Vector3d basis = Vector3d::getNormalized((node + 1)->pos - node->pos);

			Vector3d b0 = Vector3d::getNormalized( Vector3d::crossProduct(up, basis) ) * local_ivyBranchDiameter * local_ivyBranchSize * (1.3f - weight) + node->pos;

			Vector3d b1 = Vector3d::rotateAroundAxis(b0, node->pos, basis, 2.09f);

			Vector3d b2 = Vector3d::rotateAroundAxis(b0, node->pos, basis, 4.18f);		


			//create vertices
			BasicVertex tmpVertex;

			tmpVertex.pos = b0;
			vertices.push_back( tmpVertex );

			tmpVertex.pos = b1;
			vertices.push_back( tmpVertex );

			tmpVertex.pos = b2;
			vertices.push_back( tmpVertex );


			//create texCoords
			BasicTexCoord tmpTexCoord;

			float texV = (node - root->nodes.begin()) % 2 == 0 ? 1.0f : 0.0f;

			tmpTexCoord.pos = Vector2d( 0.0f, texV);
			texCoords.push_back( tmpTexCoord );

			tmpTexCoord.pos = Vector2d( 0.3f, texV);
			texCoords.push_back( tmpTexCoord );

			tmpTexCoord.pos = Vector2d( 0.6f, texV);
			texCoords.push_back( tmpTexCoord );


			if (node == root->nodes.begin()) continue;


			//create triangle
			BasicTriangle tmpTriangle;

			tmpTriangle.matid = 3;

			tmpTriangle.v0id = vertices.size()-3;
			tmpTriangle.v1id = vertices.size()-0;
			tmpTriangle.v2id = vertices.size()-4;

			tmpTriangle.t0id = vertices.size()-3;
			tmpTriangle.t1id = vertices.size()-0;
			tmpTriangle.t2id = vertices.size()-4;

			triangles.push_back( tmpTriangle );


			tmpTriangle.v0id = vertices.size()-4;
			tmpTriangle.v1id = vertices.size()-0;
			tmpTriangle.v2id = vertices.size()-1;

			tmpTriangle.t0id = vertices.size()-4;
			tmpTriangle.t1id = vertices.size()-0;
			tmpTriangle.t2id = vertices.size()-1;

			triangles.push_back( tmpTriangle );	


			tmpTriangle.v0id = vertices.size()-4;
			tmpTriangle.v1id = vertices.size()-1;
			tmpTriangle.v2id = vertices.size()-5;

			tmpTriangle.t0id = vertices.size()-4;
			tmpTriangle.t1id = vertices.size()-1;
			tmpTriangle.t2id = vertices.size()-5;

			triangles.push_back( tmpTriangle );	


			tmpTriangle.v0id = vertices.size()-5;
			tmpTriangle.v1id = vertices.size()-1;
			tmpTriangle.v2id = vertices.size()-2;

			tmpTriangle.t0id = vertices.size()-5;
			tmpTriangle.t1id = vertices.size()-1;
			tmpTriangle.t2id = vertices.size()-2;

			triangles.push_back( tmpTriangle );	


			tmpTriangle.v0id = vertices.size()-5;
			tmpTriangle.v1id = vertices.size()-2;
			tmpTriangle.v2id = vertices.size()-0;

			tmpTriangle.t0id = vertices.size()-5;
			tmpTriangle.t1id = vertices.size()-2;
			tmpTriangle.t2id = vertices.size()-0;

			triangles.push_back( tmpTriangle );	


			tmpTriangle.v0id = vertices.size()-5;
			tmpTriangle.v1id = vertices.size()-0;
			tmpTriangle.v2id = vertices.size()-3;

			tmpTriangle.t0id = vertices.size()-5;
			tmpTriangle.t1id = vertices.size()-0;
			tmpTriangle.t2id = vertices.size()-3;

			triangles.push_back( tmpTriangle );	
		}
	}


	//initialize ivy mesh
	loadTextures();

	prepareData();

	calculateVertexNormals();

	prepareData();

	createDisplayList(true);
}