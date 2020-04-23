#ifndef DUCK_H
#define DUCK_H

// Définition de la classe Duck

#include <Mesh.h>
#include <Light.h>
#include <MaterialTexture.h>
#include <gl-matrix.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

class Duck: public Mesh
{
private:

    /** matériau */
    MaterialTexture* m_Material;


    /** buffers pour la gestion du son */
    ALuint buffer, source;

    /** position 3D du cube */
    vec3 m_Position;

    /** position 3D du cube */
    vec3 m_Orientation;

    /** position du canard dans le vector*/
    int posDuck;

    bool m_Draw = false;
    bool m_Sound;


    int _duckId; // ID du canard pour le serveur

public:

    /** constructeur, crée le maillage */
    Duck();
    //Constructeur avec le changement de son du canard
    Duck(std::string sound);        

    /** destructeur, libère le maillage et l'audio */
    ~Duck();

    /**
     * dessiner le canard
     * @param matP : matrice de projection
     * @param matMV : matrice view*model (caméra * position objet)
     */
    void onRender(const mat4& matP, const mat4& matMV);

    /**
     * retourne la position % scèce du cube
     * @return vec3 position
     */
    vec3& getPosition();

    /**
     * affecte la position % scène du cube
     * @param vec3 pos position
     */
    void setPosition(vec3 pos);

    /**
     * retourne la position % scèce du cube
     * @return vec3 position
     */
    vec3& getOrientation();

    /**
     * affecte la position % scène du cube
     * @param vec3 pos position
     */
    void setOrientation(vec3 ori);

    /**
     * Récupérer la position dans le vector du canard
    */
    int getPosDuck();

    /**
     * Affecte la position dans le vector du canard
    */
    void setPosDuck(int );

    /**
     * modifie la propriete d'affichage
=     */
    void setDraw(bool b);


    /**
     * récupère la valeur de m_draw
     */
    bool getDraw();

    /**
     * modifie la propriete de son
=     */
    void setSound(bool b);

    /**
     * définit la lampe
     * @param light : instance de Light spécifiant les caractéristiques de la lampe
     */
    void setLight(Light* light);


    /**
     * Récupérer l'id du canard côté serveur
    */
    int getId();

    /**
     * Affecte un id au canard
    */
    void setId(int newid);
};

#endif
