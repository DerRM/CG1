/* ----------------------------------------------------------------
   name:           objmodel.cpp
   purpose:        code for loading and drawing wavefront .obj files
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#ifdef __APPLE__ 
#include <GLUT/glut.h>
#elif _WIN32
#include "win32/glut.h"
#else
#include <GL/glut.h>
#endif


#include <limits>

#include "glm/glm.hpp"

#include "objmodel.h"

using namespace std;

// use this with care
// might cause name collisions
using namespace glm;

OBJModel::OBJModel(void){}

OBJModel::OBJModel(string name){
    load(name);
    center();
    unitize();
    calculateNormals();
}

// load a model	
void OBJModel::load(string filename){

	    /**
	     * .obj file format
	     *
	     * '#'  -  comment
	     * 'v'  -  vertex position (3 floats separated by whitespace)
	     * 'vt' -  texture coordinates (2 floats separated by whitespace)
	     * 'vn' -  vertex normal (3 floats separated by whitespace)
	     * 'f'  -  faces, n times 3 v/vt/vn indices separated by /  and whitespace: v/t/n v/t/n ... v/t/n
	     * 'g'  -  group, 
	     */
	 
	    const string POSITION= "v";
	    const string TEX_COORD= "vt";
	    const string NORMAL= "vn";
	    const string FACE= "f";
	    const string GROUP= "g";
	    const string USE_MTL= "usemtl";
	    const string MTL_GROUP= "group";
	    const string MTL_LIB= "mtllib";


	    positions.clear();
	    faces.clear();
	    groups.clear();
	    materials.clear();
	    
	    // end last (and possibly only) group
	    Group begin= {"default", 0, ""};
	    groups.push_back(begin);
	    
	    string group= "";

	    boundingSphereRadius= 0;

	    ifstream file;
	    file.open(filename.c_str());
	 
	    vector<GLuint> pi;
	    vector<GLuint> ti;
	    vector<GLuint> ni;
	    string l;

	    while(getline(file, l)){  

	      stringstream line(l);
	      string type;
	      line >> type;

	      if(type == POSITION){

		float x, y, z;
		line >> x >> y >> z;
		positions.push_back(x);
		positions.push_back(y);
		positions.push_back(z);
	      }
	      else if(type == TEX_COORD){

		float u, v;
		line >> u >> v;
		texCoordsRAW.push_back(u);
		texCoordsRAW.push_back(v);
	      }
	      else if(type == NORMAL){

		float nx, ny, nz;
		line >> nx >> ny >> nz;
		normalsRAW.push_back(nx);
		normalsRAW.push_back(ny);
		normalsRAW.push_back(nz);
	      }
	      else if(type == FACE){

		pi.clear();
		ti.clear();
		ni.clear();
		GLuint positionIndex, texCoordIndex, normalIndex;
		char slash;

		while(!line.eof()){

		  // v v v
		  if(line.str().find('/') == string::npos){
		    line >> positionIndex;
		    pi.push_back(positionIndex-1);
		  }
		  // v//n v//n v//n
		  else if(line.str().find("//") != string::npos){

		    line >> positionIndex >> slash >> slash >> normalIndex;
		    pi.push_back(positionIndex-1);
		    ni.push_back(normalIndex-1);
		    }
		  else{
		    // v/t v/t v/t
		    line >> positionIndex >> slash >> texCoordIndex;
		    pi.push_back(positionIndex-1);
		    ti.push_back(texCoordIndex-1);
		    // v/t/n v/t/n v/t/n
		    if(line.peek() == '/'){
		      line >> slash >> normalIndex;
		      ni.push_back(normalIndex-1);
		    }
		  }
		} // line end
		triangulate(pi,ti,ni);
	      } // end face
	      else if(type == GROUP){

		string name;
		line >> name;
		Group group= {name, faces.size(), ""};
		groups.push_back(group);
	      } // end group
	      else if(type == MTL_GROUP){ // materials separrately specified

	      	line >> group;
	      }
	      else if(type == USE_MTL){

	      	string material;
	      	line >> material;
		if(group != "")
		   addMaterial(group, material);
		else groups.back().material= material;
	      }
	      else if(type == MTL_LIB){

	      	string dir, library;
		stringstream name(filename);
		getline(name, dir, '/');
	      	line >> library;
		name << dir << '/' << library;
		
	      	parseMaterials(name.str());
	      }
	      
	    } // eof
	    // end last (and possibly only) group
	    Group end= {"default", faces.size(), ""};
	    groups.push_back(end);

	    file.close();

	    cout << "loaded " << filename << ": " 
		 << positions.size() << " vertices, " << texCoordsRAW.size() << " texture coordinates, " << normalsRAW.size() << " normals, " 
		 << faces.size() << " faces, " << (groups.size()-2) << " groups, " << materials.size() << " materials" << endl;	    

	    // bring to format opengl eats
	    // this means possible duplication of normals 
	    // and / or texture coordinates
	    clean();
}

// parse a material library	
void OBJModel::parseMaterials(string filename){
	 
  const string NEW_MATERIAL= "newmtl";
  const string MATERIAL_AMBIENT= "Ka";
  const string MATERIAL_DIFFUSE= "Kd";
  const string MATERIAL_SPECULAR= "Ks";
  const string SPECULAR_EXPONENT= "Ns";

	    ifstream file;
	    file.open(filename.c_str());
	    
	      string l;

	    while(getline(file, l)){  

	      stringstream line(l);
	      string type;
	      line >> type;

	      if(type == NEW_MATERIAL){
		
		string name;
		line >> name;
		Material m;
		m.name= name;
		materials.push_back(m);
	      }
	      else if(type == MATERIAL_AMBIENT){

		float r, g, b;
		line >> r >> g >> b;
		vec3 ambient= vec3(r,g,b);
		materials.back().ambient= ambient;
	      }
	      else if(type == MATERIAL_DIFFUSE){

		float r, g, b;
		line >> r >> g >> b;
		vec3 diffuse= vec3(r,g,b);
		materials.back().diffuse= diffuse;
	      }
	      else if(type == MATERIAL_SPECULAR){

		float r, g, b;
		line >> r >> g >> b;
		vec3 specular= vec3(r,g,b);
		materials.back().specular= specular;
	      }
	      else if(type == SPECULAR_EXPONENT){

		float exponent;
		line >> exponent;
		materials.back().shininess= exponent;
	      }
	    } // eof
	    file.close();
}

void OBJModel::center(void){

  calculateBoundingBox();

  vec3 center= (boundingBoxMin + boundingBoxMax) * vec3(0.5);
  
  for(unsigned int i= 0; i<positions.size(); i+=3){  
    positions[i]-= center[0];
    positions[i+1]-= center[1];
    positions[i+2]-= center[2];
  }
}

void OBJModel::unitize(void){

  calculateBoundingSphere();

  for(unsigned int i= 0; i<positions.size(); ++i){
    positions[i]/= boundingSphereRadius;
  }
  boundingSphereRadius= 1;
}

void OBJModel::draw(void){

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &positions[0]);
  if(normals.size()>0){
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);
  }

  unsigned int begin, end;
  for(unsigned int g= 0; g+1<groups.size(); ++g){
    begin= groups[g].index;
    end= groups[g+1].index;
    
    if(materials.size() > 0){
      unsigned int material= materialIndex(groups[g].material);
      glMaterialfv(GL_FRONT, GL_AMBIENT, &materials[material].ambient[0]);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, &materials[material].diffuse[0]);
      glMaterialfv(GL_FRONT, GL_SPECULAR, &materials[material].specular[0]);
      glMaterialf(GL_FRONT, GL_SHININESS, materials[material].shininess);
    }
    glDrawElements(GL_TRIANGLES, end-begin, GL_UNSIGNED_INT, &faces[begin]);
    
  }

  // deactivate vertex arrays after drawing
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

  // triangulate a polygon
void OBJModel::triangulate(vector<GLuint> pi, vector<GLuint> ti, vector<GLuint> ni){

  for(unsigned int v= 2; v<pi.size(); v++){
    if(v == 2){
      faces.push_back(pi[v-2]);
      faces.push_back(pi[v-1]);
      faces.push_back(pi[v]);
    }
    else{
      faces.push_back(pi[v-3]);
      faces.push_back(pi[v-1]);
      faces.push_back(pi[v]);
    }
  }

  for(int v= 2; v<ti.size(); v++){
    if(v == 2){ // even/uneven?
      texCoordIndices.push_back(ti[v-2]);
      texCoordIndices.push_back(ti[v-1]);
      texCoordIndices.push_back(ti[v]);
    }
    else{
      texCoordIndices.push_back(ti[v-3]);
      texCoordIndices.push_back(ti[v-1]);
      texCoordIndices.push_back(ti[v]);
    }
  }

  for(unsigned int v= 2; v<ni.size(); v++){
    if(v == 2){ 
      normalIndices.push_back(ni[v-2]);
      normalIndices.push_back(ni[v-1]);
      normalIndices.push_back(ni[v]);
    }
    else{
      normalIndices.push_back(ni[v-3]);
      normalIndices.push_back(ni[v-1]);
      normalIndices.push_back(ni[v]);
    }
  }
}

  // bring to format opengl eats
	    // this means possible duplication of normals 
	    // and / or texture coordinates
void OBJModel::clean(void){
  
  normals.clear();
  if(normalIndices.size()>0) normals.resize(positions.size());
  for(unsigned int i= 0; i<normalIndices.size(); i++){    
    normals[3*faces[i]]= normalsRAW[3*normalIndices[i]];
    normals[3*faces[i]+1]= normalsRAW[3*normalIndices[i]+1];
    normals[3*faces[i]+2]= normalsRAW[3*normalIndices[i]+2];
  }
  normalsRAW.clear();
  normalIndices.clear();

  texCoords.clear();
  if(texCoordIndices.size()>0) normals.resize(positions.size());
  for(unsigned int i= 0; i<texCoordIndices.size(); i++){    
    texCoords[3*faces[i]]= texCoordsRAW[3*texCoordIndices[i]];
    texCoords[3*faces[i]+1]= texCoordsRAW[3*texCoordIndices[i]+1];
    texCoords[3*faces[i]+2]= texCoordsRAW[3*texCoordIndices[i]+2];
  }
  texCoordsRAW.clear();
  texCoordIndices.clear();
}

  // add material to group
void OBJModel::addMaterial(string group, string material){
  for(unsigned int i= 0; i<groups.size(); ++i){
    if(groups[i].name == group) groups[i].material= material;
  }
}
  // find material
unsigned int OBJModel::materialIndex(string name){
  for(unsigned int i= 0; i<materials.size(); ++i){
    if(materials[i].name == name) return i;
  }
  return 0;
}

  // calculate bounding sphere
void OBJModel::calculateBoundingSphere(void){
  boundingSphereRadius= 0;
  for(unsigned int i= 0; i<positions.size(); i+=3){
    vec3 v= vec3(positions.at(i), positions.at(i+1), positions.at(i+2));
    if(length(v) > boundingSphereRadius) boundingSphereRadius= length(v);
  }
}

  // calculate bounding box
void OBJModel::calculateBoundingBox(void){

  boundingBoxMin= vec3(numeric_limits<float>::max());
  boundingBoxMax= vec3(numeric_limits<float>::min());
  for(unsigned int i= 0; i<positions.size(); i+=3){
    if(positions[i] < boundingBoxMin[0]) boundingBoxMin[0]= positions[i];
    if(positions[i] > boundingBoxMax[0]) boundingBoxMax[0]= positions[i];
    if(positions[i+1] < boundingBoxMin[1]) boundingBoxMin[1]= positions[i+1];
    if(positions[i+1] > boundingBoxMax[1]) boundingBoxMax[1]= positions[i+1];
    if(positions[i+2] < boundingBoxMin[2]) boundingBoxMin[2]= positions[i+2];
    if(positions[i+2] > boundingBoxMax[2]) boundingBoxMax[2]= positions[i+2];
  }
}

  // calculate smooth normals
void OBJModel::calculateNormals(void){

  if(normals.size() > 0) return; 
  normals.resize(positions.size());
  for(int f= 0; f<faces.size(); f+=3){

    // face vertices
    vec3 v0= vec3(positions[3*faces[f]], positions[3*faces[f]+1], positions[3*faces[f]+2]);
    vec3 v1= vec3(positions[3*faces[f+1]], positions[3*faces[f+1]+1], positions[3*faces[f+1]+2]);
    vec3 v2= vec3(positions[3*faces[f+2]], positions[3*faces[f+2]+1], positions[3*faces[f+2]+2]);

    // edges
    vec3 edge0= normalize(v0 - v1);
    vec3 edge1= normalize(v1 - v2);
    vec3 edge2= normalize(v2 - v0);

    // squared lengths of edges
    float l0= dot(edge0, edge0);
    float l1= dot(edge1, edge1);
    float l2= dot(edge2, edge2);

    // normal
    vec3 n= normalize(cross(edge0, edge1));

    // weighting factors
    float f0= 1 / (l0*l2);
    float f1= 1 / (l1*l0);
    float f2= 1 / (l2*l1);

    // push out on vertices
    normals[3*faces[f]]+= n[0] * f0;
    normals[3*faces[f]+1]+= n[1] * f0;
    normals[3*faces[f]+2]+= n[2] * f0;

    normals[3*faces[f+1]]+= n[0] * f1;
    normals[3*faces[f+1]+1]+= n[1] * f1;
    normals[3*faces[f+1]+2]+= n[2] * f1;
    
    normals[3*faces[f+2]]+= n[0] * f2;
    normals[3*faces[f+2]+1]+= n[1] * f2;
    normals[3*faces[f+2]+2]+= n[2] * f2;

  }

  //normalize
  for(int i= 0; i< normals.size(); i+=3){
    vec3 n= vec3(normals[i], normals[i+1], normals[i+2]);
    n= normalize(n);
    normals[i]= n[0];
    normals[i+1]= n[1];
    normals[i+2]= n[2];
  }
}
