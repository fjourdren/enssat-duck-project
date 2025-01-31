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
#include <mutex>

#include "Consts.h"

#include "PacketFoundFlag.h"
#include "FlagToSpawn.h"

/** constructeur */
Scene::Scene() {}

void Scene::init()
{
    /*//Paramètres de lecture du fichier
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
    }*/
    



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
    // Si il y a des flags en attente de spawn dans la scène (car le spawn dans la scène est obligatoirement effectué dans la boucle principale)
    this->_mutex.lock();
    for(FlagToSpawn* ds: this->_flagsToSpawn) {
        // si l'élément à spawn est un canard
        if(ds->_type == "d") {
            Duck* d = new Duck(ds->_sound);
            d->setPosition(ds->_position);
            d->setOrientation(ds->_rotation);
            d->setDraw(ds->_found);
            d->setSound(!ds->_found); // non de _found car on arrète de jouer le son lorsque l'élément est trouvé
            d->setId(ds->_id);
            d->setPosDuck(this->m_duck.size());

            this->m_duck.push_back(d);
        }

        delete ds; // supression du flagToSpawn car instancié avec new

        std::cout << "[Game] spawn d'un objet." << std::endl;
    }
    this->_flagsToSpawn.clear();
    this->_mutex.unlock();

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

                // on informe le serveur que on a trouvé un canard
                PacketFoundFlag p = PacketFoundFlag(this->_cs->getIdClient(), duck->getId());
                std::string content = p.constructString(DEFAULT_CHAR_DELIMITER);
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



/**
 * Fonction pour réinitialiser la caméra
 **/
void Scene::resetCam() {
    this->_mutex.lock();

    // réalisation d'une action en fonction du nouvel état
    this->m_Azimut = 20.0;
    this->m_Elevation = 20.0;
    this->m_Distance = 10.0;


    // initialiser les matrices
    this->m_Center = vec3::create();

    this->_mutex.unlock();
}



/**
 * Fonction pour ajouter d'un flag (objet à trouver) à spawn dans la boucle principale
 **/
void Scene::addFlagToSpawn(FlagToSpawn* ds) {
    this->_mutex.lock();
    this->_flagsToSpawn.push_back(ds);
    this->_mutex.unlock();
}


/** supprime tous les objets de cette scène */
Scene::~Scene()
{
    for(Duck *duck : m_duck) {
        delete duck;
    }

    // destruction de tous les éléments en attente de spawn
    for(FlagToSpawn *f : _flagsToSpawn) {
        delete f;
    }

    delete m_Ground;
    delete _cs; // destruction de ClientSocket qui est instancié par main mais contenu dans Scene
}





// Getter & Setter GameState
GameState Scene::getGameState() {
    GameState output;

    this->_mutex.lock();
    output = this->_state;
    this->_mutex.unlock();

    return output;
}

void Scene::setGameState(GameState newState) {
    this->_mutex.lock();
    this->_state = newState;
    this->_mutex.unlock();
}


// getter & setter m_duck
std::vector<Duck*> Scene::getDucks() {
    std::vector<Duck*> output;

    this->_mutex.lock();
    output = this->m_duck;
    this->_mutex.unlock();

    return output;
}

Duck* Scene::getDuckById(int duckId) {
    Duck* output = nullptr;

    this->_mutex.lock();
    for(Duck* duck: this->m_duck) {
        if(duck->getId() == duckId) {
            output = duck;
            break;
        }
    }
    this->_mutex.unlock();

    return output;
}


void Scene::clearDucks() {
    this->_mutex.lock();

    // supression de tous les canards créés avec new
    for(Duck *duck : m_duck) {
        delete duck;
    }

    this->m_duck.clear();
    this->_mutex.unlock();
}



// getter & setter client socket (_cs)
ClientSocket* Scene::getClientSocket() {
    ClientSocket* output;

    this->_mutex.lock();
    output = this->_cs;
    this->_mutex.unlock();

    return output;
}

void Scene::setClientSocket(ClientSocket* cs) {
    this->_mutex.lock();
    this->_cs = cs;
    this->_mutex.unlock();
}