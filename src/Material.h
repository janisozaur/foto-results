#ifndef MATERIAL_H
#define MATERIAL_H
#include "LightIntensity.h"
#include "Texture.h"

/**
  Enumeration of material types
  */
enum MaterialType {
    DIFFUSE,
    REFLECTIVE,
    REFRACTIVE
};

/**
  Base class of material.
  By defual all materials can have texture
  */
class Material
{
public:
    /**
      Construct empty material without texture
      */
    Material();
    /**
      Construct empty material with texture t
      */
    Material(Texture*t);

    MaterialType type;

    /**
      Sets texture to material
      */
    void SetTexture(Texture*t);

    Texture* texture;
};


/**
  Diffuse material class.
  Contains ambient, diffuse, specular colors and specular coefficient and absorbtion propability.
  */
class DiffuseMaterial : public Material {
public:
    /**
      Constructs diffuse material with passed properties without texture
      */
    DiffuseMaterial(const Color& ambient, const Color& diffuse,
                    const Color& specular, float specularCoeff,
                    float absorptionPropability);


    /**
      Construct diffuse material with passed properties and with texture
      */
    DiffuseMaterial(const Color& ambient, const Color& diffuse,
                    const Color& specular, float specularCoeff,
                    float absorptionPropability,
                    Texture* texture);

    Color ambient;
    Color diffuse;
    Color specular;
    float specularCoeff;
    float absorptionPropability;
};

/**
  Relfective material class
  */
class ReflectiveMaterial : public Material {
public:
    /**
      Create reflective material without texture
      */
    ReflectiveMaterial();

    /**
      Create reflective material with texture
      */
    ReflectiveMaterial(Texture*texture);
};


/**
  Refractive material class
  */
class RefractiveMaterial : public Material {
public:
    /**
      Creates refractive material with given ratio of light speed in given media.
      Material is created without texture
      */
    RefractiveMaterial(float etaRate);

    /**
      Creates refractive material with given ratio of light speed in given media with texture
      */
    RefractiveMaterial(float etaRate, Texture* texture);

    float etaRate;
};

#endif // MATERIAL_H
