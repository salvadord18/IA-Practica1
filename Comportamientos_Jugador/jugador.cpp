#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
	int a;

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

	// Decidir la nueva acción
	if((sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S') and sensores.superficie[2] == '_'){
		accion = actFORWARD;
	} else if(!girar_derecha){
		accion = actTURN_SL;
		girar_derecha = (rand()%2 == 0);
	} else {
		accion = actTURN_SR;
		girar_derecha = (rand()%2 == 0);
	}

	// Recordar la ultima accion
	last_action = accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
