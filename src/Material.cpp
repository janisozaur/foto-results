#include "Material.h"
#include <QDebug>

Material::Material() {
    texture=0;
}

Material::Material(Texture *t) {
    texture = t;
}

void Material::SetTexture(Texture *t) {
    texture = t;
}

DiffuseMaterial::DiffuseMaterial(const Color &ambient, const Color &diffuse,
                                 const Color &specular, float specularCoeff,
                                 float absorptionPropability) : Material() {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->specularCoeff = specularCoeff;
    this->absorptionPropability = absorptionPropability;

    type = DIFFUSE;
}

DiffuseMaterial::DiffuseMaterial(const Color &ambient, const Color &diffuse,
                                 const Color &specular, float specularCoeff,
                                 float absorptionPropability, Texture *texture) : Material(texture) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->specularCoeff = specularCoeff;
    this->absorptionPropability = absorptionPropability;

    type = DIFFUSE;
}


ReflectiveMaterial::ReflectiveMaterial() : Material() {
    type = REFLECTIVE;
}

ReflectiveMaterial::ReflectiveMaterial(Texture *texture) : Material(texture) {
    type = REFLECTIVE;
}

RefractiveMaterial::RefractiveMaterial(float etaRate) : Material() {
    this->etaRate = etaRate;
    type = REFRACTIVE;
}

RefractiveMaterial::RefractiveMaterial(float etaRate, Texture *texture) : Material(texture) {
    this->etaRate = etaRate;
    type = REFRACTIVE;
}
