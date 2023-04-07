#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
	int a;
	
	// Aquí se hace el reset del juego
	if(sensores.reset){
		reset();
	}

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este" << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur " << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste" << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;
	

	// Actualización de las variables de estado
	switch (last_action){
		case actFORWARD:
			if(!sensores.colision){
				// Actualización en caso de avanzar
				switch(current_state.brujula){
					case norte: current_state.fil--; break;
					case noreste: current_state.fil--; current_state.col++; break;
					case este: current_state.col++; break;
					case sureste: current_state.fil++; current_state.col++; break;
					case sur: current_state.fil++; break;
					case suroeste: current_state.fil++; current_state.col--; break;
					case oeste: current_state.col--; break;
					case noroeste: current_state.fil--; current_state.col--; break;
      			}
			};		
			break;
		case actTURN_SL:
			// Actualización en caso de girar 45º a la izquierda
			a =  current_state.brujula;
			a = (a+7)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_SR:
			// Actualización en caso de girar 45º a la derecha
			a =  current_state.brujula;
			a = (a+1)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_BL:
			// Actualización en caso de girar 135º a la izquierda
			a =  current_state.brujula;
			a = (a+6)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_BR:
			// Actualización en caso de girar 135º a la derecha
			a =  current_state.brujula;
			a = (a+2)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
	}

	if(sensores.nivel == 1 and !bien_situado){
		current_state.brujula = sensores.sentido;
	}

	if((sensores.terreno[0] == 'G' or sensores.nivel == 0) and !bien_situado){
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	if(bien_situado){
		//mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
		//incluirMapa();
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);
	}

	// Decidir la nueva acción
	if((sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or 
	    sensores.terreno[2] == 'G' or sensores.terreno[2] == 'D' or 
		sensores.terreno[2] == 'K' or sensores.terreno[2] == 'X' or 
		(sensores.terreno[2] == 'B' and zapatillas) or 
		(sensores.terreno[2] == 'A' and bikini) and 
		sensores.superficie[2] == '_')){
		accion = actFORWARD;
	} else if(!girar_derecha){
		accion = actTURN_SL;
		girar_derecha = (rand()%2 == 0);
	} else {
		accion = actTURN_SR;
		girar_derecha = (rand()%2 == 0);
	}

	if(sensores.terreno[0] == 'D' and !zapatillas){
		zapatillas = true;
	} else if(sensores.terreno[0] == 'K' and !bikini){
		bikini = true;
	}

	int bosque = 100;
	if (zapatillas){
		bosque = 5;
	}

	int agua = 100;
	if(bikini){
		agua = 5;
	}

	int precipicio = 100;
	int suelo = 5;
	int muro = 200;

	int posicion = 5;
	if(!bien_situado){
		posicion = -300;
	}

	int recarga = 5;
	if(sensores.bateria < 3000){
		recarga = -300;
	}

	int bikini = -300;
	if(bikini){
		bikini = 5;
	}

	// Recordar la ultima accion
	last_action = accion;
	return accion;
}

void ComportamientoJugador::PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector< vector< unsigned char> > &matriz){
	// Extiende esta version inicial donde solo se pone la componente 0 en matriz
	// a poner todas las componentes de terreno en función de la orientación del agente.
	

	switch(current_state.brujula){
		case sur:
			// Casillas a delante 
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil + 1][st.col] = terreno[2];
			matriz[st.fil + 2][st.col] = terreno[6];
			matriz[st.fil + 3][st.col] = terreno[12];

			// Casillas a la izquierda
			matriz[st.fil + 1][st.col - 1] = terreno[1];
			matriz[st.fil + 2][st.col - 1] = terreno[5];
			matriz[st.fil + 3][st.col - 1] = terreno[11];
			matriz[st.fil + 2][st.col - 2] = terreno[4];
			matriz[st.fil + 3][st.col - 2] = terreno[10];
			matriz[st.fil + 3][st.col - 3] = terreno[9];

			// Casillas a la derecha
			matriz[st.fil + 1][st.col + 1] = terreno[3];
			matriz[st.fil + 2][st.col + 1] = terreno[7];
			matriz[st.fil + 3][st.col + 1] = terreno[13];
			matriz[st.fil + 2][st.col + 2] = terreno[8];
			matriz[st.fil + 3][st.col + 2] = terreno[14];
			matriz[st.fil + 3][st.col + 3] = terreno[15];
		break;

		case sureste:
			matriz[st.fil][st.col + 1] = terreno[0];
			matriz[st.fil + 1][st.col + 1] = terreno[1];
			matriz[st.fil + 1][st.col + 2] = terreno[2];
			matriz[st.fil][st.col + 2] = terreno[3];
			matriz[st.fil + 2][st.col + 1] = terreno[4];
			matriz[st.fil + 2][st.col + 2] = terreno[5];
			matriz[st.fil + 2][st.col + 3] = terreno[6];
			matriz[st.fil + 1][st.col + 3] = terreno[7];
			matriz[st.fil][st.col + 3] = terreno[8];
			matriz[st.fil + 3][st.col + 1] = terreno[9];
			matriz[st.fil + 3][st.col + 2] = terreno[10];
			matriz[st.fil + 3][st.col + 3] = terreno[11];
			matriz[st.fil + 3][st.col + 4] = terreno[12];
			matriz[st.fil + 2][st.col + 4] = terreno[13];
			matriz[st.fil + 1][st.col + 4] = terreno[14];
			matriz[st.fil][st.col + 4] = terreno[15];
		break;

		case norte:
			// Casillas a delante 
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil - 1][st.col] = terreno[2];
			matriz[st.fil - 2][st.col] = terreno[6];
			matriz[st.fil - 3][st.col] = terreno[12];

			// Casillas a la izquierda
			matriz[st.fil - 1][st.col + 1] = terreno[1];
			matriz[st.fil - 2][st.col + 1] = terreno[5];
			matriz[st.fil - 3][st.col + 1] = terreno[11];
			matriz[st.fil - 2][st.col + 2] = terreno[4];
			matriz[st.fil - 3][st.col + 2] = terreno[10];
			matriz[st.fil - 3][st.col + 3] = terreno[9];

			// Casillas a la derecha
			matriz[st.fil - 1][st.col - 1] = terreno[3];
			matriz[st.fil - 2][st.col - 1] = terreno[7];
			matriz[st.fil - 3][st.col - 1] = terreno[13];
			matriz[st.fil - 2][st.col - 2] = terreno[8];
			matriz[st.fil - 3][st.col - 2] = terreno[14];
			matriz[st.fil - 3][st.col - 3] = terreno[15];
		break;

		case suroeste:
			matriz[st.fil][st.col - 1] = terreno[0];
			matriz[st.fil + 1][st.col - 1] = terreno[1];
			matriz[st.fil + 1][st.col - 2] = terreno[2];
			matriz[st.fil][st.col - 2] = terreno[3];
			matriz[st.fil + 2][st.col - 1] = terreno[4];
			matriz[st.fil + 2][st.col - 2] = terreno[5];
			matriz[st.fil + 2][st.col - 3] = terreno[6];
			matriz[st.fil + 1][st.col - 3] = terreno[7];
			matriz[st.fil][st.col - 3] = terreno[8];
			matriz[st.fil + 3][st.col - 1] = terreno[9];
			matriz[st.fil + 3][st.col - 2] = terreno[10];
			matriz[st.fil + 3][st.col - 3] = terreno[11];
			matriz[st.fil + 3][st.col - 4] = terreno[12];
			matriz[st.fil + 2][st.col - 4] = terreno[13];
			matriz[st.fil + 1][st.col - 4] = terreno[14];
			matriz[st.fil][st.col - 4] = terreno[15];
		break;

		case oeste:
			// Casillas a delante 
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil][st.col + 1] = terreno[2];
			matriz[st.fil][st.col + 2] = terreno[6];
			matriz[st.fil][st.col + 3] = terreno[12];

			// Casillas a la izquierda
			matriz[st.fil + 1][st.col + 1] = terreno[1];
			matriz[st.fil + 2][st.col + 1] = terreno[5];
			matriz[st.fil + 3][st.col + 1] = terreno[11];
			matriz[st.fil + 2][st.col + 2] = terreno[4];
			matriz[st.fil + 3][st.col + 2] = terreno[10];
			matriz[st.fil + 3][st.col + 3] = terreno[9];

			// Casillas a la derecha
			matriz[st.fil - 1][st.col - 1] = terreno[3];
			matriz[st.fil - 2][st.col - 1] = terreno[7];
			matriz[st.fil - 3][st.col - 1] = terreno[13];
			matriz[st.fil - 2][st.col - 2] = terreno[8];
			matriz[st.fil - 3][st.col - 2] = terreno[14];
			matriz[st.fil - 3][st.col - 3] = terreno[15];
		break;

		case este:
			// Casillas a delante 
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil][st.col - 1] = terreno[2];
			matriz[st.fil][st.col - 2] = terreno[6];
			matriz[st.fil][st.col - 3] = terreno[12];

			// Casillas a la izquierda
			matriz[st.fil - 1][st.col - 1] = terreno[1];
			matriz[st.fil - 2][st.col - 1] = terreno[5];
			matriz[st.fil - 3][st.col - 1] = terreno[11];
			matriz[st.fil - 2][st.col - 2] = terreno[4];
			matriz[st.fil - 3][st.col - 2] = terreno[10];
			matriz[st.fil - 3][st.col - 3] = terreno[9];

			// Casillas a la derecha
			matriz[st.fil + 1][st.col + 1] = terreno[3];
			matriz[st.fil + 2][st.col + 1] = terreno[7];
			matriz[st.fil + 3][st.col + 1] = terreno[13];
			matriz[st.fil + 2][st.col + 2] = terreno[8];
			matriz[st.fil + 3][st.col + 2] = terreno[14];
			matriz[st.fil + 3][st.col + 3] = terreno[15];
		break;

		case noreste:
			matriz[st.fil][st.col + 1] = terreno[0];
			matriz[st.fil - 1][st.col + 1] = terreno[1];
			matriz[st.fil - 1][st.col + 2] = terreno[2];
			matriz[st.fil][st.col + 2] = terreno[3];
			matriz[st.fil - 2][st.col + 1] = terreno[4];
			matriz[st.fil - 2][st.col + 2] = terreno[5];
			matriz[st.fil - 2][st.col + 3] = terreno[6];
			matriz[st.fil - 1][st.col + 3] = terreno[7];
			matriz[st.fil][st.col + 3] = terreno[8];
			matriz[st.fil - 3][st.col + 1] = terreno[9];
			matriz[st.fil - 3][st.col + 2] = terreno[10];
			matriz[st.fil - 3][st.col + 3] = terreno[11];
			matriz[st.fil - 3][st.col + 4] = terreno[12];
			matriz[st.fil - 2][st.col + 4] = terreno[13];
			matriz[st.fil - 1][st.col + 4] = terreno[14];
			matriz[st.fil][st.col + 4] = terreno[15];
		break;

		case noroeste:
			matriz[st.fil][st.col - 1] = terreno[0];
			matriz[st.fil - 1][st.col - 1] = terreno[1];
			matriz[st.fil - 1][st.col - 2] = terreno[2];
			matriz[st.fil][st.col - 2] = terreno[3];
			matriz[st.fil - 2][st.col - 1] = terreno[4];
			matriz[st.fil - 2][st.col - 2] = terreno[5];
			matriz[st.fil - 2][st.col - 3] = terreno[6];
			matriz[st.fil - 1][st.col - 3] = terreno[7];
			matriz[st.fil][st.col - 3] = terreno[8];
			matriz[st.fil - 3][st.col - 1] = terreno[9];
			matriz[st.fil - 3][st.col - 2] = terreno[10];
			matriz[st.fil - 3][st.col - 3] = terreno[11];
			matriz[st.fil - 3][st.col - 4] = terreno[12];
			matriz[st.fil - 2][st.col - 4] = terreno[13];
			matriz[st.fil - 1][st.col - 4] = terreno[14];
			matriz[st.fil][st.col - 4] = terreno[15];
		break;
	}
}

// La función reset pondrá todos los valores a los iniciales
void ComportamientoJugador::reset(){
	current_state.brujula = norte;
	current_state.col = current_state.fil = 99;
	girar_derecha = false;
	bien_situado = false;
	zapatillas = bikini = false;
	last_action = actIDLE;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}