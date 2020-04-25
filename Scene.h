#ifndef SCENE_H
#define SCENE_H

// Définition de la classe Scene

#include <gl-matrix.h>

#include "Light.h"

#include "Duck.h"
#include "Ground.h"

#include "Consts.h"

class ClientSocket;

class Scene
{
private:

    // objets de la scène
    std::string configFile;
    std::vector<Duck*> m_duck;
    Ground* m_Ground;

    // lampes
    Light* m_Light;

    // matrices de transformation des objets de la scène
    mat4 m_MatP;
    mat4 m_MatV;
    mat4 m_MatVM;
    mat4 m_MatTMP;
    

    // caméra table tournante
    

    // souris
    bool m_Clicked;
    double m_MousePrecX;
    double m_MousePrecY;

    GameState _state = UNDEFINED;
    ClientSocket* _cs;


public:
    // caméra table tournante
    float m_Azimut;
    float m_Elevation;
    float m_Distance;

    // matrices de transformation des objets de la scène (déplacement)
    vec3 m_Center;
    

    /** constructeur */
    Scene();

    /** destructeur, libère les ressources */
    ~Scene();

    /**
    Réalise toutes les opérations d'init de l'objet + crée les objets 3D à dessiner
    */
    void init(); // on créer cette classe pour initialiser le constructeur avant (sans avoir les includes) et ainsi pouvoir utiliser les pointeurs vers Scene

    /**
     * appelée quand la taille de la vue OpenGL change
     * @param width : largeur en nombre de pixels de la fenêtre
     * @param height : hauteur en nombre de pixels de la fenêtre
     */
    void onSurfaceChanged(int width, int height);


    /**
     * appelée quand on enfonce un bouton de la souris
     * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
     * @param x coordonnée horizontale relative à la fenêtre
     * @param y coordonnée verticale relative à la fenêtre
     */
    void onMouseDown(int btn, double x, double y);

    /**
     * appelée quand on relache un bouton de la souris
     * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
     * @param x coordonnée horizontale relative à la fenêtre
     * @param y coordonnée verticale relative à la fenêtre
     */
    void onMouseUp(int btn, double x, double y);

    /**
     * appelée quand on bouge la souris
     * @param x coordonnée horizontale relative à la fenêtre
     * @param y coordonnée verticale relative à la fenêtre
     */
    void onMouseMove(double x, double y);

    /**
     * appelée quand on appuie sur une touche du clavier
     * @param code : touche enfoncée
     */
    void onKeyDown(unsigned char code);

    /** Dessine l'image courante */
    void onDrawFrame();

    // getter & setter GameState
    GameState getGameState();
    void setGameState(GameState newState);

    // getter & setter m_duck
    std::vector<Duck*> getDucks();
    Duck* getDuckById(int duckId);
    void addDuck(Duck* d);
    void clearDucks();

    // getter & setter client socket (_cs)
    ClientSocket* getClientSocket();
    void setClientSocket(ClientSocket* cs);
};

#endif
