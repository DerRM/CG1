/* ----------------------------------------------------------------
   name:           objmodel.h
   purpose:        class for loading and drawing wavefront .obj files
   version:	   LIBRARY CODE
   TODO:           nothing
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include <string>
#include <vector>

#include "glm/glm.hpp"

#pragma once

class OBJModel{

 public:

  OBJModel(void);
  OBJModel(std::string filename);

  // load a model from a file	
  void load(std::string filename);
  // draw model
  void draw(void);
  // normalize to bounding sphere radius 1
  void unitize(void);
  // center model
  void center(void);
  // calculate bounding sphere
  void calculateBoundingSphere(void);
  // calculate bounding box
  void calculateBoundingBox(void);
  // calculate smooth normals
  void calculateNormals(void);

 protected:

    // triangulate a polygon
  void triangulate(std::vector<GLuint> positionIndices, std::vector<GLuint> texCoordIndices, std::vector<GLuint> normalIndices);
  // bring to format opengl eats
  // (no shared normals / texture coordinates)
  void clean(void);
  // parse a material library	
  void parseMaterials(std::string filename);
  // find material
  unsigned int materialIndex(std::string name);
  // add material to group
  void addMaterial(std::string group, std::string material);

  typedef struct group{
    std::string name;
    unsigned int index;
    std::string material;
  } Group;
  
  typedef struct material{
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
  } Material;

  float boundingSphereRadius;
  // two opposite corners of bounding box 
  glm::vec3 boundingBoxMin;
  glm::vec3 boundingBoxMax;

  std::vector<float> positions;
  std::vector<float> texCoords, texCoordsRAW;
  std::vector<float> normals, normalsRAW;
  std::vector<glm::vec3> faceNormals;
  std::vector<GLuint> faces;
  std::vector<GLuint> texCoordIndices;
  std::vector<GLuint> normalIndices;
  std::vector<Group> groups;
  std::vector<Material> materials;
};
