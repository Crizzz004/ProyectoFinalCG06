﻿/*
Proyecto aviario
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <iostream>

#include <glew.h>
#include <glfw3.h>
#include <random>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include <irrKlang.h>
#include <thread>

#include <chrono>			

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

//para probar el importer
//#include<assimp/Importer.hpp>

#include "Mesh.h"
#include "Shader_light.h"
#include "Toroide.h" //Toroide 
#include "Toroide2.h" //Toroide 2


#include "Window.h"
#include "Camera.h"

#include "Texture.h"

#include"Model.h"
#include "Skybox.h"


//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"



const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.1416f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;



Camera camera, cameraAvatar, cameraAerea;


/*TEXTURAS*/
//--------------------------------------------------------------------------------------*//
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

/*DECLARACI�N DE MODELOS*/
//--------------------------------------------------------------------------------------*//
// Modelos Daniela //
Model Llanta_M;
Model Lampara;
Model Puerta_izquierda;
Model Puerta_derecha;
Model Arco_piedra;
Model Cartel;
Model Elemento;
Model AutoB_base;		// base del auto con textura
Model AutoB_cofre_tex;	// cofre con textura
Model AutoB_LLTI;		// Llanta 1
Model AutoB_LLTD;		// Llanta 3
Model Nadder_Cuerpo;
Model Nadder_AlaI;
Model Nadder_AlaD;

Model Nadder_MusloD;
Model Nadder_MusloI;
Model Nadder_cabeza;
Model Nadder_mandibula;
Model Nadder_PataD;
Model Nadder_PataI;
Model Nadder_cola1;
Model Nadder_cola2;
Model Nadder_cola3;


Model Coche_coco;
Model Llanta_coco;

//Skrill
Model Skrill_cuerpo;
Model Skrill_AlaD;
Model Skrill_AlaI;
Model Skrill_cola;

// Modelos Karla //
Model Jaula_A;
Model Domo;
Model DomoA;
Model Jaula_B;
Model Estatua_A;
Model Lampara_Spot; //Lampara spotlight para la estatua.
Model Keroppi_estatua_M;
Model Gazebo_M;
Model Cartel_M;
Model Arbusto_M;
Model Stand_M;
Model Bocina_M;
Model PuertaP_M;
Model Plataforma_M;
Model Cerca01_M;
Model Cerca02_M;
Model Cerca03_M;
Model Basura_M;
Model Senal_M;
Model PopCorn_M;
Model Cartel2_M;
Model Fruta;
Model Palmera;
Model Penguin;
Model Ice;
Model Lamp;

bool ciclo;
float valorCiclo;
float cicloOffset;

/*Partes Kuromi*/
Model Cabeza;
Model PataIzq;
Model PataDer;
Model Varita;
Model BrazoIzq;
Model BrazoDer;
Model Cuerpo;

// Modelos Eduardo Miguel //
Model dodrio;
Model rowlet;
Model banderaPokemon;
Model lamparaLalo;
Model expendedora;
Model paredPuertaLalo;
Model puertaLalo;
Model Eeve;
Model Plant02;
Model Sudowoodo;
Model Zubat;
Model Vigilancia;

//Partes Psybuck
Model psyduckBase;
Model psyduckPieI;
Model psyduckPieD;

//Partes carro de golf
Model carroGolfBase;
Model carroGolfLlanta;


Model Meta;

// Modelos Cristopher //
Model banca;
Model Bandera_K;
Model Bandera_C;
Model Bandera_H;
Model Bandera_P;
Model box;
Model metalUp;
Model metal;
Model fruit;
Model normal;
Model chicken;
Model car;
Model stork;
Model vulture;
Model kart;
Model kartllanta;
Model avionCrash;
float movHeliy;
float movAvion;
float movHeliyoffset;
float movAvionoffset;
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;

//--------------------------------------------------------------------------------------*//
///Bresenham 
int screenWidth;
int screenHeight;
float centerX;
float centerY;
float radius;

//Sol
float movSol;
float movSolOffset;
int contSol;
float green;
float red;

Skybox skybox;


/*DECLARACION DE MATERIALES*/
//--------------------------------------------------------------------------------------*//
Material Material_brillante;
Material Material_opaco;
Material Material_kuromi;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

/*DECLARACI�N DE LUCES*/
//--------------------------------------------------------------------------------------*//
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];
PointLight pointLights3[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];
SpotLight spotLights3[MAX_SPOT_LIGHTS];
SpotLight spotLights4[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//Toroide Cristpher
Toroide ta = Toroide(3.0, 1000, 1000); //recibe radio, slices, stacks

//Toroide  Karla 
Toroide2 tb = Toroide2(1.0, 3.0, 1000, 1000); //recibe radio menor, radio mayor, slices, stacks.



//funci�n de calculo de normales por promedio de v�rtices
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {		//Desde donde lo voy a ilumnar
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	-1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		-1.0f, 1.0f,		0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		1.0f, -1.0f, -1.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);	//calcula normal por promedio de vertives

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void playMusic()
{
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		printf("error starting up the engine \n"); 

	engine->play2D("audio/Audio_C.ogg", true);

}

void playMusic3D() {
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		printf("error starting up the engine \n");


	ISound* music = engine->play3D("audio/ophelia.ogg",
		vec3df(0, 0, 0), true, false, true);


	if (music)
		music->setMinDistance(5.0f);	
}

void playSonido() {
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		printf("Could not startup engine\n");

	engine->play2D("audio/magic.wav");

}

void playPasos() {
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		printf("Could not startup engine\n");

	engine->play2D("audio/bell.wav",false);

	while (engine->isCurrentlyPlaying("audio/pasos.wav")) {
		// Sleep for a short time to allow the sound to play
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	

}

///////////////////////////////////////////////////////////////////////////////

void drawCircleBresenham(int radio, int centroX, int centroY) {
	int x = 0;
	int y = radio;
	int d = 3 - 2 * radio;

	// Algoritmo de Bresenham para dibujar la circunferencia
	while (x <= y) {
		glBegin(GL_POINTS);
		// Dibujar los puntos simétricos alrededor del centro
		glVertex2i(centroX + x, centroY + y);
		glVertex2i(centroX - x, centroY + y);
		glVertex2i(centroX + x, centroY - y);
		glVertex2i(centroX - x, centroY - y);
		glVertex2i(centroX + y, centroY + x);
		glVertex2i(centroX - y, centroY + x);
		glVertex2i(centroX + y, centroY - x);
		glVertex2i(centroX - y, centroY - x);
		glEnd();

		// Actualizar el valor del parámetro de decisión y las coordenadas x e y
		if (d < 0) {
			d += 4 * x + 6;
		}
		else {
			d += 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
}
///////////////////////////////////////////////////////////////////////////////

const float pi = 3.14159265358979323846;
const int numSegments = 100;
const float innerRadius = 1.0;
const float outerRadius = 3.0;

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();


	playMusic();
	CreateObjects();
	CreateShaders();

	cameraAerea = Camera(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, 0.6f, 0.5f);

	//Inicializa los toroides
	ta.init();
	tb.init();

	//Envía los toroides al shader
	ta.load();
	tb.load();
	//Variables de luz temporizada
	int contador = 0;															////////////////////////////////////////////////////////////////
	auto inicio = std::chrono::steady_clock::now();
	auto tiempo_transcurrido = std::chrono::steady_clock::now() - inicio;		////////////////////////////////////////////////////////////////



	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//--------------------------------------------------------------------------------------*//

	/*TEXTURAS*/
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/plano_aviario.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	//--------------------------------------------------------------------------------------*//

	//--------------------------------------------------------------------------------------*//

	/*MODELOS DANIELA*/
	expendedora = Model();
	expendedora.LoadModel("Models/Daniela/Maquina_expendedora.obj");
	Lamp = Model();
	Lamp.LoadModel("Models/Daniela/lampara_2.obj");

	//Nadder
	Nadder_Cuerpo = Model();
	Nadder_Cuerpo.LoadModel("Models/Daniela/Nadder_cuerpo_tex.obj");
	Nadder_AlaD = Model();
	Nadder_AlaD.LoadModel("Models/Daniela/Nadder_ala_der.obj");
	Nadder_AlaI = Model();		// Llanta trasera izquierda
	Nadder_AlaI.LoadModel("Models/Daniela/Nadder_ala_izq.obj");
	Nadder_MusloD = Model();		// Llanta trasera derecha
	Nadder_MusloD.LoadModel("Models/Daniela/Nadder_muslo_der.obj");
	Nadder_MusloI = Model();		// Llanta trasera derecha
	Nadder_MusloI.LoadModel("Models/Daniela/Nadder_muslo_izq.obj");
	Nadder_cabeza = Model();		// Llanta trasera derecha
	Nadder_cabeza.LoadModel("Models/Daniela/Nadder_cabeza_tex.obj");
	Nadder_mandibula = Model();		// Llanta trasera derecha
	Nadder_mandibula.LoadModel("Models/Daniela/Nadder_mandibula.obj");
	Nadder_PataD = Model();		// Llanta trasera derecha
	Nadder_PataD.LoadModel("Models/Daniela/Nadder_pata_izq.obj");
	Nadder_PataI = Model();		// Llanta trasera derecha
	Nadder_PataI.LoadModel("Models/Daniela/Nadder_pata_der.obj");
	Nadder_cola1 = Model();		// Llanta trasera derecha
	Nadder_cola1.LoadModel("Models/Daniela/Nadder_cola1.obj");
	Nadder_cola2 = Model();		// Llanta trasera derecha
	Nadder_cola2.LoadModel("Models/Daniela/Nadder_cola2.obj");
	Nadder_cola3 = Model();		// Llanta trasera derecha
	Nadder_cola3.LoadModel("Models/Daniela/Nadder_cola3.obj");


	//Skrill
	Skrill_cuerpo = Model();
	Skrill_cuerpo.LoadModel("Models/Daniela/Skrill_cuerpo.obj");
	Skrill_AlaD = Model();
	Skrill_AlaD.LoadModel("Models/Daniela/Skrill_alaD.obj");
	Skrill_AlaI = Model();		// Llanta trasera izquierda
	Skrill_AlaI.LoadModel("Models/Daniela/Skrill_alaI.obj");
	Skrill_cola = Model();		// Llanta trasera derecha
	Skrill_cola.LoadModel("Models/Daniela/Skrill_cola.obj");

	//Coche 01
	Coche_coco = Model();
	Coche_coco.LoadModel("Models/Daniela/kart_crash_coco.obj");

	Llanta_coco = Model();
	Llanta_coco.LoadModel("Models/Daniela/llanta_optimizada.obj");

	//--------------------------------------------------------------------------------------*//

	/*MODELOS KARLA*/
	Jaula_A = Model();
	Jaula_A.LoadModel("Models/Karla/JaulaA_M.obj");
	Domo = Model();
	Domo.LoadModel("Models/Karla/Domo_M.obj");
	DomoA = Model();
	DomoA.LoadModel("Models/Karla/DomoB_M.obj");
	Estatua_A = Model();
	Estatua_A.LoadModel("Models/Karla/Kuromi_estatua_M.obj");
	Lampara_Spot = Model();
	Lampara_Spot.LoadModel("Models/Karla/LamparaS_M.obj");
	Keroppi_estatua_M = Model();
	Keroppi_estatua_M.LoadModel("Models/Karla/Keroppi_estatua_M.obj");
	Gazebo_M = Model();
	Gazebo_M.LoadModel("Models/Karla/Gazebo_M.obj");
	Cartel_M = Model();
	Cartel_M.LoadModel("Models/Karla/Cartel_M.obj");
	Arbusto_M = Model();
	Arbusto_M.LoadModel("Models/Karla/Arbusto_M.obj");
	Stand_M = Model();
	Stand_M.LoadModel("Models/Karla/Stand_M.obj");
	Bocina_M = Model();
	Bocina_M.LoadModel("Models/Karla/Bocina_M.obj");
	PuertaP_M = Model();
	PuertaP_M.LoadModel("Models/Karla/PuertaP_M.obj");
	Plataforma_M = Model();
	Plataforma_M.LoadModel("Models/Karla/Auditorio_M.obj");
	Cerca01_M = Model();
	Cerca01_M.LoadModel("Models/Karla/Cerca01_M.obj");
	Cerca02_M = Model();
	Cerca02_M.LoadModel("Models/Karla/Cerca02_M.obj");
	Cerca03_M = Model();
	Cerca03_M.LoadModel("Models/Karla/Cerca03_M.obj");
	Senal_M = Model();
	Senal_M.LoadModel("Models/Karla/Senal_M.obj");
	Basura_M = Model();
	Basura_M.LoadModel("Models/Karla/Basura_M.obj");
	PopCorn_M = Model();
	PopCorn_M.LoadModel("Models/Karla/PopCorn.obj");
	Cartel2_M = Model();
	Cartel2_M.LoadModel("Models/Karla/Cartel2_M.obj");
	//KUROMI
	Cabeza = Model();
	Cabeza.LoadModel("Models/Karla/Kuromi/Cabeza_M.obj");
	Cuerpo = Model();
	Cuerpo.LoadModel("Models/Karla/Kuromi/Cuerpo_M.obj");
	PataIzq = Model();
	PataIzq.LoadModel("Models/Karla/Kuromi/PataIzq_M.obj");
	PataDer = Model();
	PataDer.LoadModel("Models/Karla/Kuromi/PataDer_M.obj");
	BrazoDer = Model();
	BrazoDer.LoadModel("Models/Karla/Kuromi/BrazoDer_M.obj");
	BrazoIzq = Model();
	BrazoIzq.LoadModel("Models/Karla/Kuromi/BrazoIzq_M.obj");
	Varita = Model();
	Varita.LoadModel("Models/Karla/Kuromi/Varita_M.obj");
	Palmera = Model();
	Palmera.LoadModel("Models/Karla/Palmera_M.obj");
	Fruta = Model();
	Fruta.LoadModel("Models/Karla/Wumpa_M.obj");
	Penguin = Model();
	Penguin.LoadModel("Models/Karla/Tuxedo.obj");
	Ice = Model();
	Ice.LoadModel("Models/Karla/Hielo_M.obj");
	//--------------------------------------------------------------------------------------*//


	//--------------------------------------------------------------------------------------*//
	/*MODELOS EDUARDO*/
	dodrio = Model();
	dodrio.LoadModel("Models/Eduardo/dodrio.obj");
	rowlet = Model();
	rowlet.LoadModel("Models/Eduardo/rowlet.obj");
	paredPuertaLalo = Model();
	paredPuertaLalo.LoadModel("Models/Eduardo/PAREDES.obj");
	puertaLalo = Model();
	puertaLalo.LoadModel("Models/Eduardo/Puerta.obj");
	Eeve = Model();
	Eeve.LoadModel("Models/Eduardo/Eeve.obj");
	Plant02 = Model();
	Plant02.LoadModel("Models/Eduardo/Plant02.obj");
	Sudowoodo = Model();
	Sudowoodo.LoadModel("Models/Eduardo/Sudowoodo.obj");
	Zubat = Model();
	Zubat.LoadModel("Models/Eduardo/Zubat.obj");
	Vigilancia = Model();
	Vigilancia.LoadModel("Models/Eduardo/cabina.obj");

	//Partes Psybuck
	psyduckBase = Model();
	psyduckBase.LoadModel("Models/Eduardo/Psyduck_cuerpo.obj");
	psyduckPieI = Model();
	psyduckPieI.LoadModel("Models/Eduardo/Psyduck_pie_izquierdo.obj");
	psyduckPieD = Model();
	psyduckPieD.LoadModel("Models/Eduardo/Psyduck_pie_derecho.obj");

	//Partes carro de golf
	carroGolfBase = Model();
	carroGolfBase.LoadModel("Models/Eduardo/Carro_golf_base.obj");

	carroGolfLlanta = Model();
	carroGolfLlanta.LoadModel("Models/Eduardo/Carro_golf_llanta.obj");


	//--------------------------------------------------------------------------------------*//

	/*MODELOS CRISTOPHER*/
	banca = Model();
	banca.LoadModel("Models/Christopher/banca.obj");
	Bandera_C = Model();
	Bandera_C.LoadModel("Models/Christopher/crash.obj");
	Bandera_K = Model();
	Bandera_K.LoadModel("Models/Christopher/kuromi.obj");
	Bandera_H = Model();
	Bandera_H.LoadModel("Models/Christopher/dragon.obj");
	Bandera_P = Model();
	Bandera_P.LoadModel("Models/Christopher/pokemon.obj");
	box = Model();
	box.LoadModel("Models/Christopher/ModelsCrash/box.obj");
	metalUp = Model();
	metalUp.LoadModel("Models/Christopher/ModelsCrash/metalUp.obj");
	fruit = Model();
	fruit.LoadModel("Models/Christopher/ModelsCrash/fruit.obj");
	normal = Model();
	normal.LoadModel("Models/Christopher/ModelsCrash/normal.obj");
	chicken = Model();
	chicken.LoadModel("Models/Christopher/ModelsCrash/chicken.obj");
	car = Model();
	car.LoadModel("Models/Christopher/ModelsCrash/CrashStangFEShape.obj");
	stork = Model();
	stork.LoadModel("Models/Christopher/ModelsCrash/Stork.obj");
	metal = Model();
	metal.LoadModel("Models/Christopher/ModelsCrash/metal.obj");
	vulture = Model();
	vulture.LoadModel("Models/Christopher/ModelsCrash/ave.obj");
	kart = Model();
	kart.LoadModel("Models/Christopher/ModelsCrash/base_kart.obj");
	kartllanta = Model();
	kartllanta.LoadModel("Models/Christopher/ModelsCrash/rueda_kart.obj");
	avionCrash = Model();
	avionCrash.LoadModel("Models/Christopher/ModelsCrash/avion_crash.obj");

	//--------------------------------------------------------------------------------------*//

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_up.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	Material_kuromi = Material(0.0f,0);
	//--------------------------------------------------------------------------------------*//




									/*LUCES*/
	//--------------------------------------------------------------------------------------*//
		//luz direccional, s�lo 1 y siempre debe de existir
	/*mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);		// a donde apunta

	 //luz direccional, s lo 1 y siempre debe de existir
	mainLight = DirectionalLight(R, G, B,
		(G / 2) + 0.1f, (G / 2) + 0.1f,
		movSol, -300.0f, 0.0f);		// a donde apunta */

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int pointLightCount2 = 0;
	unsigned int pointLightCount3 = 0;

	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 1.0f,
		1.0f, 10.0f,
		-55.0f, 30.0f, 110.0f,
		0.1f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 0.0f, 1.0f,		
		5.0f, 10.0f,										
		55.0f, 30.0f, 110.0f,								
		0.1f, 0.2f, 0.1f);								
	pointLightCount++;

	pointLights2[0] = PointLight(1.0f, 0.0f, 0.0f,
		10.0f, 30.0f,
		10.0, 0.0f, -100.0f,
		0.1f, 0.2f, 0.1f);
	pointLightCount2++;

	unsigned int spotLightCount = 0;
	unsigned int spotLightCount3 = 0;

	//linterna principal
	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,		// color de la luz // R,G,B
		0.0f, 2.0f,									// ambIntensidad, diffIntensidad
		0.0f, 0.0f, 0.0f,								// Posicion // X,Y,Z
		0.0f, -1.0f, 0.0f,								// direccion
		1.0f, 0.0f, 0.0f,								// con, lin, exp
		5.0f);											// angulo de apertura del cono de la linterna
	spotLightCount++;

	//Luz para habitat kuromi
	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
		1.0f, 5.0f,
		110.0f, -1.0f, -90.0f,
		0.0f, 0.1f, 0.0f,
		0.5f, 0.1f, 0.001f,
		50.0f);
	spotLightCount++;

	//Luz del faro para la estatua de keroppi 1
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 5.0f,
		15.0f, 2.0f, 85.0f,
		0.0f, 1.0f, 1.0f,
		0.5f, 0.1f, 0.001f,
		50.0f);
	spotLightCount++;

	//Luz del faro para la estatua de keroppi 2
	spotLights[3] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 5.0f,
		-15.0f, 2.0f, 85.0f,
		0.0f, 1.0f, 1.0f,
		0.5f, 0.1f, 0.001f,
		50.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana

	movHeliyoffset = 12.0f;
	movAvionoffset = 1.0f;

	movCoche = 0.0f;
	movOffset = 0.1f;
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;
	avanza = true;

	/////LUZ SOL
	movSol = 1000.0f;
	movSolOffset = 0.1f;
	contSol = 1;
	green = 0.0f;
	red = 0.0f;

	valorCiclo = 0.0f;
	cicloOffset = 1.0f;

	float posGolf = 0.0f, posChoco = 0.0f;

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime para estandarizar el tiempo trascurrido entre ciclos de reloj y las variables
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		/*
		//CICLO AUTOMATICO.
		if (ciclo == true)
		{
			if (valorCiclo < 100.0f)
			{
				valorCiclo += deltaTime * cicloOffset;
				shaderList[0].SetSpotLights(spotLights, spotLightCount);
			}
			else
				ciclo = false;
		}
		else
		{
			if (valorCiclo > -100.0f)
			{
				valorCiclo -= deltaTime * cicloOffset;
				shaderList[0].SetSpotLights(spotLights, spotLightCount - 1);
			}
			else
				ciclo = true;
		}

		*/

		//SKYBOX
		//movimiento del sol
		if (deltaTime <= 5.0) {
			contSol = 0;
		}

		if (contSol == 0) {
			if (movSol <= 1000.0f && movSol >= -1000.0f) {
				movSol -= movSolOffset * deltaTime;
			}
			else {
				movSol = 1000.0f;
			}
		}

		if (movSol >= 1000.0f && contSol == 0) {
			green = 0.0f;
			red = 0.0f;
		}

		if (movSol <= -500 && movSol >= -1000 && contSol == 0) {
			green -= (deltaTime * 0.0002);
			red -= (deltaTime * 0.0002);

		}

		if (movSol >= 500 && movSol <= 1000 && contSol == 0) {
			green += (deltaTime * 0.0002);
			red += (deltaTime * 0.0002);
		}

		if (green > 1.5f && contSol == 0) {
			green = 0.0f;
			red = 0.0f;
		}

		if ((movSol > 995 && movSol <= 1000 || movSol < -995 && movSol >= -1000) && contSol == 0) {
			std::vector<std::string> skyboxFaces;
			skyboxFaces.push_back("Textures/Skybox/escenario_noche_rt.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_noche_lf.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_noche_dn.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_noche_up.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_noche_bk.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_noche_ft.tga");



			skybox = Skybox(skyboxFaces);
		}

		if ((movSol > 845 && movSol <= 850 || movSol < -845 && movSol >= -850) && contSol == 0) {
			std::vector<std::string> skyboxFaces;
			skyboxFaces.push_back("Textures/Skybox/escenario_tarde_rt.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_tarde_lf.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_tarde_dn.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_tarde_up.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_tarde_bk.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_tarde_ft.tga");

			skybox = Skybox(skyboxFaces);
		}

		if ((movSol > 745 && movSol <= 750 || movSol < -745 && movSol >= -750) && contSol == 0) {
			std::vector<std::string> skyboxFaces;

			skyboxFaces.push_back("Textures/Skybox/escenario_mediodia_rt.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_mediodia_lf.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_mediodia_dn.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_mediodia_up.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_mediodia_bk.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_mediodia_ft.tga");

			skybox = Skybox(skyboxFaces);
		}

		if ((movSol <= 650 && movSol >= 645 || movSol <= -645 && movSol >= -650) && contSol == 0) {
			std::vector<std::string> skyboxFaces;
			skyboxFaces.push_back("Textures/Skybox/escenario_dia_rt.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_dia_lf.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_dia_dn.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_dia_up.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_dia_bk.tga");
			skyboxFaces.push_back("Textures/Skybox/escenario_dia_ft.tga");


			skybox = Skybox(skyboxFaces);
		}
		///printf("%f\n", movSol);
		//luz direccional, sólo 1 y siempre debe de existir
		mainLight = DirectionalLight(green, red, 1.0f,
			(green / 2) + 0.2f, (green / 2) + 0.2f,
			movSol, -500.0f, 0.0f);


		//Movimiento animación compleja
		if (movHeliyoffset > 360.0f)
			movHeliyoffset = 0.0f;

		movHeliy += movHeliyoffset;
		//Fin movimiento animacion compleja

		//Movimiento animación compleja Avión
		if (movAvionoffset > 360.0f)
			movAvionoffset = 0.0f;

		movAvion += movAvionoffset;
		//Fin movimiento animacion compleja Avión

		//Movimiento animacion simple
		if (deltaTime <= 1.0f)
		{
			if (avanza == true)
			{
				if (movCoche > -50.0f)
				{
					movCoche -= movOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotllanta += rotllantaOffset * deltaTime;
				}
				else
					avanza = false;
			}

			else
			{
				if (movCoche < 50.0f)
				{
					movCoche += movOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotllanta -= rotllantaOffset * deltaTime;
				}
				else
					avanza = true;
			}
		}

		//Fin movimiento animacion simple

		cameraAvatar = Camera(glm::vec3(mainWindow.getPosCamaraX(), 18.0f, mainWindow.getPosCamaraZ()),
			glm::vec3(0.0f, 1.0f, 0.0f),
			-mainWindow.getAng() + 90.0f,
			0.0f, 0.3f, 0.5f);

		//Recibir eventos del usuario
		glfwPollEvents();
		cameraAerea.keyControl(mainWindow.getsKeys(), deltaTime);
		cameraAerea.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		if (mainWindow.getControlCamera() > 0)
			camera = cameraAvatar;
		else
			camera = cameraAerea;

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		/*DIBUJAR TOROIDE CRISTOPHER*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, 7.0f, -170.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ta.render(); //dibuja TOROIDE

		/*DIBUJAR TOROIDE KARLA*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, 9.0f, -170.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		tb.render(); //dibuja TOROIDE 02

		/*DIBUJO DEL PISO*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(26.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		//--------------------------------------------------------------------------------------*//
		//
		//
		//	 
		//
		//--------------------------------------------------------------------------------------*//

									/*MODELOS LAB (DANIELA)*/

		//--------------------------------------------------------------------------------------*//
		//Nadder Cuerpo
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-150.0f, 40.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-150.0f, 40.0f + (2.0f * abs(sin(glm::radians(movHeliy * 0.1f)))), 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_Cuerpo.RenderModel();
		modelaux = model;

		//Nadder alas
		modelaux = model;
		model = glm::translate(model, glm::vec3(1.3f, 0.0f, -0.5f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(abs(sin(glm::radians(movHeliy * 0.1f))), 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_AlaD.RenderModel();

		model = modelaux;
		modelaux = model;
		model = glm::translate(model, glm::vec3(-1.3f, 0.0f, -0.5f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(abs(sin(glm::radians(movHeliy * 0.1f))), 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_AlaI.RenderModel();
		model = modelaux;

		//Nadder piernas
		modelaux = model;
		model = glm::translate(model, glm::vec3(1.75f, -2.3f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_MusloD.RenderModel();
		model = modelaux;
		modelaux = model;
		model = glm::translate(model, glm::vec3(-1.75f, -2.3f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_MusloI.RenderModel();
		model = modelaux;

		// Nadder cabeza
		model = glm::translate(model, glm::vec3(0.0f, 4.75f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_cabeza.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getangulollantas()), glm::vec3(1.0f, 0.0f, 0.0f));

		//Nadder cola
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_cola1.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.75f, -7.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getangulollantas()), glm::vec3(1.0f, 0.0f, 0.0f));

		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_cola2.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -5.0f));

		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Nadder_cola3.RenderModel();
		model = modelaux;

		//Skril cuerpo 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, 20.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(150.0f + movCoche, 0.5f, 70.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Skrill_cuerpo.RenderModel();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		// Skrill ala D
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Skrill_AlaD.RenderModel();
		model = modelaux;

		// Skrill ala I
		modelaux = model;
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Skrill_AlaI.RenderModel();
		model = modelaux;

		// Skrill cola
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Skrill_cola.RenderModel();
		//--------------------------------------------------------------------------------------*//


											/*MODELOS KARLA*/
		//--------------------------------------------------------------------------------------*//
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//PopCorn
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -230.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PopCorn_M.RenderModel();

		glDisable(GL_BLEND);

		//Plataforma
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 100.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Plataforma_M.RenderModel();


		//ESTATUTA KEROPPI
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 100.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Keroppi_estatua_M.RenderModel();

		//LAMPARAS SPOT
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 2.0f, 85.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara_Spot.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 2.0f, 85.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara_Spot.RenderModel();

		//ESTATUA KUROMI
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-122.0f, -1.0f, -180.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Estatua_A.RenderModel();

		//GAZEBO 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -1.0f, -170.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//GAZEBO 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, -200.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//GAZEBO 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -1.0f, -130.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//GAZEBO 04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, -150.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//GAZEBO 05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-56.0f, -1.0f, -230.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//GAZEBO 06
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-56.0f, -1.0f, -180.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//CARTEL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-122.0f, -1.0f, -125.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cartel_M.RenderModel();

		//LAMP 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.0f, -1.0f, 110.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp.RenderModel();

		//ON & OFF de la luz de la lampara. [TECLA X]
		if (mainWindow.getprenderluz()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}

		//LAMP 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, -1.0f, 110.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp.RenderModel();

		//Stan de comida
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -1.0f, -170.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_M.RenderModel();

		//Expendedora
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -1.0f, -130.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		expendedora.RenderModel();

		//Bocina
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -100.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bocina_M.RenderModel();

		//Arbusto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto_M.RenderModel();

		//Arbusto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, -15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto_M.RenderModel();

		//Arbusto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, -120.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto_M.RenderModel();

		//Arbusto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, -130.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto_M.RenderModel();

		//Puerta Principal
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaP_M.RenderModel();

		//Senial con mapa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 1.0f, 35.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Senal_M.RenderModel();

		//Bote de Basura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -1.0f, -160.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Basura_M.RenderModel();

		//Bote de Basura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-195.0f, -1.0f, -150.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Basura_M.RenderModel();

		//Palmera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Palmera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Palmera.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -135.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Palmera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -5.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Palmera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 50.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fruta.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 50.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fruta.RenderModel();

		//Cercas
		// 
		//Cercas del enfrente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(142.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(204.0f, -2.0f, -275.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca02_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-142.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-204.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		//Cercas de la parte trasera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(124.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(204.0f, -2.0f, 290.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca02_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-124.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-204.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca02_M.RenderModel();

		//Cercas del lado izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, -245.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca02_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, -180.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, -118.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, -55.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 7.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 69.3f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 131.6f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 194.4f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 257.2f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		//Cercas del lado derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, -245.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca02_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, -180.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, -118.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, -55.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 7.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 69.3f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 131.6f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 194.4f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 257.2f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		//PERSONAJES SANRIO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f + sin(glm::radians(movHeliy * 0.1f)), -1.0f, -80.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Penguin.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, -60.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(abs(sin(glm::radians(movHeliy * 0.1f))), 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Penguin.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, -50.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		model = glm::rotate(model, -95 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Penguin.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -1.0f, -75.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Ice.RenderModel();

		//CARTEL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -1.0f, -60.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cartel2_M.RenderModel();

		/*PERSONAJE KUROMI*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getPosX(), 4.0f, mainWindow.getPosZ()));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, glm::radians(mainWindow.getAng()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_kuromi.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cabeza.RenderModel();

		/*
		if (mainWindow.getPasos() == true){
			playPasos();
		}*/
		
		//Pata Izq.
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.0f, 4.0f, 0.0));
		model = glm::rotate(model, glm::radians(mainWindow.getAngPieIzq()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataIzq.RenderModel();
		model = modelaux;	

		//Pata Der.
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0, 4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getAngPieDer()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataDer.RenderModel();
		model = modelaux;

		//Brazo Izq.
		model = modelaux;
		model = glm::translate(model, glm::vec3(-6.0, 8.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getAngPieDer()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzq.RenderModel();
		model = modelaux;

		//Brazo Der.
		model = modelaux;
		model = glm::translate(model, glm::vec3(6.0, 8.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getAngPieIzq()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDer.RenderModel();
		model = modelaux;

		//Varita
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.0, 8.0f, 0.0f)); //TECLA Z
		model = glm::rotate(model, glm::radians(mainWindow.getmueve()), glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getprenderluz()), glm::vec3(-1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Varita.RenderModel();
		model = modelaux;

		if (mainWindow.getprender_luz2()) {
			shaderList[0].SetPointLights(pointLights2, pointLightCount2);

		}
		else {
			shaderList[0].SetPointLights(pointLights2, pointLightCount2 - 1);
		}

		if(mainWindow.getSonido())
		{
			playSonido();
		}
	


		//--------------------------------------------------------------------------------------*//


										/*MODELOS LAB (EDUARDO)*/
		//--------------------------------------------------------------------------------------*//
		/*ANIMALES POKEMON*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f + +sin(glm::radians(movHeliy * 0.1f)), -1.0f, -200.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 260 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dodrio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f + sin(glm::radians(movHeliy * 0.1f)), 10.0f, -200.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		model = glm::rotate(model, 260 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rowlet.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movCoche, -1.0f, 250.0f));
		//model = glm::translate(model, glm::vec3(150.0f + movCoche, 0.5f, 70.0f));

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(124.0f, -1.0f, 220.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-124.0f, -1.0f, 220.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -1.0f, 150.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Plant02.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Plant02.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 25.0f + abs(sin(glm::radians(movHeliy * 0.1f))), -220.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 260 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Zubat.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, 6.0f, -53.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Eeve.RenderModel();

		/*PERSONAJE PSYDUCK */
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + sin(4.0f * glm::radians(movHeliy * 0.5f)), 8.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckBase.RenderModel();

		modelaux = model;
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckPieI.RenderModel();
		model = modelaux;

		modelaux = model;
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckPieD.RenderModel();
		model = modelaux; 

		//--------------------------------------------------------------------------------------*//


									/*MODELOS CRISTOPHER*/
		//--------------------------------------------------------------------------------------*//			
		//BANCAS DEL DOMO G Y F
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, -1.0f, -20.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		//BANCAS DEL DOMO F Y E
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, -1.0f, -140.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, -125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		//BANCAS DEL DOMO GRANDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 295 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -1.0f, -95.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 260 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, -95.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 250 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		box.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		normal.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, 4.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 70 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		normal.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, 9.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 80 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		normal.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fruit.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, 20.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fruit.RenderModel();

		//ANIMALES CRASH BANDICOOT
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, 90.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		chicken.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -1.0f, 80.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		stork.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, 5.0f, 70.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vulture.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f + 30 * sin(glm::radians(movHeliy * 0.1f)), 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		car.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		metalUp.RenderModel(); 

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 7.0f + 30 * abs(sin(glm::radians(movHeliy * 0.1f))), -100.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		metal.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f + movCoche, 0.5f, 70.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kart.RenderModel();

		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 10.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kartllanta.RenderModel();

		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 10.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kartllanta.RenderModel();

		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, -10.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kartllanta.RenderModel();

		model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -10.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kartllanta.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, 30 * sin(glm::radians(movHeliy * 0.1f)), 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		avionCrash.RenderModel();

		//--------------------------------------------------------------------------------------*//

		//Carrera coches
		if (mainWindow.getControlCarrera() < 1)
		{
			posChoco = posChoco + 1.2 * deltaTime;
			posGolf = posGolf + 0.5 * deltaTime;
		}
		else
		{
			posChoco = 0.0f;
			posGolf = 0.0f;
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 2.0f, -280.0f));
		if (posChoco == 0.0f)
			posGolf = 0.0f;
		if (posGolf < (2 * 260.0f))
			model = glm::translate(model, glm::vec3(-posGolf, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		carroGolfBase.RenderModel();

		modelaux = model;
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckBase.RenderModel();
		model = glm::rotate(model, glm::radians(mainWindow.getAngPieIzq()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckPieI.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckPieD.RenderModel();
		model = modelaux;


		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-55.0f, -15.0f, 25.0f));
		model = glm::rotate(model, glm::radians(posGolf), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carroGolfLlanta.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(35.0f, -15.0f, 25.0f));
		model = glm::rotate(model, glm::radians(posGolf), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carroGolfLlanta.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(-55.0f, -15.0f, -25.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(posGolf), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carroGolfLlanta.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(35.0f, -15.0f, -25.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(posGolf), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carroGolfLlanta.RenderModel();
		model = modelaux;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 2.0f, -290.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		if (posChoco < (2 * 280.0f))
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -posChoco));
		else
			posGolf = 0.0f;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Coche_coco.RenderModel();


		modelaux = model;
		model = glm::translate(model, glm::vec3(-4.5f, -2.0f, 4.0f));
		model = glm::rotate(model, glm::radians(posChoco), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_coco.RenderModel();
		model = modelaux;


		modelaux = model;
		model = glm::translate(model, glm::vec3(-4.5f, -2.0f, -4.0f));
		model = glm::rotate(model, glm::radians(posChoco), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_coco.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(4.5f, -2.0f, 4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(posChoco), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_coco.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::translate(model, glm::vec3(4.5f, -2.0f, -4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(posChoco), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_coco.RenderModel();
		model = modelaux;

		///////////////////////////////////////////////////////////////////////////////////
											/*DOMOS*/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Caseta de vigilancia
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, -1.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Vigilancia.RenderModel();

		//JAULA A
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -0.5f, 95.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jaula_A.RenderModel();

		//DOMO GRANDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		DomoA.RenderModel();

		//BANDERA DRAGON
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -1.0f, -80.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bandera_H.RenderModel();

		//DOMO G
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, 65.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -55 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Domo.RenderModel();

		//BANDERA CRASH
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, -1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bandera_C.RenderModel();

		//DOMO F
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, -70.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Domo.RenderModel();

		//BANDERA KUROMI
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, -1.0f, -90.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bandera_K.RenderModel();

		//DOMO E
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, -200.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Domo.RenderModel();

		//BANDERA POKEMON
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, -1.0f, -220.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bandera_P.RenderModel();




		glDisable(GL_BLEND);

		//--------------------------------------------------------------------------------------*//
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return EXIT_SUCCESS;
}

