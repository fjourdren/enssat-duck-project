#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <utils.h>

#include "Scene.h"

//include rajoutées
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Consts.h"

#include "PacketFoundFlag.h"

/** constructeur */
Scene::Scene() {}

void Scene::init()
{
    //Paramètres de lecture du fichier
    configFile = "configuration.txt";
    std::fstream file;
    std::string line;
    std::string token;

    //Paramètres du canard
    std::string soundDuck;
    int position[3];
    int orientation[3];
    int posVector = 0;

    //Ouverture du fichier
    file.open(configFile); 

    //Lecture du fichier
    while(getline(file, line)) {
        std::istringstream buffer(line);
        //Lecture ligne par ligne et affectation
        while(std::getline(buffer, token, ':')){
            std::getline(buffer, token, ':');
            soundDuck = token;

            std::getline(buffer, token, ':');
            position[0] = atoi(token.c_str());
            std::getline(buffer, token, ':');
            position[1] = atoi(token.c_str());
            std::getline(buffer, token, ':');
            position[2] = atoi(token.c_str());

            std::getline(buffer, token, ':');
            orientation[0] = atoi(token.c_str());
            std::getline(buffer, token, ':');
            orientation[2] = atoi(token.c_str());
            std::getline(buffer, token, ':');
            orientation[3] = atoi(token.c_str());
            
            posVector = posVector + 1;
        }

        // Affectation des paramètres au canard
        Duck* duck = new Duck(soundDuck);
        duck->setPosition(vec3::fromValues(position[0], position[1], position[2]));
        duck->setOrientation(vec3::fromValues(orientation[0], orientation[1], orientation[2]));
        duck->setPosDuck(posVector);
        duck->setDraw(false);
        duck->setSound(true);
        duck->setId(posVector);

        std::cout << "Ajout d'un canard" << std::endl;

        // Changement de case du vector
        m_duck.push_back(duck);
    }
    



    m_Ground = new Ground();

    // caractéristiques de la lampe
    m_Light = new Light();
    m_Light->setColor(500.0, 500.0, 500.0);
    m_Light->setPosition(0.0,  16.0,  13.0, 1.0);
    m_Light->setDirection(0.0, -1.0, -1.0, 0.0);
    m_Light->setAngles(30.0, 40.0);

    // couleur du fond : gris foncé
    glClearColor(0.4, 0.4, 0.4, 0.0);

    // activer le depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // initialiser les matrices
    m_MatP = mat4::create();
    m_MatV = mat4::create();
    m_MatVM = mat4::create();
    m_MatTMP = mat4::create();

    // gestion vue et souris
    m_Azimut    = 20.0;
    m_Elevation = 20.0;
    m_Distance  = 10.0;
    m_Center    = vec3::create();
    m_Clicked   = false;
}


/**
 * appelée quand la taille de la vue OpenGL change
 * @param width : largeur en nombre de pixels de la fenêtre
 * @param height : hauteur en nombre de pixels de la fenêtre
 */
void Scene::onSurfaceChanged(int width, int height)
{
    // met en place le viewport
    glViewport(0, 0, width, height);

    // matrice de projection (champ de vision)
    mat4::perspective(m_MatP, Utils::radians(25.0), (float)width / height, 0.1, 100.0);
}


/**
 * appelée quand on enfonce un bouton de la souris
 * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseDown(int btn, double x, double y)
{
    if (btn != GLFW_MOUSE_BUTTON_LEFT) return;
    m_Clicked = true;
    m_MousePrecX = x;
    m_MousePrecY = y;
}


/**
 * appelée quand on relache un bouton de la souris
 * @param btn : GLFW_MOUSE_BUTTON_LEFT pour le bouton gauche
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseUp(int btn, double x, double y)
{
    m_Clicked = false;
}


/**
 * appelée quand on bouge la souris
 * @param x coordonnée horizontale relative à la fenêtre
 * @param y coordonnée verticale relative à la fenêtre
 */
void Scene::onMouseMove(double x, double y)
{
    if (! m_Clicked) return;
    m_Azimut  += (x - m_MousePrecX) * 0.1;
    m_Elevation += (y - m_MousePrecY) * 0.1;
    if (m_Elevation >  90.0) m_Elevation =  90.0;
    if (m_Elevation < -90.0) m_Elevation = -90.0;
    m_MousePrecX = x;
    m_MousePrecY = y;
}


/**
 * appelée quand on appuie sur une touche du clavier
 * @param code : touche enfoncée
 */
void Scene::onKeyDown(unsigned char code)
{
    // construire la matrice inverse de l'orientation de la vue à la souris
    mat4::identity(m_MatTMP);
    mat4::rotateY(m_MatTMP, m_MatTMP, Utils::radians(-m_Azimut));
    mat4::rotateX(m_MatTMP, m_MatTMP, Utils::radians(-m_Elevation));

    // vecteur indiquant le décalage à appliquer au pivot de la rotation
    vec3 offset = vec3::create();
    switch (code) {
    case GLFW_KEY_W: // avant
//        m_Distance *= exp(-0.01);
        vec3::transformMat4(offset, vec3::fromValues(0, 0, +0.1), m_MatTMP);
        break;
    case GLFW_KEY_S: // arrière
//        m_Distance *= exp(+0.01);
        vec3::transformMat4(offset, vec3::fromValues(0, 0, -0.1), m_MatTMP);
        break;
    case GLFW_KEY_A: // droite
        vec3::transformMat4(offset, vec3::fromValues(+0.1, 0, 0), m_MatTMP);
        break;
    case GLFW_KEY_D: // gauche
        vec3::transformMat4(offset, vec3::fromValues(-0.1, 0, 0), m_MatTMP);
        break;
    case GLFW_KEY_Q: // haut
        vec3::transformMat4(offset, vec3::fromValues(0, -0.1, 0), m_MatTMP);
        break;
    case GLFW_KEY_Z: // bas
        vec3::transformMat4(offset, vec3::fromValues(0, +0.1, 0), m_MatTMP);
        break;
    default:
        return;
    }

    // appliquer le décalage au centre de la rotation
    vec3::add(m_Center, m_Center, offset);
}


/**
 * Dessine l'image courante
 */
void Scene::onDrawFrame()
{
    /** préparation des matrices **/

    // positionner la caméra
    mat4::identity(m_MatV);

    // éloignement de la scène
    mat4::translate(m_MatV, m_MatV, vec3::fromValues(0.0, 0.0, -m_Distance));

    // rotation demandée par la souris
    mat4::rotateX(m_MatV, m_MatV, Utils::radians(m_Elevation));
    mat4::rotateY(m_MatV, m_MatV, Utils::radians(m_Azimut));

    // centre des rotations
    mat4::translate(m_MatV, m_MatV, m_Center);


    mat4 tmp_v;
    vec4 pos;    

    for(Duck *duck : m_duck) {
        // si le canard n'a pas été trouvé
        if(duck->getDraw() == false) {
            mat4::translate(tmp_v, m_MatV, duck->getPosition());
            vec4::transformMat4(pos, vec4::fromValues(0,0,0,1), tmp_v);

            //std::cout << vec4::length(pos) << duck->getDraw() << std::endl; (debug, voir la distance par rapport à un canard)
            
            // si le joueur est à < 5 d'un canard => le joueur trouve le canard
            if(vec4::length(pos) < 5) {
                duck->setDraw(true);

                std::string posDuck = std::to_string(duck->getPosDuck());
                std::cout<< "Requête canard " + std::to_string(duck->getPosDuck()) +" trouvé envoyé." << std::endl;

                // on informe le serveur que on a trouvé un canard
                PacketFoundFlag* p = new PacketFoundFlag(this->_cs->getIdClient(), duck->getId());
                std::string content = p->constructString(DEFAULT_CHAR_DELIMITER);
                this->_cs->send(content);
            }
        }
    }

    /** gestion des lampes **/

    // calculer la position et la direction de la lampe par rapport à la scène
    m_Light->transform(m_MatV);

    // fournir position et direction en coordonnées caméra aux objets éclairés
    m_Ground->setLight(m_Light);
    for(Duck *duck : m_duck) {
        duck->setLight(m_Light);
    }



    /** dessin de l'image **/

    // effacer l'écran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // dessiner le sol
    m_Ground->onDraw(m_MatP, m_MatV);

    // dessiner le canard en mouvement
    for(Duck *duck : m_duck) {
        duck->onRender(m_MatP, m_MatV);
    }

}


/** supprime tous les objets de cette scène */
Scene::~Scene()
{
    for(Duck *duck : m_duck) {
            delete duck;
    }
    delete m_Ground;
}



// Getter & Setter GameState
GameState Scene::getGameState() {
    return this->_state;
}

void Scene::setGameState(GameState newState) {
    this->_state = newState;
}


// getter & setter m_duck
std::vector<Duck*> Scene::getDucks() {
    return this->m_duck;
}

Duck* Scene::getDuckById(int duckId) {
    Duck* output = nullptr;

    for(Duck* duck: this->m_duck) {
        if(duck->getId() == duckId) {
            output = duck;
            break;
        }
    }

    return output;
}

// Ajout du'un canard dans la liste
void Scene::addDuck(Duck* d) {
    this->m_duck.push_back(d);
}

void Scene::clearDucks() {
    this->m_duck.clear();
}



// getter & setter client socket (_cs)
ClientSocket* Scene::getClientSocket() {
    return this->_cs;
}

void Scene::setClientSocket(ClientSocket* cs) {
    this->_cs = cs;
}