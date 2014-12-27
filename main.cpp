//*************les bibliothèques***************
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <vector>
using namespace std; //l'espace de noms std (on y trouve String , Vector , ...)

//*******var globales ********************************************************************

#define periode 300	// periode est la periode d'affichage 
						




int score = 0;

// Enumération Direction
enum Direction //enum pour fixer les directions qu'on  a !
{
	HAUT, BAS, GAUCHE, DROITE
};


// Fonction pour l'affichage du texte à l'écran
void Print(void *font, const char *texte, float x, float y) 
{
	glRasterPos2f(x, y);
	char *e = (char*) texte; 
	while (*e != '\0') glutBitmapCharacter(font, *e++); 
}

//*****************classes**************************
// Classe Coordonnees
class Coordonnees
{
public:
	int X ;
	int Y ;

public:
    Coordonnees() { //Constructeur par défaut , mais il est nécessaire pour l'initialisation !
        X=0; Y=0; 
    }
	// Constructeur			
	Coordonnees(int x, int y) { 
         X=x; Y=y; 
    }		
	Coordonnees(const Coordonnees& coordonnees) //Constructeur de copie qui est obligatoire dans notre cas !
	{
		X = coordonnees.X;
		Y = coordonnees.Y;
	}

	 void SetX(int x) {
           X = x; 
    }
	inline void SetY(int y) { 
           Y = y; 
     }
	inline void SetXY(int x, int y) { //set de couple(x,y)
           X = x; Y = y; 
     }

	inline int GetX() { 
       return X; 
    }
	inline int GetY() { 
       return Y; 
    }
};

// Déclaration de la classe Serpent
class Serpent
{
//Attributs
public:
	vector<Coordonnees> Vect; //vecteur de cooedonées
		vector<Coordonnees> Obst;//Obst vecteur
	int manger; //mangere de case à agrandir !

	Direction directionSerpent;
//Les methodes
public:
	Serpent();	// Constructeur par défaut
	Serpent( vector<Coordonnees> Vect,vector<Coordonnees> Obst, Direction dir = GAUCHE);	// Constructeur 
	Serpent(const Serpent& serpent);			// Constructeur de copie
	~Serpent();									// Destructeur

	void Affich();
	void Allonger(); //fonction pour agrandir notre serpent
	void Translation(Direction dir); //Pour decaler notre serpent
	inline vector<Coordonnees> GetCoordonnees() { //on a GetCoordonnees de type vector<Coordonnees>
	    return Vect; 
	}
	inline vector<Coordonnees> GetCoordonneess() { //on a GetCoordonnees de type vector<Coordonnees>
	    return Obst; 
	}
	void Reinitialiser();
	bool CollisionAvecSoi();
	
	void SensInterdit(Direction dir); //  sens interdit 
	
	inline Direction GetDirection() { //la fonction inline permet au compilateur de faire l'exécution au cette ligne
	    return directionSerpent; 
	} 
	
	bool CollisionAvecObstacle();//Pour detecter lacollision avec les Obstacles
	bool CollisionAvecWindow();//Pour detecter la collision avec le cadre
};

// Implémentation de la classe Serpent
Serpent::Serpent()	// Constructeur par défaut
{
	for(int i=0 ; i<3 ; i++) //Pour construire 3 pts au début de notre jeux
	{
		Vect.push_back(Coordonnees(4+i, 9));
	}
	manger = 0;
	
	directionSerpent = HAUT; //la directio par defaut au début de jeux!
	
	//Construction de 4 cube del'obstacle
//*******************************************************************************************	
		for(int i=0 ; i<4 ; i++) //Pour construire 7 pts au début de notre jeux
	{
		Obst.push_back(Coordonnees(8+i, 13));
			Obst.push_back(Coordonnees(8+i, 4));
			Obst.push_back(Coordonnees(13+i, 8));
	}
 //***************************************************************************************	
}

Serpent::Serpent(vector<Coordonnees> Vect,vector<Coordonnees> Obst, Direction dir)	// Constructeur
{
	Vect = Vect;
	Obst = Obst;
	manger = 0;
	directionSerpent = dir;
}

Serpent::Serpent(const Serpent& serpent)	// Constructeur de copie
{
    this->Obst = serpent.Obst;                   
	this->Vect = serpent.Vect;
	this->manger = serpent.manger;
	this->directionSerpent = serpent.directionSerpent;
}

Serpent::~Serpent()	{} // Destructeur

void Serpent::Affich()
{
  glColor3f(1, 0, 1); //Couleur de notre Serpent
  glBegin(GL_QUADS); 
	int x,y;
	for (int i=0 ; i<Vect.size() ; i++){
		x = Vect[i].GetX();
		y = Vect[i].GetY(); 
		//Nb : largeur de la case cad l'unité est 32 du coup ona 20 cellules à l'horizontale et 
		// 15 cellules verticalement.
		glVertex2i(x*32 + 2 ,  y*32 + 2);
		glVertex2i(x*32 + 32 - 2 , y*32 + 2);
		glVertex2i(x*32 + 32 - 2,y*32 + 32 - 2);
		glVertex2i(x*32 + 2,y*32 + 32 - 2);
	}
	
	//Déssiner un vecteur Obst qui contient les coordonnés de l'obstacle
	//*********************************************************************
	
	 glColor3f(0, 1, 0);
	for (int i=0 ; i<Obst.size() ; i++){
		x = Obst[i].GetX();
		y = Obst[i].GetY(); 
		//Nb : largeur de la case cad l'unité est 32 du coup ona 20 cellules à l'horizontale et 
		// 15 cellules verticalement.
		glVertex2i(x*32 + 2 ,  y*32 + 2);
		glVertex2i(x*32 + 32 - 2 , y*32 + 2);
		glVertex2i(x*32 + 32 - 2,y*32 + 32 - 2);
		glVertex2i(x*32 + 2,y*32 + 32 - 2);
	}
	//*********************************************************************
   glEnd();
}
// si le serpent mange un cube
void Serpent::Allonger() //Pour agrandir le serpent !
{
	manger++;
}

void Serpent::Translation(Direction dir) //Pour décaler 
{
	Coordonnees queue;//variable aux pour sauvegarder les coords du queue avant agrandir
	if (manger != 0)	// Si on doit agrandir
	{
		// On recupere les coordonnées de la queue 
		queue.SetX(Vect[Vect.size()-1].GetX());
		queue.SetY(Vect[Vect.size()-1].GetY());
	}
	//On décale tout sauf la tete                                                                      
	for (int i=Vect.size()-1 ; i>=1 ; i--)	// On parcourt le serpent
										   // de la queue jusqu'a l'avant tete
	{ //Chaque element prend les coordonnées de son precedent
		Vect[i].SetX(Vect[i-1].GetX());
		Vect[i].SetY(Vect[i-1].GetY());
	}
	
	// On décale la tete en fonction de la direction
	switch (dir)
	{
	case HAUT:
		Vect[0].SetY(Vect[0].GetY()+1);
		break;
	case BAS:
		Vect[0].SetY(Vect[0].GetY()-1);
		break;
	case GAUCHE:
		Vect[0].SetX(Vect[0].GetX()-1);
		break;
	case DROITE:
		Vect[0].SetX(Vect[0].GetX()+1);
		break;
	}

	if (manger != 0)	// Si on doit agrandir le serpent
	{
		// On rajoute à la fin le cube mangé
		Vect.push_back(queue);
		manger--;
	}
}

void Serpent::Reinitialiser()	// Equivalent au constructeur par défaut
{    
	Vect.clear();
	for(int i=0 ; i<3 ; i++)
	{
		Vect.push_back(Coordonnees(4+i, 9));// bech nsawrou 3 ka3bet 
	}
	manger = 0;
	directionSerpent = HAUT;
}

bool Serpent::CollisionAvecSoi()
{
	Coordonnees tete;// variable auxiliére qui va sauvegarder les coords de la téte
	tete.SetXY(Vect[0].GetX(), Vect[0].GetY());//on récupére les coordonnées (x et y)
	
	// On parcourt le serpent 
	for (int i=1 ; i<Vect.size() ; i++)
	{
		if (tete.GetX() == Vect[i].GetX() &&  //Si il touche son corps
                                               // un flag est déclanché
			tete.GetY() == Vect[i].GetY())
		{
			return true;
		}
	}
	return false;	// Dans tous les autres cas false !!!
}
//********************************************************************
//Collision de serpent avec l'obstacle

bool Serpent::CollisionAvecObstacle()
{
	Coordonnees tete;// variable auxiliére qui va sauvegarder les coords de la téte
	tete.SetXY(Vect[0].GetX(), Vect[0].GetY());//on récupére les coordonnées (x et y)
	
	// On parcourt le serpent 
	for (int i=0 ; i<Obst.size() ; i++)
	{
		if (tete.GetX() == Obst[i].GetX() &&  //Si il touche son corps
                                               // un flag est déclanché
			tete.GetY() == Obst[i].GetY())
		{
			return true;
		}
	}
	return false;	// Dans tous les autres cas false !!!
}
//********************************************************************

// cad ne peut pas rentrer au direction inverse 
void Serpent::SensInterdit(Direction dir)
{
	bool drapeau = true;	// =false si on ne peut pas faire de demi-tour
							
	switch(dir)
	{
	case HAUT://si la direction initiale du serpent est le bas, et le joueur a choisit le haut on l'interdit 
		if (Vect[1].GetY() == Vect[0].GetY()+1)	
		{
			drapeau = false;
		}
		break;
	case BAS://si la direction initiale du serpent est le haut, et le joueur a choisit le bas on l'interdit 
		if (Vect[1].GetY() == Vect[0].GetY()-1)	
		{
			drapeau = false;
		}
		break;
	case GAUCHE:
		if (Vect[1].GetX() == Vect[0].GetX()-1)	
		{
			drapeau = false;
		}
		break;
	case DROITE:
		if (Vect[1].GetX() == Vect[0].GetX()+1)	
		{
			drapeau = false;
		}
		break;
	}
	if(drapeau) //si drapeau est true on affecte la directionSerpent avec la direction actuelle dir
	{
		directionSerpent = dir;//direction choisit par le joueur
	}
}
// collision de serpent avec la fenétre
bool Serpent::CollisionAvecWindow() 
{
	if (Vect[0].GetX() > 20 ||Vect[0].GetX() < 0||Vect[0].GetY() > 15 ||Vect[0].GetY() < 0)
	{
		return true;
	}
	return false;
}

// Déclaration de la classe Gangner
class Gagner
{
private:
	int X, Y;
	
public:
	Gagner();
	Gagner(vector<Coordonnees> coords);
	Gagner(const Gagner& ref); //constructeur de copie 
	~Gagner();
	void Affich();
	inline int GetX() { 
	    return X; 
	}
	inline int GetY() { 
	    return Y; 
	}
	void RandomGagner(vector<Coordonnees> coords);//donne une position aléatoire au cube a manger

	inline void SetX(int x) { 
	    X = x; 
	}
	inline void SetY(int y) { 
	    Y = y; 
	}
	inline void SetXY(int x, int y) { 
	    X = x; 
	    Y = y; 
	}
};

// Implémentation de la classe Gagner
Gagner::Gagner(){}

Gagner::Gagner(vector<Coordonnees> coords)
{
	RandomGagner(coords);
}

Gagner::Gagner(const Gagner& ref)
{
	this->X = ref.X;
	this->Y = ref.Y;

}

Gagner::~Gagner(){}

void Gagner::Affich()
{
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
//Creation de la cube a manger 
	glVertex2i(X*32+ 2,Y*32 + 2);
	glVertex2i(X*32 + 32 - 2,Y*32 + 2);
	glVertex2i(X*32 + 32 - 2,Y*32 + 32 - 2);
	glVertex2i(X*32 + 2,Y*32 + 32 - 2);
	glEnd();
}

void Gagner::RandomGagner(vector<Coordonnees> coords)
{
	X = rand()% 20; //HORZ_NB_DE_CASES)unité 
	Y = rand()% 15;// de mm
	
    for(int i=0 ; i<coords.size() ; i++)	// On parcourt coords
	{
		if (X == coords[i].GetX() && Y == coords[i].GetY())	// Si les
						// coordonnees prises au hasard correspondent à une paire
						// de coordonnées de coords
		{
			i=0;		// On recommence
			X = rand()% 20;
			Y = rand()% 15;
		}
	}
}

// Objet Serpent global
Serpent serp;
Serpent obs;

// Objet Insecte global
Gagner gagne;

// Variables pour le temps
struct timeb dernierPassage; 
unsigned short tempsPasse = 0; 

// Drapeaux concernant l'état du jeu
bool Demarage_jeu = true;
bool Jouer = false;
bool Affiche_score = false;

//******************fin classes*********************


//**********************Play************************************
void Play()
{
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT /* GL_DEPTH_BUFFER_BIT*/);

	// Phase de démarrage
	if(Demarage_jeu)
	{
		glColor3f(1, 0, 0);
		Print(GLUT_BITMAP_HELVETICA_18, "Snake :p", 280, 340);
			glColor3f(1, 1, 1);
		Print(GLUT_BITMAP_HELVETICA_18, "***************************************************", 180, 310);
			glColor3f(1, 0, 0);
		Print(GLUT_BITMAP_HELVETICA_18, "Tapper sur une touche pour commencer a jouer ", 170, 280);
			glColor3f(1, 1, 1);
		Print(GLUT_BITMAP_HELVETICA_18, "****************************************************", 180, 250);
		glColor3f(1, 0, 0);
		Print(GLUT_BITMAP_HELVETICA_18, "Tapper sur échap pour quitter le jeu ", 200, 220);
	}
	else
	{
        //Si je suis dans la phase de jouer on va afficher le score 
        
		if (Jouer)
		{
                  
                  //des variables pour calculer les score avec codasqui
             	int a ,b=0;
			
             
             	glColor3f(1, 3, 2);
			Print(GLUT_BITMAP_HELVETICA_18, "SCORE :", 20, 530);
		
				glRasterPos2f(100, 530);//la position de score
				b = (score/10);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, b+48);
				a = score - (b*10);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, a+48);
				
		//Dessiner un line pour la limite en haut
		
		glColor3f(1, 1 , 1);
		glBegin(GL_LINES);
		glVertex3f(+1,520,0);
		glVertex3f(+650,520,0);
		glEnd();
		glFlush();	
        
        /*
        //Dessiner un obstacle dans la scéne
        
       	glColor3f(0, 1 , 1);
		glBegin(GL_QUAD_STRIP);
		glVertex3f(195,220,0);
		glVertex3f(223,220,0);
		glVertex3f(195,320,0);
		glVertex3f(223,320,0);
		glEnd();
		glFlush();	
		
		
		glColor3f(0, 1 , 1);
		glBegin(GL_QUAD_STRIP);
		glVertex3f(355,220,0);
		glVertex3f(383,220,0);
		glVertex3f(355,320,0);
		glVertex3f(383,320,0);
		glEnd();
		glFlush();	
		*/		
                  
                  
			// Si la tête et la cube a manger ont les mêmes coordonnées
			if (serp.GetCoordonnees()[0].GetX() == gagne.GetX() && serp.GetCoordonnees()[0].GetY() == gagne.GetY())
			{
				gagne.RandomGagner(serp.GetCoordonnees());	// On change la cube a manger de place tq ne prend pas les valeur de sepent
				gagne.RandomGagner(obs.GetCoordonnees());	// On change la cube a manger de place tq ne prend pas les valeurs des Obstacle
				// On agrandit le serpent
				serp.Allonger();
				// On incrémente le score
				score += 1;
			}
			// Dans tous les cas, on affiche le cube a manger
			gagne.Affich();
			
			// On teste si le serpent se mord la queue
			if (serp.CollisionAvecSoi())
			{
				Jouer = false;
				Affiche_score = true;//Passer ala phase d'affichage de score
			}
			
			// On teste si le serpent entre en collision avec l'obstacle
			
			if(serp.CollisionAvecObstacle()){
                                Jouer = false;
				Affiche_score = true;//Passer ala phase d'affichage de score        
   }
			
			

			// On teste si le serpent sort du cadre
			if (serp.CollisionAvecWindow())
			{
				Jouer = false;
				Affiche_score = true;//Passer ala phase d'affichage de score
			}

			// On décale le serpent dès que ça fait 100 ms depuis le dernier décalement
			struct timeb maintenant; // Structure returnée par la fonction 'ftime' .
	
			if(tempsPasse < periode)
			{
				ftime(&maintenant);
				tempsPasse = maintenant.millitm-dernierPassage.millitm;
			}
			else
			{
				tempsPasse = 0;
				ftime(&dernierPassage);
				serp.Translation(serp.GetDirection());
			}
			serp.Affich(); //Dans tous les cas on affiche le serpent !
		} 
		
		//Si je suis dans la phase d'affichage de score
		
		else if (Affiche_score){
             //des variables pour calculer les score avec codasqui
             	int d1 ,d2=0;
			
             
             	glColor3f(1, 0, 1);
			Print(GLUT_BITMAP_HELVETICA_18, "Votre score est :", 210, 350);
				glColor3f(1, 1, 1);
		Print(GLUT_BITMAP_HELVETICA_18, "***************************************************", 140, 320);
			glColor3f(1, 0, 1);
			Print(GLUT_BITMAP_HELVETICA_18,"Pour rejouer, tapez sur Entree.", 180, 300);
				glColor3f(1, 1, 1);
		Print(GLUT_BITMAP_HELVETICA_18, "***************************************************", 140, 270);
			glColor3f(1, 0, 1);
			Print(GLUT_BITMAP_HELVETICA_18, "Pour Quitter, tapez sur Echap.",200, 250);
				
				glRasterPos2f(350, 350);//la position de score
				d2 = (score/10);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, d2+48);
				d1 = score - (d2*10);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, d1+48);
				
				
             }
		
		}
	
    glutSwapBuffers(); 
	glutPostRedisplay();
    }

	

//************************fin Play ******************************

//**********************************Modifier *****************************

//cette fontion permet de modofier le repére en un repére orthonormé
void Modifier(int w, int h) //pour modifier le repere en un repere "orthonormé"
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);
	
}

//*******************************fin RModifier******************************

//*******************************Direction***********************************
void Direction(int key, int x, int y)
{
	if(Jouer)
	{
		switch(key)
		{
		case GLUT_KEY_UP:
			serp.SensInterdit(HAUT);
			break;
		case GLUT_KEY_DOWN:
			serp.SensInterdit(BAS);
			break;
		case GLUT_KEY_LEFT:
			serp.SensInterdit(GAUCHE);
			break;
		case GLUT_KEY_RIGHT:
			serp.SensInterdit(DROITE);
			break;
		default:
			break;
		}
	}
}
//**********************************fin Direction*********************

//*********************************Tapper***********************
void Tapper(unsigned char key, int x, int y)
{
	// Dans tous les cas
	if(key == 27) // Echap
	{
		exit(0);
	}
	if(Demarage_jeu)	// A l'appui d'une touche après le démarrage du jeu,
							// on commence le jeu.
	{
		Demarage_jeu = false;
		Jouer = true;
	}

	if (Affiche_score)	// A l'appui sur une touche pendant l'affichage
									// du score, on lance un nouveau jeu
	{
		Affiche_score= false;
		Jouer = true;
		serp.Reinitialiser();
		srand((unsigned int)time(NULL));
		gagne.RandomGagner(serp.GetCoordonnees());
		gagne.RandomGagner(obs.GetCoordonnees());
		
		
		score = 0;
	}


}
//********************************Fin Keyboard*********************


//***********************init********************************
void Init()
{
	glClearColor(0.0,0.0, 0.0, 1.0);//couleur de la scene

	
	ftime(&dernierPassage); 
	srand((unsigned int)time(NULL));

	gagne.RandomGagner(serp.GetCoordonnees());
	gagne.RandomGagner(obs.GetCoordonnees());
}
//***********************Fin init********************************
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA /*| GLUT_DEPTH*/);
	glutInitWindowPosition(200, 150);
	glutInitWindowSize(640, 550);
	glutCreateWindow("Snake Houssem_Eddine_Safar && Hasni_Badis ( Projet Synthése d'images )");
	Init();

	// Callbacks
	glutDisplayFunc(Play);
	glutReshapeFunc(Modifier);
	glutSpecialFunc(Direction);
	glutKeyboardFunc(Tapper);
	
//	atexit(EndProgram);

	glutMainLoop();

	return 0;
}
